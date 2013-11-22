#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <wordexp.h>
#include <cstring>
#include <exception>
#include <system_error>
#include <sstream>
#include <iostream>
#include <process-control/Process.hpp>


namespace ProcessControl
{
struct ProcessPrivate
{
	pid_t pid;
	wordexp_t result;
	int pipefd[2];
};

Process::Process(const std::string & cmd)
{

	priv_ = std::unique_ptr<ProcessPrivate>(new ProcessPrivate());

	// pipe with close on exec
	if (pipe2(priv_->pipefd, O_CLOEXEC)  == -1)
	{
		throw std::system_error(errno, std::system_category(), "pipe2 failed");
	}

	// evaluate word expression
	switch(wordexp(cmd.c_str(), &priv_->result, 0))
	{
		case 0:
			// success
			break;
		case WRDE_BADCHAR:
			throw std::invalid_argument("Illegal  occurrence of newline or one of |, &, ;, <, >, (, ), {, }.");
		case WRDE_BADVAL:
			throw std::invalid_argument("An undefined shell variable was referenced, and the WRDE_UNDEF flag told us to consider this an error.");
		case WRDE_CMDSUB:
			throw std::invalid_argument("Command  substitution  occurred, and the WRDE_NOCMD flag told us to consider this an error.");
		case WRDE_NOSPACE:
			throw std::runtime_error("Out of memory.");
		case WRDE_SYNTAX:
			throw std::invalid_argument("Shell syntax error, such as unbalanced parentheses or unmatched quotes.");
		default:
			throw std::runtime_error("unknown wordexp error");
	}

	priv_->pid = fork();
	if (priv_->pid == -1)
	{
		// throw some exception indicating fork failure
		throw std::system_error(errno, std::system_category(), "fork failed");
	}
	else if(priv_->pid == 0)
	{
		// ---- child process ----

		// close read end of pipe
		static_cast<void>(close(priv_->pipefd[0]));

		// set child process group id to the pid of the child
		// so that killing it kills the entire process group
	    setpgid(0, 0);

	    // execute with argument array (pipes will close to notify parent that exec succeeded)
	    if (execv(priv_->result.we_wordv[0], priv_->result.we_wordv) == -1)
	    {
	    	// exec failed but there isn't much we can do
	    	int error = errno;

	    	// write errno to pipe to notify
	    	uint8_t * buf = reinterpret_cast <uint8_t *> (& error);
	    	for (size_t total = 0 ; total < sizeof(error); )
	    	{
	    		ssize_t count = write(priv_->pipefd[1], &buf[total],sizeof(error) - total);
	    		if (count < 0)
	    		{
	    			// if acceptable error, continue
	    			if (errno == EINTR)
	    			{
	    				continue;
	    			}
	    		}
	    		total += count;
	    	}

	    	// close write end of pipe
	    	static_cast<void>(close(priv_->pipefd[1]));
	    	abort();
	    }
	}

	// ---- parent process ----

	// close write end of pipe
	static_cast<void>(close(priv_->pipefd[1]));

	// read error code or get pipe closed out of pipe to notify process that exec failed
	int error = 0;
	uint8_t * buf = reinterpret_cast<uint8_t *> (&error);
	for (size_t total = 0; total < sizeof(error); )
	{
		ssize_t count = read(priv_->pipefd[0], buf, sizeof(error) - total);
		if (count < 0)
		{
			if (errno == EINTR)
			{
				// interrupted in the middle of read
				continue;
			}

			// close read end of pipe
			static_cast<void>(close(priv_->pipefd[0]));
			throw std::system_error(errno, std::system_category(), "read failed with " + std::string(strerror(errno)));
		}
		else if (count == 0)
		{
			if (total != 0)
			{
				// partial read failure
				// close read end of pipe
				static_cast<void>(close(priv_->pipefd[0]));
				throw std::runtime_error("error reading exec error");
			}

			// pipe has closed and there was no data
			// which indicates that exec has succeeded
			// now we can return
			break;
		}

		total += count;
	}

	// close read end of pipe
	static_cast<void>(close(priv_->pipefd[0]));

	if (error != 0)
	{
		throw std::system_error(error, std::system_category(), "exec failed with " + std::string(strerror(error)));
	}
}

Process::~Process()
{
	/// @todo kill the process

	static_cast<void>(close (priv_->pipefd[0]));
	static_cast<void>(close (priv_->pipefd[1]));
	wordfree(&priv_->result);
}

std::uint8_t Process::wait()
{
	int status;

	// does this have to be in a loop?
	if (-1 == waitpid(priv_->pid, &status, 0))
	{
		throw std::system_error(errno, std::system_category(), "waitpid failed");
	}

	if (WIFEXITED(status))
	{
		// process exited normally (exited or returned from main)
		return static_cast<std::uint8_t>(WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		// process was killed by a signal
		bool dumpedCore = WCOREDUMP(status);
		std::stringstream stream;
		stream << "process exited abnormally with signal: " << strsignal(WTERMSIG(status)) << "(" << WTERMSIG(status) << ")";
		if ( WCOREDUMP(status))
		{
			stream << " ... dumped core";
		}
		throw std::runtime_error(stream.str());
	}

	// this should never happen
	abort();
}

} // namespace Process
