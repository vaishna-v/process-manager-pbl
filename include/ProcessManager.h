#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Process.h"
#include "../../dbms/DatabaseAdapter.hpp"

class ProcessManager {
private:
    std::string sandboxName;
    std::unique_ptr<DatabaseAdapter> db;

public:
    void SetSandboxName(const std::string& name);
    std::string GetSandboxName() const;
    std::vector<Process> DiscoverProcesses();
};