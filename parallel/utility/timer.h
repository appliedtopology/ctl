#ifndef CTLIB_TBB_TIMER_H
#define CTLIB_TBB_TIMER_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the license below, but, please
* do not commit acts of academic dishonesty. We strongly encourage and request 
* that for any [academic] use of this source code one should cite one the 
* following works:
* 
* \cite{hatcher, z-ct-10}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
#include <chrono>
#include <iostream>  // cerr 

#include <tbb/tick_count.h>

//exported functionality
namespace ctl{
namespace parallel{
class Timer {    
	typedef tbb::tick_count Clock;
	typedef typename tbb::tick_count time_point;
public:
  Timer() : start_(), stop_(){}

  // method:  start
  // starts timer
  void start() { start_ = stop_ = Clock::now(); }
  // method:  stop
  void stop() { stop_ = Clock::now(); } 

  template< typename T = std::chrono::seconds>
  double elapsed() const { return (stop_ - start_).seconds();  } 
private:
  time_point start_;
  time_point stop_;
}; // class Timer
} //namespace parallel
} //namespace ctl

#endif //CTLIB_TBB_TIMER_H
