#ifndef _CTL_COVERS_H_
#define _CTL_COVERS_H_
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2010 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
//STL
#include <iterator>
#include <list>
#include <iostream>
#include <algorithm> //max & min_element
#include <string> 
#include <iomanip>
#include <map>

// CTL
#include <ctl/parallel/partition_covers/cover_stats.hpp>
#include <ctl/one_skeleton/one_skeleton.hpp> //one_skeleton 
#include <ctl/parallel/partition_covers/graph_partition.hpp> //metis
#include <ctl/cell_complex/complex_boundary.hpp> //Complex_boundary

//TIMER
#include <ctl/utility/timer.hpp>


//BOOST
#include <boost/lexical_cast.hpp>

//TBB
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

namespace ctl {
namespace parallel {

//Forward Declarations

// Id comparison and cover_cell ordering
template< typename CellIterator>
struct Cover_less : 
    public std::binary_function< const CellIterator&,
				 const CellIterator&,
				 bool> {
  bool operator()( const CellIterator& c1,
		   const CellIterator& c2) const 
  {

    const unsigned int nerve_id_1 = c1->second.data()->second.id();
    const unsigned int nerve_id_2 = c2->second.data()->second.id();

    //assumes the nerve's VERTICES have been built closed. 
    return nerve_id_1 < nerve_id_2 || 
	   (!(nerve_id_2 < nerve_id_1) && c1->second.id() < c2->second.id());
  }
}; // class Parallel_id_less

template< typename Cover_complex>
void init_cover_complex( Cover_complex & nerve, const size_t & num_parts){
	typedef typename Cover_complex::Cell Cell;
	typedef typename Cell::vertex_type vertex_t;
	//maintains a lexicographic order on vertex Id's for the nerve
	for (vertex_t i = 0; i < num_parts; ++i){
		Cell cell =  { i };
		nerve.insert_open_cell(cell);
	}
}

template< typename Complex, typename Cover_complex, typename Vertex_to_nerve>
bool compute_graph_partition( Complex & complex, 
			      Cover_complex & nerve,
			      Vertex_to_nerve & vertex_to_nerve)
{
	//major assumption here. this is not in general correct
	//but will be at least for all /blowup codes
	//num_parts needs to in general be the number of vertices in the nerve
	const size_t num_parts = nerve.size();
	typedef typename std::vector< idx_t> Vector;
	//typedef typename Complex::Cell::vertex_type Vertex;
        typedef typename Cover_complex::Cell Cover_cell;
        Vector neighbors; //num_vertices+1
        Vector offsets; //2*num_edges
        Vector index_to_vertex_map; //num_vertices
        Vector part( 1, 0); //num_vertices
        idx_t num_vertices = 0;
        idx_t edgecut = 0;

	//complex --> graph        
        ctl::one_skeleton( complex, neighbors, offsets, 
              	index_to_vertex_map);
        
        num_vertices = index_to_vertex_map.size();
        part.resize( num_vertices, 0);

 	ctl::parallel::metis( offsets, neighbors, part, edgecut, num_vertices, 
			      num_parts);

/*	//graph --> partition        
        ctl::metis( offsets, neighbors, part, options,
               edgecut, num_vertices, num_parts);
*/

	//partition --> complex        
        ctl::parallel::metis_to_complex( complex, nerve, index_to_vertex_map, 
              	    vertex_to_nerve, part);
        if(edgecut > 0){
	      Cover_cell cell( 1, nerve.size());
              nerve.insert_open_cell( cell);
              return true;
        }
        return false;
}

template< typename CellIterator>
struct Dimension_less :
    public std::binary_function< const CellIterator&,
                                 const CellIterator&,
                                 bool> {
  bool operator()( const CellIterator& c1,
                   const CellIterator& c2) const
  {
    // ask cells
    return (c1->first.dimension() < c2->first.dimension()) ||
           (!(c2->first.dimension() < c1->first.dimension()) &&
                (c1->first < c2->first));
  }
}; // class Dimension_less

template<typename Iterator>
bool Id_equal(const Iterator & a, const Iterator & b){
        return a->second.id() == b->second.id();
}

template< typename Map_iterator>
struct Is_one : 
  public std::unary_function< const Map_iterator&, bool> {
  bool operator()( const Map_iterator & a) const {return a.second == 1;}
}; 

template<typename Complex_iterator,
	 typename Cover_iterator,
	 typename Vert_to_nerve_map,
	 typename Cell_list>
void compute_open_cell_cover( Complex_iterator & cell,
			      Cover_iterator & last_cell,
			      Vert_to_nerve_map & vert_to_nerve,
			      Cell_list & cells_to_close){

        typedef typename Cover_iterator::value_type::first_type Cover_cell;
        typedef typename Cover_cell::vertex_type vertex_t;

        typedef typename Complex_iterator::value_type::first_type Complex_cell;
        typedef typename Complex_cell::const_iterator Cell_iterator;
	typedef typename Complex_cell::vertex_type Vertex;
        //commence terrible names.
        //TODO: this maybe can be a simple array O(p) in size.
        //Then removes branch around 207?
        typedef typename std::map< vertex_t, size_t > Nerve_map; 
        typedef typename Nerve_map::iterator Nerve_map_iterator;
        typedef typename Nerve_map::value_type Nerve_pair;
        typedef Is_one< Nerve_pair> Is_one; 
       	typedef typename Vert_to_nerve_map::iterator Vert_to_nerve_it;
 
        Nerve_map vertex_partitions;
	Vertex vertex = *(cell->first.begin());
        Cover_iterator & first = vert_to_nerve.find( vertex)->second;
        for(Cell_iterator i = cell->first.begin(); 
                             i != cell->first.end(); ++i){
		Vert_to_nerve_it v2n_it = vert_to_nerve.find( *i);
		Cover_iterator nerve_it = v2n_it->second;
                const vertex_t part_num = *(nerve_it->first.begin());

                Nerve_map_iterator lb = 
                                vertex_partitions.lower_bound( part_num);

                if(lb != vertex_partitions.end() && 
                   !(vertex_partitions.key_comp()( part_num, lb->first))){
                        lb->second++;
                }else{
                        vertex_partitions.insert( lb, Nerve_pair( part_num, 1));
                }
        }
        if(vertex_partitions.size() > 1){
                if (vertex_partitions.size() == 2){
			//if we get the larger of the two iterators here
			//we can actually separate the cell_list into disjoint
			//sub lists and the closure can be performed in
			//parallel.
                        size_t singletons = count_if( vertex_partitions.begin(),
                                                      vertex_partitions.end(),
                                                      Is_one());
                        if(singletons > 0){
                                cells_to_close.push_back( cell);
                        }
                }
                cell->second.data() = last_cell;
        }else{
                cell->second.data() = first;
        }
	cell->second.data()->second.pp();
}

template< typename Filtration_iterator,
	  typename Nerve_iterator,
	  typename Vert_to_nerve_map,
	  typename Cell_list>
class Open_cell_body{
	private:
	typedef Open_cell_body< Filtration_iterator, 
				Nerve_iterator, 
				Vert_to_nerve_map, 
				Cell_list> Self;
	typedef typename Cell_list::value_type Complex_iterator;
	public:
	Open_cell_body( Filtration_iterator & complex_begin,
			Nerve_iterator & last, 
			Vert_to_nerve_map & vert_to_nerve,
			Cell_list & cell_list ):
			_complex_begin( complex_begin),
			_last( last), 
			_vert_to_nerve( vert_to_nerve),
			_cell_list( cell_list){}

