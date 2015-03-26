#ifndef CTL_PARALLEL_PERSISTENCE_H
#define CTL_PARALLEL_PERSISTENCE_H
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
*******************************************************************************
* ********** BSD-3 License ****************
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
//CTL
#include <ctl/io/io.hpp>
#include <ctl/persistence/persistence.hpp>
#include <ctl/parallel/utility/timer.hpp>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_vector.h>
#include <boost/functional/hash.hpp>

namespace ctl {
namespace parallel{
template<typename Timer>
struct Parallel_stats{
	Timer timer;
	double parallel_persistence;
	double initialize_cascade_boundary;
	double filtration_time;
	double get_iterators;
};

template<typename Blowup>
inline std::ostream& operator<<( std::ostream& out, 
				 const typename Blowup::iterator &i)
{
  out << i->second.id() << " " << i->first;
  //out << i->first;
  return out;
}
} //namespace parallel
} // namespace ctl

namespace std {
template<typename Blowup>
std::size_t hash_value( const typename Blowup::iterator& cell)
{
  boost::hash< std::size_t> hasher;
  return hasher( cell->second.id());
}
}
namespace ctl {
namespace parallel{
template<typename Boundary,
         typename Cascade_map,
	 typename Vector>
class Persistence_body{
        private:
        typedef Persistence_body< Boundary,
                                  Cascade_map, Vector> Self;
        public:
        Persistence_body( Boundary & boundary,
                          Cascade_map & cascade_prop_map, Vector & v ):
                        _boundary( boundary),
                        _cascade_prop_map( cascade_prop_map), times( v){}

        Persistence_body( const Self & _self):
                        _boundary( _self._boundary),
                        _cascade_prop_map( _self._cascade_prop_map), 
			times( _self.times){}

        template< typename Range>
        void operator()( const Range & r ) const{
                typedef typename Range::const_iterator Iterator;
                for(Iterator current = r.begin();
                             current != r.end();
                           ++current){
		
		times.push_back( ctl::persistence( current->first,
				  current->second,
				  _boundary,
		 		  _cascade_prop_map, false, _cascade_prop_map.index()));
		}
        }

        private:
        Boundary & _boundary;
        Cascade_map & _cascade_prop_map;
	Vector& times;
}; //Persistence_body

template< typename Iterator_pairs, 
	  typename Boundary, 
	  typename Cascade_map, typename Filtration_map=ctl::identity>
std::pair< double, double> 
persistence( Iterator_pairs & ranges, 
 	     Boundary & complex_boundary,
 	     Cascade_map & cascade_prop_map,
 	     size_t & num_parallel_ranges, 
	     Filtration_map fm = Filtration_map()){
	typedef typename Iterator_pairs::iterator Iterator;
	typedef typename Iterator_pairs::value_type Pair;
	typedef typename Pair::first_type Filtration_iterator;

	typedef typename tbb::blocked_range< Iterator > Range;
	typedef std::pair< double, double> Time_pair;
	typedef tbb::concurrent_vector< Time_pair>  Vector;
	typedef 	 Persistence_body< Boundary, Cascade_map, Vector> Body;
	Iterator parallel_end = ranges.begin()+num_parallel_ranges;
	Vector time_vector;
	time_vector.reserve( ranges.size());
	Body body( complex_boundary, cascade_prop_map, time_vector);
	Range range( ranges.begin(), parallel_end, 1);
	tbb::parallel_for( range, body);
	Time_pair result; 
	if(parallel_end != ranges.end()){
	        Filtration_iterator begin = parallel_end->first;
		Filtration_iterator end = ranges.rbegin()->second;
	        result = ctl::persistence( begin, end, 
				  complex_boundary, 
				  cascade_prop_map, false, fm);
	}
	double first_max = 0, second_max = 0;
	for( Vector::const_iterator i = time_vector.begin(); i != time_vector.end(); ++i){
		if (i->first > first_max){ first_max = i->first; }
		if (i->second > second_max){ second_max = i->second; }
	}
	result.first += first_max;
	result.second += second_max;
	return result; 
}

/*
 assumes we have a special filtration ordering: 
 S x 0 < U x 1 < V x 0 1
 for any S, U, V
*/
template< typename Iterator,
	  typename Iterator_pairs>
void get_cover_iterators( const Iterator & _begin, 
			  const Iterator & _end,
			  Iterator_pairs & it_pairs){
				
  typedef typename Iterator_pairs::value_type filtration_pair;
  Iterator begin = _begin;
  do{
        Iterator end = begin;
        end++;
        while( end != _end && 
	       ((*end)->first.first_cell() == (*begin)->first.first_cell())){
               end++;
        }
        #ifdef ITERATOR_BLOWUP_DEBUG
        std::cout << "(" << (*begin)->first.first_cell() << " , ";
        if (end != _end){
                std::cout << (*end)->first.first_cell() << "), ";
        }else{
                std::cout << " END)" << std::endl;
        }
        #endif
        it_pairs.push_back(filtration_pair(begin,end));
        begin = end;
  }while(begin != _end);
}

/* 
 * Works when we do not have a blowup
 */
template< typename Iterator,
	  typename Iterator_pairs>
void get_cover_iterators( const Iterator & _begin, 
			  const Iterator & _end,
			  Iterator_pairs & it_pairs, 
			  bool flag){
				
  typedef typename Iterator_pairs::value_type filtration_pair;
  Iterator begin = _begin;
  do{
        Iterator end = begin;
        end++;
        while( end != _end && 
	       ((*end)->second.data() == (*begin)->second.data() )){
               end++;
        }
        #ifdef ITERATOR_BLOWUP_DEBUG
        std::cout << "[" << (*begin)->second.data()->first << " , ";
        if (end != _end){
                std::cout << (*end)->second.data()->first << "), ";
        }else{
                std::cout << " END)" << std::endl;
        }
        #endif
        it_pairs.push_back(filtration_pair(begin,end));
        begin = end;
  }while(begin != _end);
}
} //namespace parallel
} //namespace CTL
#endif //CTL_PARALLEL_PERSISTENCE_H
