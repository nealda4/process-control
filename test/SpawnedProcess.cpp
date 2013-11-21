#include <signal.h>
#include <cstdint>
#include <sstream>
#include <iostream>

int main(int argc, const char ** argv)
{
	if (argc == 2)
	{
		//  convert argv[1] to uint8_t and "exit"
		std::stringstream ss; ss << argv[1];
		int exitNumber;
		ss >> exitNumber;
		std::cerr << static_cast<int>(exitNumber) << std::endl;
		exit(static_cast<uint8_t>(exitNumber));
	}

	// exit abnormally
	raise(SIGHUP);
	raise(SIGSEGV);
}
