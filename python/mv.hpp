#define DEBUG_PERSISTENCE

#include <ctl/ctl.hpp>
#include <sstream>      // std::stringstream
#include <algorithm>

template< typename Iterator, typename Bd, typename SS_Term, typename M>
decltype(auto)
compute_homology_at_term_ijr( Iterator F_begin, Bd& unused, 
			     SS_Term& er_term, SS_Term& er_target, M& R_map, M& D_map){ 
   typedef ctl::Finite_field< 2> Coeff;
   typedef std::vector< std::size_t> WTF;
   typedef typename ctl::Offset_map< typename WTF::iterator> Offset_map;
   typedef ctl::Sparse_matrix< Coeff> Matrix;
   typedef typename Matrix::value_type Chain;
   typedef typename Chain::Term Term;
   typedef typename ctl::Sparse_matrix_map< Coeff, Offset_map> Chain_map;
    	
 
   WTF v( er_target.size()+er_term.size());
   std::iota(v.begin(), v.end(), 0); 
   Matrix quotient_R( v.size());
   Matrix quotient_D( v.size());
   Offset_map offset_map( v.begin());
   Chain_map QR_map( quotient_R.begin(), offset_map);
   Chain_map QD_map( quotient_D.begin(), offset_map);
   
   //Multiply on the right by an identity matrix
   for( auto i = 0; i != v.size(); ++i){
   	 Chain& d = QD_map[ i];
	 d += Term( i, 1);
   }
 
   for( auto i = er_target.size(); i != v.size(); ++i){
	 std::size_t idx = v[i];
	 std::size_t original_idx = er_term[i-er_target.size()];
	 //std::cout << "Working on the boundary of: " << F_begin[original_idx]->first << std::endl;
	 
	 Chain& c = QR_map[ idx];
	 for(auto& nz : R_map[ original_idx]){
		//std::cout << nz << " <--> "; 
	 	auto row_pos = std::find( std::begin(er_target), std::end(er_target), nz.cell());
		if( row_pos != std::end(er_target) ){
			Term t( std::distance( std::begin(er_target), row_pos), nz.coefficient());
			//std::cout << "Created a Term with value: " << t << std::endl;
			c += t;
		}else {
			//std::cout << "NOTHING." << std::endl;
		}
	 }
   }
   //std::cout << "Just before persistence: " << er_term.size() <<  std::endl;
   
   ctl::persistence<false>( v.begin(), v.end(),
                            unused, QR_map, QD_map, true, offset_map);
   //std::cout << "Just after persistence." << std::endl;
   typename std::remove_const< SS_Term>::type ker, im;
   for( auto i = 0; i != er_target.size(); ++i){
	   auto act_idx = er_target[ i];
	   //std::cout << "idx " << v[ i] << " --> " << act_idx << std::endl;;
	   if( !QR_map[ i].empty()){
		im.push_back( act_idx);
	   }
   }
   for( auto i = er_target.size(); i !=v.size(); ++i){
	   std::size_t idx = v[ i];
	   std::size_t er_term_index = i-er_target.size();
	   auto act_idx = er_term[ er_term_index];
	   //std::cout << "Working on local col idx " << idx << " --> Global column index" << act_idx << std::endl;;
	   //std::cout << "Cell: " << F_begin[ act_idx]->first << std::endl;
	   //std::cout << "Offset Map: " << offset_map[ idx] << std::endl;
	   //std::cout << "We added these columns together: " << QD_map[ idx] << std::endl;
	   if (QR_map[ idx].empty()){ ker.push_back( act_idx); }
	   for(auto& local_combinator: QD_map[ idx]){
		//When you multiply guass transforms you need to end up with an identity element.
		auto act_col_idx = er_term[local_combinator.cell()-er_target.size()];
		if( act_col_idx == act_idx) { continue ; }
		//std::cout << local_combinator.cell() << " --> " << act_col_idx << std::endl;
	   	D_map[ act_idx] += Term( act_col_idx, local_combinator.coefficient());
	   	R_map[ act_idx].scaled_add( local_combinator.coefficient(), R_map[ act_col_idx]);
	   }

	   //if (R_map[ act_idx].empty()) { 
	   //std::cout << ctl::delta << "(";
	   //for( auto & elt: D_map[ act_idx]){ std::cout << F_begin[ elt.cell()]->first  << " "; }
	   //std::cout << ") = 0";
	   //std::cout << std::endl;
	   //}
	   //for( auto & elt: R_map[ act_idx]){ std::cout << F_begin[ elt.cell()]->first  << " "; }
   }
   return std::make_pair( ker, im);
}

