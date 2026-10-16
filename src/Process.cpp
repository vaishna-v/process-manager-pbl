#include "../include/Process.h"
#include <utility>

Process::Process(pid_t pid, pid_t ppid, std::string name, char state, double cpuUsage, long memoryUsage, std::string latestAction)
    : pid(pid), ppid(ppid), name(std::move(name)), state(state), cpuUsage(cpuUsage), memoryUsage(memoryUsage), latestAction(std::move(latestAction)) {}