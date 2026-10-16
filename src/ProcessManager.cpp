#include "../include/ProcessManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unistd.h>

namespace fs = std::filesystem;

void ProcessManager::SetSandboxName(const std::string& name) {
    sandboxName = name;
}

std::string ProcessManager::GetSandboxName() const {
    return sandboxName;
}

std::vector<Process> ProcessManager::DiscoverProcesses() {
    std::vector<Process> processes;
    
    if (sandboxName.empty()) return processes;

    std::string cgroupProcsPath = "/sys/fs/cgroup/" + sandboxName + "/cgroup.procs";
    std::ifstream procsFile(cgroupProcsPath);
    
    if (!procsFile.is_open()) return processes;

    std::string pidStr;
    while (std::getline(procsFile, pidStr)) {
        if (pidStr.empty()) continue;
        pid_t pid = std::stoi(pidStr);

        std::ifstream statFile("/proc/" + pidStr + "/stat");
        std::ifstream syscallFile("/proc/" + pidStr + "/syscall");
        
        if (statFile.is_open()) {
            std::string line;
            if (std::getline(statFile, line)) {
                std::istringstream iss(line);
                std::string comm, latestAction = "running";
                char state;
                pid_t ppid;
                int temp_pid;
                
                iss >> temp_pid;
                
                size_t startName = line.find('(');
                size_t endName = line.rfind(')');
                if (startName != std::string::npos && endName != std::string::npos) {
                    comm = line.substr(startName + 1, endName - startName - 1);
                    iss.seekg(endName + 1);
                }
                
                iss >> state >> ppid;
                
                if (syscallFile.is_open()) {
                    std::string sysLine;
                    if (std::getline(syscallFile, sysLine)) {
                        std::istringstream sysIss(sysLine);
                        std::string sysNum;
                        sysIss >> sysNum;
                        if (sysNum != "running") {
                            latestAction = "syscall_" + sysNum;
                        }
                    }
                }

                // Check for exact Sandbox memory telemetry first
                long memoryUsage = 0;
                std::ifstream usageFile("/tmp/sandbox_mem_" + pidStr + ".txt");
                if (usageFile.is_open()) {
                    usageFile >> memoryUsage;
                } else {
                    // Fallback to OS RSS if not a sandboxed program
                    long rss;
                    for(int i = 0; i < 19; ++i) {
                        std::string dummy;
                        iss >> dummy;
                    }
                    iss >> rss;
                    memoryUsage = rss * sysconf(_SC_PAGESIZE);
                }

                processes.emplace_back(pid, ppid, comm, state, 0.0, memoryUsage, latestAction);
            }
        }
    }
    return processes;
}