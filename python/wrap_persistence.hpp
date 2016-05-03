template< typename Complex, typename Cell_less, bool Remove_destroyers>
std::pair< std::vector<std::size_t>, 
	    std::vector<  std::vector< ctl::Term< typename Complex::Cell, ctl::Finite_field< 2> > > >
	  >
run_persistence( Complex & complex, Cell_less & less, bool compute_cascade = false){
   typedef ctl::Graded_cell_complex< Complex, Cell_less > Filtration;
   //Boundary Operator
   //NOTE: This is not a general purpose boundary operator.
   //It works correctly only when successive 
   //boundaries are taken in a filtration order
   //typedef typename Filtration::iterator Filtration_iterator;
   typedef ctl::Graded_boundary< Filtration> Filtration_boundary;
   typedef typename Filtration::Term Filtration_term;
   typedef typename Filtration_term::Coefficient Coeff;
   typedef typename ctl::Sparse_matrix< Coeff> Sparse_matrix;
   typedef typename ctl::Offset_map< typename Filtration::iterator> Offset_map;
   typedef typename ctl::Sparse_matrix_map< Coeff, Offset_map> Chain_map;
   
   //produce a filtration
   Filtration complex_filtration( complex);
   Filtration_boundary filtration_boundary( complex_filtration);

   //begin instantiate our vector of cascades homology

   //we hand persistence a property map for genericity!                        
   Offset_map offset_map( complex_filtration.begin());
   Sparse_matrix R( complex.size());
   Chain_map R_map( R.begin(), offset_map);
   if( compute_cascade){ 
   Sparse_matrix D( complex.size());
   Chain_map D_map( D.begin(), offset_map);
   ctl::persistence<Remove_destroyers>( complex_filtration.begin(), complex_filtration.end(),
                            filtration_boundary,
                            R_map, D_map, false, offset_map);
   typedef std::vector< ctl::Term< typename Complex::Cell, Coeff>> Chain;
   typedef std::vector< Chain> Chains;
   Chains generators;
   for(auto s = complex_filtration.begin(); s !=  complex_filtration.end(); ++s){
   	if( R_map[s].empty()){
		Chain generator;
		generator.reserve( D_map[s].size());
		for( auto & elt : D_map[s]){
			generator.emplace_back(complex_filtration.begin()[ elt.cell()]->first, elt.coefficient());
		}
		generators.emplace_back( generator);
	}
   }
   std::vector< std::size_t> bti;
   compute_betti( complex_filtration, R_map, bti, true);
   return std::make_pair( bti, generators);

   } else {
	ctl::persistence<Remove_destroyers>( complex_filtration.begin(), complex_filtration.end(),
                            		     filtration_boundary,
   					     R_map,  false, offset_map);
   	std::vector< std::size_t> bti;
   	compute_betti( complex_filtration, R_map, bti, true);
   	return std::make_pair(bti, std::vector<  std::vector< ctl::Term< typename Complex::Cell, Coeff> > >());
   }
}

template< typename Complex>
decltype(auto) homology_two_impl( Complex& complex, bool compute_cascade=false, bool remove_destroyers=true){
  typedef ctl::Cell_less Complex_cell_less;
  Complex_cell_less less;
  if( remove_destroyers){
      return run_persistence<Complex, Complex_cell_less, true>( complex, less, compute_cascade);
  }
  return run_persistence<Complex, Complex_cell_less, false>( complex, less, compute_cascade);
}

template< typename Complex>
decltype(auto) homology_one_impl( Complex& complex, bool compute_cascade=false) 
{ return homology_two_impl( complex, compute_cascade, true); }


template< typename Complex>
decltype(auto) 
homology_impl( Complex& complex) 
{ return homology_two_impl( complex, false, true).first; }


decltype(auto) 
homology_sc( ctl::Simplicial_complex<>& complex)
{ return homology_impl( complex); }

decltype(auto) 
homology_one_sc( ctl::Simplicial_complex<>& complex, bool cc)
{ return homology_one_impl( complex, cc); }


decltype(auto) 
homology_two_sc( ctl::Simplicial_complex<>& complex, bool cc, bool rd)
{ return homology_two_impl( complex, cc, rd); }

decltype(auto) 
homology_pd( ctl::Prod_simplicial_complex& complex)
{ return homology_impl( complex); }

decltype(auto) 
homology_one_pd( ctl::Prod_simplicial_complex& complex, bool cc)
{ return homology_one_impl( complex, cc); }


decltype(auto) 
homology_two_pd( ctl::Prod_simplicial_complex& complex, bool cc, bool rd)
{ return homology_two_impl( complex, cc, rd); }


// Creates a Python class for an `Abstract_simplex`. 
void wrap_persistence(py::module &mod){
  mod.def("homology", &homology_two_sc, "compute the homology of the list of simplices");
  mod.def("homology", &homology_one_sc, "compute the homology of the list of simplices");
  mod.def("homology", &homology_sc, "compute the homology of the list of simplices");

  mod.def("homology", &homology_two_pd, "compute the homology of the list of products of simplices");
  mod.def("homology", &homology_one_pd, "compute the homology of the list of products of simplices");
  mod.def("homology", &homology_pd, "compute the homology of the list of products of simplices");
}
