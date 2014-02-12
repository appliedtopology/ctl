#ifndef CTLIB_TIMER_H
#define CTLIB_TIMER_H

#include <ctime>
#include <iostream>  // cerr 

#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef WITH_TBB
//TBB timer
#include <tbb/tick_count.h>
#endif

//exported functionality
namespace ctl{

class Timer {     
public:
  Timer() : time_(){}

  // method:  start
  // starts timer
  void start() { time_ = std::clock(); }

  // method:  stop
  // returns time since start() in seconds
  void stop() { time_ = ((double)(std::clock()- time_))/CLOCKS_PER_SEC; }
  double elapsed() const { return time_; } 
private:
  double time_;
}; // class Timer

#ifdef WITH_TBB
class Thread_timer {     
public:
  Thread_timer() : time_(){}

  // method:  start
  // starts timer
  void start() { time_ = tbb::tick_count::now(); }

  // method:  stop
  // returns time since start() in seconds
  void stop() { time_ = (tbb::tick_count::now() - time_).seconds(); }
  void elapsed() const { return time_; } 
private:
  tbb::tick_count time_;
}; // class Thread_timer
#endif

} //namespace ctl

//non-exported functionality
namespace {

} //anonymous namespace

#endif //CTLIB_TIMER_H