	Open_cell_body( const Self & _self): 
			_complex_begin( _self._complex_begin),
			_last( _self._last), 
                        _vert_to_nerve( _self._vert_to_nerve),
                        _cell_list( _self._cell_list){}

	template< typename Range>
	void operator()( const Range & r ) const{
		typedef typename Range::const_iterator Iterator;
		for(Iterator cur_cell = r.begin(); 
			     cur_cell != r.end(); 
			   ++cur_cell){
	    		compute_open_cell_cover( *cur_cell,
				    		 _last,
				     		 _vert_to_nerve, 
       		                    		 _cell_list);
			size_t new_pos = std::distance( _complex_begin, 
						        cur_cell);
			(*cur_cell)->second.id( new_pos);
		}
	}

	private:
	Filtration_iterator _complex_begin;
	Nerve_iterator _last;
	Vert_to_nerve_map & _vert_to_nerve;
	Cell_list & _cell_list;
};

template< typename Filtration_iterator, typename Vert_to_nerve_map>
class No_edgecut_body{
	private:
	typedef No_edgecut_body< Filtration_iterator, Vert_to_nerve_map> Self;
	typedef typename Vert_to_nerve_map::key_type Vertex;
	public:
	No_edgecut_body( Filtration_iterator  complex_begin, 
			 Vert_to_nerve_map & vert_to_nerve):
			_complex_begin( complex_begin),
			_vert_to_nerve( vert_to_nerve){}

