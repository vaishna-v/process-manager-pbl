#include "../include/TUI.h"
#include <ncurses.h>
#include <algorithm>

TUI::TUI(ProcessManager& manager) : manager(manager) {}

void TUI::Run() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(1000); 

    std::string sName = PromptUser("Enter Sandbox Name to monitor: ");
    manager.SetSandboxName(sName);

    bool running = true;
    int scrollOffset = 0;

    while (running) {
        erase();
        
        mvprintw(0, 0, "Sandbox Monitor [%s] | [q] Exit | [Enter/Down/Up] Scroll", manager.GetSandboxName().c_str());
        
        mvprintw(2, 0, "%-10s %-10s %-20s %-10s %-15s %-15s", "PID", "PPID", "NAME", "STATE", "MEMORY", "LATEST ACTION");
        mvhline(3, 0, '-', 85);

        auto processes = manager.DiscoverProcesses();
        int totalProcesses = processes.size();
        int maxDisplay = LINES - 4;

        if (scrollOffset > std::max(0, totalProcesses - maxDisplay)) {
            scrollOffset = std::max(0, totalProcesses - maxDisplay);
        }

        int row = 4;
        for (int i = scrollOffset; i < totalProcesses && row < LINES; ++i) {
            mvprintw(row++, 0, "%-10d %-10d %-20s %-10c %-15ld %-15s", 
                     processes[i].pid, processes[i].ppid, processes[i].name.c_str(), 
                     processes[i].state, processes[i].memoryUsage, processes[i].latestAction.c_str());
        }

        refresh();

        int ch = getch();
        if (ch == 'q') {
            running = false;
        } else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER || ch == KEY_DOWN) {
            if (scrollOffset < totalProcesses - maxDisplay) scrollOffset++;
        } else if (ch == KEY_UP) {
            if (scrollOffset > 0) scrollOffset--;
        }
    }
    endwin();
}

std::string TUI::PromptUser(const std::string& prompt) {
    timeout(-1); 
    echo();      
    
    move(1, 0);
    clrtoeol();
    printw("%s", prompt.c_str());
    
    char buffer[256];
    getnstr(buffer, 255);
    
    noecho();    
    timeout(1000); 
    return std::string(buffer);
}