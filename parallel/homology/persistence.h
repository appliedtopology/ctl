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
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

namespace ctl {
namespace parallel{
template<typename Timer>
struct Parallel_stats{
	Timer timer;
	double parallel_persistence;
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
         typename Cascade_map>
class Persistence_body{
        private:
        typedef Persistence_body< Boundary,
                                  Cascade_map> Self;
        public:
        Persistence_body( Boundary & boundary,
                          Cascade_map & cascade_prop_map ):
                        _boundary( boundary),
                        _cascade_prop_map( cascade_prop_map){}

        Persistence_body( const Self & _self):
                        _boundary( _self._boundary),
                        _cascade_prop_map( _self._cascade_prop_map){}

        template< typename Range>
        void operator()( const Range & r ) const{
                typedef typename Range::const_iterator Iterator;
                for(Iterator current = r.begin();
                             current != r.end();
                           ++current){
		
		ctl::persistence( current->first,
				  current->second,
				  _boundary,
		 		  _cascade_prop_map);
		}
        }

        private:
        Boundary & _boundary;
        Cascade_map & _cascade_prop_map;
}; //Persistence_body

template< typename Iterator_pairs, 
	  typename Boundary, 
	  typename Cascade_map>
void parallel_persistence(Iterator_pairs & ranges, 
			  Boundary & complex_boundary,
			  Cascade_map & cascade_prop_map,
			  size_t & num_parallel_ranges){
	typedef typename Iterator_pairs::iterator Iterator;
	typedef typename Iterator_pairs::value_type Pair;
	typedef typename Pair::first_type Filtration_iterator;

	typedef typename tbb::blocked_range< Iterator > Range;
	typedef typename ctl::Persistence_body< Boundary, Cascade_map> Body;
	ctl::Thread_timer timer;

	Iterator parallel_end = ranges.begin()+num_parallel_ranges;
	Body body( complex_boundary, cascade_prop_map);
	Range range( ranges.begin(), parallel_end, 1);
	tbb::parallel_for( range, body);
	std::cout << "parallel for complete" << std::endl;
	if(parallel_end != ranges.end()){
	        Filtration_iterator begin = parallel_end->first;
		Filtration_iterator end = ranges.rbegin()->second;
	        ctl::persistence( begin, end, 
				  complex_boundary, 
				  cascade_prop_map);
	}
	//double serial_time = timer.get();
	
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