	No_edgecut_body( const Self & _self): 
		_complex_begin( _self._complex_begin),
		_vert_to_nerve( _self._vert_to_nerve){}

	template< typename Range>
	void operator()( const Range & r ) const{
		typedef typename Range::const_iterator Iterator;
		Vertex vertex;
		for(Iterator cur_cell = r.begin(); 
			     cur_cell != r.end(); 
			   ++cur_cell){
			size_t new_pos = std::distance( _complex_begin, 
						       cur_cell);
			vertex = *((*cur_cell)->first.begin());
			(*cur_cell)->second.data() = 
				_vert_to_nerve.find( vertex)->second;
			(*cur_cell)->second.data()->second.pp(); 
			(*cur_cell)->second.id( new_pos);
		}
	}

	private:
	Filtration_iterator _complex_begin;
	Vert_to_nerve_map & _vert_to_nerve;
};

template< typename Filtration_iterator, typename Nerve_iterator>
class No_partition_body{
	private:
	typedef No_partition_body< Filtration_iterator, Nerve_iterator> Self;
	public:
	No_partition_body( Filtration_iterator & begin, 
			   Nerve_iterator & any): 
				_begin( begin), _any( any) {}
	No_partition_body( const Filtration_iterator begin, 
			   const Nerve_iterator any):
				_begin( begin),  _any( any) {}
	No_partition_body( const Self & _self):
				_begin( _self._begin),_any( _self._any) {}

	template< typename Range>
	void operator()( const Range & r ) const{
		typedef typename Range::const_iterator Iterator;
		for(Iterator cur_cell = r.begin(); 
			     cur_cell != r.end(); 
			   ++cur_cell){
			size_t new_pos = std::distance( _begin, 
						        cur_cell);
			(*cur_cell)->second.data() =  _any; 
			(*cur_cell)->second.id( new_pos);
			_any->second.pp();
		}
	}

