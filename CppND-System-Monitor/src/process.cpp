#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { 
    pid_ = pid; 
    user_ = LinuxParser::User(pid_);
    cmd_ = LinuxParser::Command(pid_);
    uptime_ = LinuxParser::UpTime(pid_);
    ram_ = LinuxParser::Ram(pid_);
    cpuUtil_ = LinuxParser::CpuUtilization(pid_);
}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const { return cpuUtil_; }

string Process::Command() const { return cmd_; }

string Process::Ram() const { return ram_;}

string Process::User() const { return  user_;}

long int Process::UpTime() const { return uptime_; }

bool Process::IsValid() const{
   if(Command()!="" && Ram()!="") 
        return true;
   return false;
}

bool Process::operator<(Process const& a) const { 
    return CpuUtilization()<a.CpuUtilization();
}
