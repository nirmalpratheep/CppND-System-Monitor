#include <vector>
#include <iostream>
#include "processor.h"
#include "linux_parser.h"

using namespace std;

float Processor::Utilization() { 
    vector<string> cpuTimes = LinuxParser::CpuUtilization();
    long user,nice,system,idle,iowait,irq,softirq,steal;
    user = stol(cpuTimes.at(0).c_str());
    nice = stol(cpuTimes.at(0).c_str());
    system = stol(cpuTimes.at(0).c_str());
    idle = stol(cpuTimes.at(0).c_str());
    iowait = stol(cpuTimes.at(0).c_str());
    irq = stol(cpuTimes.at(0).c_str());
    softirq = stol(cpuTimes.at(0).c_str());
    steal = stol(cpuTimes.at(0).c_str());
    long nonIdle = user + nice + system + irq + softirq + steal;
    long idleTotal = idle + iowait;
    long total = idleTotal + nonIdle;
    long totalld = total - prevTotal;
    long idled = idleTotal - prevIdle;
    prevIdle = idleTotal;
    prevTotal = total;
    return (totalld - idled)*1.0/totalld; 
}