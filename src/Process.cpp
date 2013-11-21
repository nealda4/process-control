#include <sys/types.h>
#include <sys/wait.h>
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
};

Process::Process(const std::string & cmd)
{
	// evaluate word expression
	wordexp_t result;
	switch(wordexp(cmd.c_str(), &result, 0))
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

	/// @todo check to see that the first argument path actually exists and is executable (throw exception if not)

	pid_t pid = fork();
	if (pid == -1)
	{
		// throw some exception indicating fork failure
		throw std::system_error(errno, std::system_category(), "fork failed");
	}
	else if(pid == 0)
	{
		// set child process group id to the pid of the child
		// so that killing it kills the entire process group
	    setpgid(0, 0);

	    // execute with argument array
	    if (execv(result.we_wordv[0], result.we_wordv) == -1)
	    {
	    	// exec failed but there isn't much we can do
	    	int error = errno;
	    	std::cerr << "ABORTING: execution of '" << cmd << "' failed with " << strerror(error) << std::endl;
	    	abort();
	    }
	}
	else
	{
		// allocate private impl data and store the pid
		priv_ = std::unique_ptr<ProcessPrivate>(new ProcessPrivate());
		priv_->pid = pid;
	}

	wordfree(&result);
	return;
}

Process::~Process()
{
	/// @todo
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
	throw std::runtime_error ("dumpadoodledoo");
	// this should never happen
	abort();
}

} // namespace Process
