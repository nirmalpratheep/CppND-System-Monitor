#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  


 private:
   float prevTotal{0};
   float prevIdle{0};
   int counter{0};
};

#endif