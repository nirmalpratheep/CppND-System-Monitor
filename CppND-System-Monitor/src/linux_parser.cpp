#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  float memFree;
  float memTotal;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal")
        memTotal = value;
      else if (key == "MemFree") {
        memFree = value;
        break;
      }
    }
  }
  return ((memTotal - memFree) / memTotal);
}

long int LinuxParser::UpTime() {
  long int uptime,idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime; 
}

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuTimes;
  string key,line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key ; 
      if (key == "cpu") {
        for(int i=0;i<10;i++) {
          string val ;
          linestream >> val;
          cpuTimes.push_back(val);
        }
      }
    }
  }
  filestream.close();
  return cpuTimes;
 }

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

int LinuxParser::RunningProcesses() { string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

string LinuxParser::Command(int pid ) { 
  string cmd; 
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, cmd);
    if(cmd.length()>40)
      cmd = cmd.substr(0,40)+"...";
  }
  return cmd;
 }


string LinuxParser::Ram(int pid ) { 
  string line, key,val;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){ 
      std::stringstream linestream(line);
      while (linestream >> key >> val){
        if (key == "VmRSS:") {  // To get the exact physical memory used by the process
          return val.length()>3?val.substr(0,val.length()-3):val; // KB to MB
        }
      }
    }
    filestream.close();
 }
 return string(); 
 }

string LinuxParser::Uid(int pid ) { 
   string line, key, val{};
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){ 
      std::stringstream linestream(line);
      while (linestream >> key >> val){
        if (key.compare("Uid:")==0) {
          return val; 
        }
      }
    }
    filestream.close();
 }
 return val; 
  
}

vector<string> LinuxParser::splitString(string line,char delim) {
  vector<string> strs;
  int len = line.length();
  while(len>0) {
    string str = line.substr(0,line.find(delim));
    len = len - str.length()-1;
    strs.push_back(str);
    if(len>0)
      line = line.substr(line.find(delim)+1);
  }
  return strs;
}

string LinuxParser::User(int pid ) { 
  string uid = Uid(pid); 
  string line, key, val{};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      vector<string> strs = splitString(line,':');
        //std::cout<<strs.size()<<","<<strs[0]<<"-"<<uid<<std::endl;
        if (strs.size()>=2 && strs.at(2) == uid) {
          return strs[0]; 
        }
    }
  }
  filestream.close();
 return val; 
}


long LinuxParser::UpTime(int pid ) { 
  string line, key;
  long int startTime; 
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    vector<string> strs = splitString(line,' ');
    startTime = std::stol(strs.at(21).c_str());
    filestream.close();
    std::time_t duration = LinuxParser::UpTime() - (startTime/sysconf(_SC_CLK_TCK));
    return duration;
 }
 return 0; 
 }

 float LinuxParser::CpuUtilization(int pid) {
  string line, val; 
  long total_time, utime, stime, cutime, cstime, starttime;  
  float cpu{0.0}; 
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    vector<string> strs = splitString(line,' ');
    //std::cout<<strs.at(13)<<" "<<strs.at(14)<<" "<<strs.at(15)<<" "<<strs.at(16)<<" "<<strs.at(21)<<std::endl;
    utime = std::stol(strs.at(13).c_str());
    stime = std::stol(strs.at(14).c_str());
    cutime = std::stol(strs.at(15).c_str());
    cstime = std::stol(strs.at(16).c_str());
    starttime = std::stol(strs.at(21).c_str());
      
  }
  total_time = utime + stime + cutime + cstime; 
  long upTime = LinuxParser::UpTime();
  long sec = upTime - starttime/sysconf(_SC_CLK_TCK);
  cpu = (total_time/sysconf(_SC_CLK_TCK)*1.0)/sec; 
  filestream.close();
  return cpu;
 }
