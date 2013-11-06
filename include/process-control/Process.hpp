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
	/// @exception ???
	Process(const std::string & cmd);

	/// Destructor
	virtual ~Process();

	/// @brief Wait for a process to complete.
	/// @retval exit code of the spawned process
	/// @exception ??? process did not exit normally
	std::uint8_t wait();

private:
	std::unique_ptr<ProcessPrivate> priv_;
};

} // namespace Process

#endif
