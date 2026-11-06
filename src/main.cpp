#include "../include/ProcessManager.h"
#include "../include/TUI.h"

int main() {
    ProcessManager manager;
    TUI tui(manager);
    tui.Run();
    return 0;
}