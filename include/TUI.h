#pragma once
#include "ProcessManager.h"
#include <string>

class TUI {
private:
    ProcessManager& manager;
    std::string PromptUser(const std::string& prompt);

public:
    explicit TUI(ProcessManager& manager);
    void Run();
};