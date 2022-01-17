#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;                              
  std::string User() const;                     
  std::string Command() const;                  
  float CpuUtilization() const;  
  bool IsValid() const;               
  std::string Ram() const;                      
  long int UpTime() const;                      
  bool operator<(Process const& a) const; 
  Process(int pid);  
  ~Process() {}
 private:
   std::string user_;
   std::string cmd_;
   long int uptime_;
   std::string ram_;
   float cpuUtil_;
   int pid_;
};

#endif