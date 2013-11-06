#include <signal.h>

int main(int argc, const char ** argv)
{
	if (argc == 2)
	{
		/// @todo convert argv[1] to uint8_t and "exit"
	}
	else
	{
		/// @todo exit abnormally
		raise(SIGSEGV);
	}
}
