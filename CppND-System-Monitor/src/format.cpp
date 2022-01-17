#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

using std::string;
using std::stringstream;
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long time) {
    int hour = 0;
    int min = 0;
    int sec = 0;
    hour = time/3600;
    time = time%3600;
    min = time/60;
    time = time%60;
    sec = time;
    stringstream ss;
    ss<<std::setfill('0') << std::setw(2) << hour << ':' << std::setw(2) << min
              << ':' << std::setw(2) << sec ; 
    return ss.str(); 
}