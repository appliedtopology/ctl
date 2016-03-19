#ifndef CTLIB_TIMER_H
#define CTLIB_TIMER_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
#include <iostream>  // cerr 
#include <chrono>

#ifdef _OPENMP
#include <omp.h>
#endif

//exported functionality
namespace ctl{

class Timer {    
	typedef std::chrono::steady_clock Clock;
	typedef typename Clock::time_point time_point;
public:
  Timer() : start_(), stop_(){}

  // method:  start
  // starts timer
  void start() { start_ = stop_ = Clock::now(); }

  // method:  stop
  // returns time since start() in units of T, which defaults to seconds.
  template< typename T = std::chrono::seconds>
  double elapsed() const {
	typedef std::chrono::duration< double, typename T::period> D; 
	return (std::chrono::duration_cast< D>(Clock::now() - start_).count());  
  } 
private:
  time_point start_;
  time_point stop_;
}; // class Timer

} //namespace ctl

#endif //CTLIB_TIMER_H
