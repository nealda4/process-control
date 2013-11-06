#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <wordexp.h>
#include <process-control/Process.hpp>


namespace ProcessControl
{
struct ProcessPrivate
{
	pid_t pid;
};

Process::Process(const std::string & cmd)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		/// @todo throw some exception indicating fork failure
	}
	else if(pid == 0)
	{
		// set child process group id to the pid of the child
		// so that killing it kills the entire process group
	    setpgid(0, 0);

	    wordexp_t result;
	    switch(wordexp(cmd.c_str(), &result, 0))
	    {
	    	case 0:
	    		// success
	    		break;
	    	case WRDE_BADCHAR:
	    		// Illegal  occurrence of newline or one of |, &, ;, <, >, (, ), {, }.
	    	case WRDE_BADVAL:
	    		// An undefined shell variable was referenced, and the WRDE_UNDEF flag
	    		// told us to consider this an error.
	    	case WRDE_CMDSUB:
	        	// Command  substitution  occurred, and the WRDE_NOCMD flag
	    		// told us to consider this an error.
	    	case WRDE_NOSPACE:
	            // Out of memory.
	    	case WRDE_SYNTAX:
	    		// Shell syntax error, such as unbalanced parentheses or unmatched	quotes.
	    	default:
	    		// throwing an exception here doesn't do anything (process has already forked)
	    		// abort to terminate the program.  calling wait on this process can see that
	    		// the process was killed with SIGABRT
	    		abort();
	    }

	    // execute with argument array
	    execv(result.we_wordv[0], result.we_wordv);

	    // should never happen
	    abort();
	}
	else
	{
		// allocate private impl data and store the pid
		priv_ = std::unique_ptr<ProcessPrivate>(new ProcessPrivate());
		priv_->pid = pid;
	}
	return;
}

Process::~Process()
{
	/// @todo
}

std::uint8_t Process::wait()
{
	/// @todo
	int status;

	// does this have to be in a loop?
	if (-1 == waitpid(priv_->pid, &status, 0))
	{
		/// @todo throw exception with error string
	}

	if (WIFEXITED(status))
	{
		// process exited normally (exited or returned from main)
		return static_cast<std::uint8_t>(WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		// process was killed by a signal
		/// @todo throw exception with signal used to kill the process
	}
	else
	{
		// should not occur
		/// @todo throw exception or assert
	}
	abort();
}

} // namespace Process