template< typename Page, typename Map, typename Cell_map>
void print_page_with_map( Page& page, std::size_t r , Map& D_map, Map& R_map, std::size_t nerve_dimension, std::size_t complex_dimension, Cell_map M){
for( std::size_t j = 0; j < nerve_dimension+1; ++j){
		for( std::size_t i = 0; i < complex_dimension+1; ++i){
	auto& indices = page[ std::make_pair(i,j)];
	std::cout << "dim E^"<< r << "_{" << i << "," << j << "} = " << indices.size() << std::endl;
	if( indices.size() == 0){ 
		continue; 
	} //If the source is empty, bail now.
	auto im_tgt = std::make_pair(i+r-1, j-r);
	if( i+r >= 1 and j >= r and page.count(im_tgt) > 0 and page[im_tgt].size() > 0){ //If there is an image, then there should is a nonzero map to print
		std::cout << ctl::delta << "^" << r << " :  E^"<< r << "_{" << i << "," << j << "} --> E^"<< r << "_{" << im_tgt.first << "," << im_tgt.second << "} "<< std::endl;
		auto& Im = page[im_tgt];
		for(auto & index: indices){
			std::cout << "\t " << ctl::delta << "(";
			for(auto& elt: D_map[ index]){
				std::cout << M[ elt.cell()]->first << " ";				
			}
			std::cout << ") = ";
			for(auto & elt: R_map[ index]){
				if( std::find( std::begin(Im), std::end(Im), elt.cell()) != std::end(Im)){
					std::cout << M[ elt.cell()]->first << " ";
				}
			}
			if( R_map[ index].empty()){ std::cout << "0"; }
			std::cout << std::endl;
		}
	} else { //Otherwise print page
		std::cout << "E^" << r << "_{" << i << "," << j << "} : ";
		for(auto & index: indices){
			std::cout << "\t";
			for(auto& elt: D_map[ index]){
				std::cout << M[ elt.cell()]->first << " ";				
			}
			std::cout << std::endl;
		}
		std::cout << "But Differential is zero! " << std::endl;
	}
  }
}
}



