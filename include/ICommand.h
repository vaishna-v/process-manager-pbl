#pragma once
#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual std::string Execute() = 0;
};