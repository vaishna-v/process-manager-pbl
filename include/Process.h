#pragma once
#include <string>
#include <sys/types.h>

class Process {
public:
    pid_t pid;
    pid_t ppid;
    std::string name;
    char state;
    double cpuUsage;
    long memoryUsage;
    std::string latestAction;

    Process(pid_t pid, pid_t ppid, std::string name, char state, double cpuUsage, long memoryUsage, std::string latestAction);
};