std::string 
mv_demo( ctl::Prod_simplicial_complex& K){
	typedef ctl::Finite_field< 2> Coeff;
	std::stringstream out;
	std::size_t complex_dimension = 0;
	std::size_t nerve_dimension = 0;
	typedef ctl::Graded_cell_complex< ctl::Prod_simplicial_complex> Graded_complex;
        Graded_complex	F( K);
	typedef ctl::Graded_boundary< Graded_complex> Graded_boundary;
	Graded_boundary G_bd( F);
	
	typedef std::vector< std::map< std::pair< std::size_t, std::size_t>, std::vector< std::size_t> > > Double_complex;
	typedef ctl::Sparse_matrix< Coeff> Matrix;
	typedef typename Matrix::value_type Chain;
	typedef typename Double_complex::value_type Page;
	typedef typename Page::mapped_type SS_Term;

	typedef typename ctl::Offset_map< typename Graded_complex::iterator> Offset_map;
	//typedef ctl::Sparse_matrix_map< Coeff, Offset_map> Map;
   	typedef typename ctl::Sparse_matrix_map< Coeff, Offset_map> Chain_map;
 
        //we hand persistence a property map for genericity!                        
        Offset_map offset_map( F.begin());
        Matrix R( K.size());
        Chain_map R_map( R.begin(), offset_map);
    	
	Matrix D( K.size());
   	Chain_map D_map( D.begin(), offset_map);
 	
	Double_complex E;
	Page E0;
	Matrix bd_matrix( K.size());
	for(auto itr = F.begin(); itr != F.end(); ++itr){
		const auto& fc = (*itr)->first.first_cell();
		const auto& sc = (*itr)->first.second_cell();
		complex_dimension = std::max( complex_dimension, fc.dimension());
		nerve_dimension = std::max( nerve_dimension,   sc.dimension());
		E0[ std::make_pair(fc.dimension(), sc.dimension())].emplace_back( 
								    std::distance(F.begin(), itr) );
		for (auto f = G_bd.begin( itr); f != G_bd.end( itr); ++f){
			R_map[ itr] += *f;
		}
		D_map[itr] += typename Chain::Term( std::distance(F.begin(), itr), 1);
	}
	print_page_with_map( E0, 0, D_map, R_map, nerve_dimension, complex_dimension, F.begin());
	E.emplace_back( E0);
	for(std::size_t r = 0; r < 3; ++r){
	 std::cout << "Computing differential r = " << r << std::endl;
	 Page& E_r = E[r];
	 Page E_next;
	  for( std::size_t j = 0; j < nerve_dimension+1; ++j){
		for( std::size_t i = 0; i < complex_dimension+1; ++i){
				//std::cout << "At Term: " << i << " " << j << " on page: " << r << std::endl;
				auto src = std::make_pair(i,j);
				if( E_r.count( src)==0 or i+r < 1 or j < r) {
					//Our map is the zero map, because either the source vector space is zero, or the target space is defined to be zero vector space.
					//Kernel is whatever it is where we are at. keep going
			        	E_next.emplace( src, E_r[src]);
					continue;
				}
			       //valid access by assumption	
				const SS_Term& er_term = E_r[ src];
				//auto im_tgt = std::make_pair(j-r, i+r-1);
				//construct the target of the map indices, which is in a place where data was assigned nonzero at some point by assumption
				auto im_tgt = std::make_pair(i+r-1, j-r);
				//std::cout << "Computing map to E_" << im_tgt.first << "," << im_tgt.second << " on page " <<  r << std::endl;
				//if we have the image, and if its nonzero we compute homology, otherwise we know we have a zero map
				if( E_r.count( im_tgt) != 0 && E_r[ im_tgt].size() > 0){ 
					const SS_Term& er_im_target = E_r[ im_tgt]; 
					auto& ker = E_next[ im_tgt];
					std::pair< SS_Term, SS_Term> ker_and_im_indices=
						compute_homology_at_term_ijr( F.begin(),
									      G_bd,
									      er_term,
									      er_im_target, 
									      R_map, D_map);
					E_next.emplace( src, ker_and_im_indices.first);
					for( auto& im_index : ker_and_im_indices.second){
						std::remove( std::begin(ker), std::end(ker), im_index);
					}
					ker.resize( ker.size() - ker_and_im_indices.second.size());
					//std::cout << "Computed term: E_" << i << "," << j << " @ " <<  r << std::endl;
				} else { //Another example of a zero map!
					E_next.emplace( src, E_r[src]);
				}
			}
		}
	    print_page_with_map( E_next, r+1, D_map, R_map, nerve_dimension, complex_dimension, F.begin());
	    E.emplace_back(E_next);
	}
	std::cout << "Returning.." << std::endl;
	//TODO: compute persistence here.
	return out.str();
}

decltype(auto)
sc_open_star_cover( ctl::Simplicial_complex<>& sc, 
		    std::function< std::size_t(std::size_t)>& f){
	return ctl::open_star_cover( sc, f);
}

decltype(auto)
make_blowup_impl( ctl::Simplicial_complex<>& K,  std::vector< ctl::Abstract_simplex>& sc){
	typedef ctl::Prod_simplicial_complex Blowup;
	typedef typename Blowup::Cell Product_cell;
	Blowup blowup;
	std::size_t index = 0;
	//std::cout << K.size() << " " << sc.size() << std::endl; 
	for( const auto & sigma : K){
		const auto& tau = sc[index];
		//std::cout << "sigma: " << sigma.first << " " << "tau: " << tau << std::endl;
		Product_cell pd( sigma.first, tau);
		blowup.insert_closed_cell( pd);
	        index++;	
	}
	return blowup;
}

decltype(auto)
make_blowup( ctl::Simplicial_complex<>& K,  std::list< ctl::Abstract_simplex>& sc){
	std::vector< ctl::Abstract_simplex> wtf( std::begin(sc), std::end(sc));
	return make_blowup_impl(K, wtf);
}
