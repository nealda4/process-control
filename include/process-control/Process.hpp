#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <cstdint>
#include <memory>

namespace ProcessControl
{

class ProcessPrivate;

class Process
{
public:
	/// @brief Constructor
	/// @param cmd path followed by arguments separated by spaces
	/// @exception system_error if there was a problem creating the process
	/// @exception runtime_error if there was a problem communicating with child process
	Process(const std::string & cmd);

	/// Destructor
	virtual ~Process();

	/// @brief Wait for a process to complete.
	/// @retval exit code of the spawned process
	/// @exception system_error if wait fails
	/// @exception runtime_error if child process did not exit normally
	std::uint8_t wait();

private:
	std::unique_ptr<ProcessPrivate> priv_;
};

} // namespace Process

#endif
