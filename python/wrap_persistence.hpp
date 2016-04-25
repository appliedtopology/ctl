template< typename Complex, typename Cell_less, bool Remove_destroyers>
std::vector<std::size_t> 
run_persistence( Complex & complex, Cell_less & less, bool compute_cascade = false){
   typedef ctl::Graded_cell_complex< Complex, Cell_less > Filtration;
   //Boundary Operator
   //NOTE: This is not a general purpose boundary operator.
   //It works correctly only when successive 
   //boundaries are taken in a filtration order
   //typedef typename Filtration::iterator Filtration_iterator;
   typedef ctl::Graded_boundary< Filtration> Filtration_boundary;
   typedef typename Filtration::Term Filtration_term;
   typedef typename Filtration_term::Coefficient Coefficient;

   typedef typename ctl::Sparse_matrix< Coefficient> Sparse_matrix;
   typedef typename ctl::Offset_map< typename Filtration::iterator> Offset_map;
   typedef typename ctl::Sparse_matrix_map< Coefficient, Offset_map> Chain_map;
   
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
   } else {
	ctl::persistence<Remove_destroyers>( complex_filtration.begin(), complex_filtration.end(),
                            		     filtration_boundary,
                            	    	     R_map,  false, offset_map);
   }
   std::vector< std::size_t> bti;
   compute_betti( complex_filtration, R_map, bti, true);
   return bti;
}

 


std::vector< std::size_t> 
homology_two( ctl::Simplicial_complex<>& complex, bool compute_cascade=false, bool remove_destroyers=true){
  typedef ctl::Simplicial_complex<> Complex;
  typedef ctl::Cell_less Complex_cell_less;
  Complex_cell_less less;
  if( remove_destroyers){
      return run_persistence<Complex, Complex_cell_less, true>( complex, less, compute_cascade);
  }
  return run_persistence<Complex, Complex_cell_less, false>( complex, less, compute_cascade);
}

std::vector< std::size_t> 
homology_one( ctl::Simplicial_complex<>& complex, bool compute_cascade=false){
	return homology_two( complex, compute_cascade, true);
}

std::vector< std::size_t> 
homology( ctl::Simplicial_complex<>& complex){
	return homology_two( complex, false, true);
}





// Creates a Python class for an `Abstract_simplex`. 
void wrap_persistence(py::module &mod){
  mod.def("homology", &homology_two, "compute the homology of the list of simplices");
  mod.def("homology", &homology_one, "compute the homology of the list of simplices");
  mod.def("homology", &homology, "compute the homology of the list of simplices");
}
