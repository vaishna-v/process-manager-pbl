#pragma once
#include <vector>
#include <string>
#include "Process.h"

class ProcessManager {
private:
    std::string sandboxName;

public:
    void SetSandboxName(const std::string& name);
    std::string GetSandboxName() const;
    std::vector<Process> DiscoverProcesses();
};