	private:
	Filtration_iterator _begin;
	Nerve_iterator _any;
};
// These are generic methods given any
// partition it lifts to the open covering
template< typename Filtration, typename Cover_complex, 
	  typename Vertex_to_nerve_map, typename Cell_list >
void compute_open_cover( Filtration & filtration, 
			 Cover_complex & nerve, 
		         Vertex_to_nerve_map & vertex_to_nerve, 
			 Cell_list & nearly_pure_cells,
			 const bool edgecut = true){
	typedef typename Filtration::iterator Filtration_iterator;
	typedef typename Cover_complex::iterator Cover_complex_iterator;
	typedef typename Cover_complex::Cell Simplex;
 	typedef typename tbb::blocked_range< Filtration_iterator > Range;
	typedef  Open_cell_body< Filtration_iterator,
		   	      Cover_complex_iterator, 
		   	      Vertex_to_nerve_map, 
		   	      Cell_list > Open_cell_body;

	typedef  No_edgecut_body< Filtration_iterator, 
		   	       Vertex_to_nerve_map > 
		   	       No_edgecut_body;

	typedef  No_partition_body< Filtration_iterator, 
						 Cover_complex_iterator > 
					No_partition_body;

	Filtration_iterator _begin = filtration.begin(); 
	Range range( _begin, filtration.end(), 10000);
	if(edgecut){
		Simplex last_cell( 1, nerve.size()-1);
        	Cover_complex_iterator last = nerve.find_cell( last_cell);
		Open_cell_body body( _begin, 
				     last, 
				     vertex_to_nerve,
				     nearly_pure_cells);
		tbb::parallel_for( range, body);
	}else{
		if(nerve.size() == 1){
			//no partition map lookups necessary
			No_partition_body body( _begin, 
						nerve.begin()); 
			tbb::parallel_for( range, body);
		}else{
			//single partition map lookups necessary
			No_edgecut_body body( _begin,
					       vertex_to_nerve);
			tbb::parallel_for( range, body);
		}
	}
}

// And these methods will close the last set
template<typename Boundary, typename Complex_iterator, typename Cover_cell>
void recursive_close_cover_set( Boundary & boundary, 
		     	        Complex_iterator & cur_cell, 
		     		Cover_cell new_data, 
				Cover_cell impure_set){
	typedef typename Boundary::const_iterator Boundary_iterator;
	if(cur_cell->second.data() != new_data){
		cur_cell->second.data() = new_data;
		new_data->second.pp();
		impure_set->second.pp();
		for(Boundary_iterator i = boundary.begin( cur_cell);
				      i != boundary.end( cur_cell);
						++i){
			recursive_close_cover_set( boundary, i->cell(),
						   new_data, impure_set);
		}
	}
}

template< typename Boundary, 
	  typename Cell_list_iterator, 
	  typename Cover_complex>
void close_cover( Cell_list_iterator begin, 
		  Cell_list_iterator end,
		  Boundary & boundary, 
		  Cover_complex & nerve){

	typedef typename Boundary::const_iterator Boundary_iterator;
	typedef typename Cover_complex::Cell Cover_cell;
	//typedef typename Cover_complex::iterator Cover_iterator;
	typedef typename Cover_cell::vertex_type vertex_type;
	//typedef typename Cover_cell::iterator Cover_cell_iterator;
	typedef typename Cover_complex::Data Cover_cell_data;
                                                                  
	Cover_cell  impure_cell = (*begin)->second.data()->first;
	const vertex_type impure_index = *(impure_cell.begin());

	//with the right type of locking this loop could be made parallel..
	for(Cell_list_iterator cur_cell = begin; cur_cell != end; ++cur_cell){
	  	 for(Boundary_iterator i = boundary.begin( *cur_cell); 
	  	      		       i != boundary.end( *cur_cell);
	  	      		       ++i){
	  	      Cover_cell boundary_cell = i->cell()
							->second.data()->first;
	  	      const vertex_type first_vertex = *(boundary_cell.begin());
	  	      if( boundary_cell.dimension()== 0 && 
			  first_vertex != impure_index){
	  	      	//create the edge for the intersection
	  	      	Cover_cell edge = { impure_index, first_vertex };
			//this gives us a filtration ordering!
	  	      	nerve.insert_open_cell( edge, 
			Cover_cell_data( 1+ (impure_index+1) + first_vertex));
	  	      	recursive_close_cover_set( boundary, i->cell(), 
						   nerve.find_cell( edge), 
						  (*begin)->second.data());
	  	      }
	  	}
	}
}
// Plug in METIS
template< typename Filtration, 
	  typename Cover_complex, 
	  typename Cell_list>
bool graph_partition_open_cover( Filtration & filtration,
				 Cover_complex & nerve,
				 Cell_list & nearly_pure_cells = 
				     tbb::concurrent_vector< typename 
					Filtration::Complex::iterator>() ) {
	typedef typename Filtration::Complex Complex;
	typedef typename Complex::Cell::vertex_type Vertex;
	typedef typename Cover_complex::iterator Cover_complex_iterator;
	typedef typename std::map< Vertex, Cover_complex_iterator> 
							Vertex_to_nerve_map;
 
	//assumes a filtration with a non-const complex() accessor
	Complex & complex = filtration.complex();
	Vertex_to_nerve_map vertex_to_nerve;

	bool is_edgecut = compute_graph_partition( complex, 
						   nerve, 
						   vertex_to_nerve);
	compute_open_cover( filtration, 
			    nerve, 
			    vertex_to_nerve,
			    nearly_pure_cells, 
			    is_edgecut);
	return is_edgecut; 
}

template< typename Filtration, typename Cover_complex>
bool graph_partition_cover( Filtration & filtration, 
			    Cover_complex & nerve) {
	//assumes init_nerve( nerve, num_parts) called earlier
	typedef typename Filtration::Complex Complex;	
	typedef typename Complex::iterator Complex_iterator;
	typedef typename tbb::concurrent_vector< Complex_iterator> Cell_list;
	typedef typename ctl::Complex_boundary< Complex> Complex_boundary;	
	Cell_list nearly_pure_cells;
	//we find an open cover
	if( graph_partition_open_cover( filtration, 
					nerve, 
					nearly_pure_cells) ){
		// and if necessary
		// we close it.
		Complex_boundary boundary( filtration.complex());
		close_cover( nearly_pure_cells.begin(), 
			     nearly_pure_cells.end(), 
			     boundary, 
			     nerve );
		return true;
	}
	return false;
}
} //end namespace parallel
}  //end namespace ctl
#endif // _CTL_COVERS_H
