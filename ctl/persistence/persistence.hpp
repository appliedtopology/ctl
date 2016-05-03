#ifndef CTLIB_PERSISTENCE_H
#define CTLIB_PERSISTENCE_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/
#include <ctl/persistence/persistence_data.hpp>
#include <ctl/io/io.hpp>
#include <ctl/parallel/utility/timer.hpp>
#include <ctl/finite_field/finite_field.hpp>
//exported functionality
namespace ctl{
namespace detail{
/**
* @brief tag for persistence output tag dispatching
*/
class partner {};
/**
* @brief tag for persistence output tag dispatching
*/
class partner_and_cascade {};

/**
* @brief tag for persistence input tag dispatching
*/
class empty_bd {};
/**
* @brief tag for persistence input tag dispatching
*/
class bd_init {};


/**
* @brief Nothing to do, no cascades stored
*
* @tparam Persistence_data
* @tparam Term
* @tparam Scalar
* @param data
* @param tau
* @param scalar
* @param partner
*/
template< typename Persistence_data, typename Term, typename Scalar>
void update_cascade( Persistence_data& data, const Term & tau, 
		     Scalar & scalar, detail::partner){}

/**
* @brief Updates the cascade
*
* @tparam Persistence_data
* @tparam Term
* @tparam Scalar
* @param data
* @param tau
* @param scalar
* @param partner_and_cascade
*/
template< typename Persistence_data, typename Term, typename Scalar>
void update_cascade( Persistence_data& data, const Term & tau, 
		     Scalar & scalar, detail::partner_and_cascade){
   typedef typename Persistence_data::Chain Chain;
   Chain& tau_cascade = data.cascade_map[ tau];
   data.cascade.scaled_add( scalar,  tau_cascade, data.temporary_chain);
}

/**
* @brief Inner loop for persistence algorithm
*
* @tparam Persistence_data
* @param data
*/
template< typename Persistence_data>
void eliminate_boundaries( Persistence_data & data){ 
   //Cell here is really a pointer into the complex
   typedef typename Persistence_data::Chain Chain;
   typedef typename Chain::value_type Term;
   //typedef typename Term::Cell Cell;
   typedef typename Term::Coefficient Coefficient;
   while( !data.cascade_boundary.empty()){
	//instead of directly accessing tau, we just get tau's position
	const Term& tau_term = data.cascade_boundary.youngest();
	const Chain& bd_cascade_tau = data.cascade_boundary_map[ tau_term];
	#ifdef DEBUG_PERSISTENCE
	std::cerr << tau_term.cell() << " is youngest" << std::endl;
	#endif
	//tau is the partner
	if( bd_cascade_tau.empty()){ return; }
	//otherwise tau has a partner
	const Term& tau_partner_term = bd_cascade_tau.youngest();
   	const Coefficient& scalar = ctl::inverse( tau_partner_term.coefficient());
	const Chain& bd_cascade_tau_partner = 
				data.cascade_boundary_map[ tau_partner_term];
	#ifdef DEBUG_PERSISTENCE
	std::cerr << "adding: " << bd_cascade_tau_partner << std::endl;
	#endif 
  	data.cascade_boundary.scaled_add( scalar, bd_cascade_tau_partner,
				          data, data.temporary_chain);
	#ifdef DEBUG_PERSISTENCE
 	std::cerr << ctl::delta << "(cascade["<< ctl::sigma << "]) = " 
		  << data.cascade_boundary << std::endl;  
	#endif
	update_cascade( data, tau_partner_term, scalar, data.policy); 
  }
}

/**
* @brief Returns true if cell is a creator (positive), 
	 false if destroyer (negative)
* @tparam Term
* @tparam Chain_map
* @param term
* @param cascade_boundary_map
*
* @return 
*/
template< typename Term, typename Chain_map>
inline bool is_creator( const Term & term, 
			const Chain_map & cascade_boundary_map){
	typedef typename boost::property_traits< Chain_map>::value_type Chain;
	typedef typename Chain::Less Term_less;
	const Chain& bd = cascade_boundary_map[ term];
	Term_less term_less;
	return  bd.empty() || term_less( term, bd.youngest());
}

/**
* @brief See is_creator
*
* @tparam Chain_map
*/
template< typename Chain_map>
struct Is_not_creator{
	typedef typename boost::property_traits< Chain_map>::value_type Chain;
	typedef typename Chain::Term Term;
	typedef typename Chain::Less Term_less;
	Is_not_creator( Chain_map & _m): chain_map( _m) {}

	inline bool operator()( Term & term) const { 
		const Chain& bd = chain_map[ term];
		return  !bd.empty() && !term_less( term, bd.youngest());
	}
	const Chain_map & chain_map;
	Term_less term_less;
}; //struct Is_not_creator

/**
* @brief Initializes cascade data  
*
* @tparam Remove_destroyers
* @tparam Filtration_iterator
* @tparam Persistence_data
* @param sigma
* @param data
* @param remove_destroyers
* @param bd_init
* @param partner
*/
template< bool Remove_destroyers,
	  typename Filtration_iterator, 
	  typename Persistence_data>
void initialize_cascade_data( const Filtration_iterator sigma,
			      Persistence_data & data, bd_init, detail::partner){
 //typedef typename Filtration_iterator::value_type Cell;
 typedef typename Persistence_data::Chain Chain;
 typedef typename Persistence_data::Cell_chain_map Cell_chain_map;
 if( Remove_destroyers){
 typedef Is_not_creator< Cell_chain_map> Remover;
 //typedef typename Chain::value_type Term;
 Remover is_not_creator( data.cascade_boundary_map);
 Chain& cascade_boundary = data.cascade_boundary;
 //here we are using the fact that rbegin() is really the begin of the 
 //underlying vector so [i, rend()) is really [i, end())
 auto i=std::remove_if( cascade_boundary.rbegin(), cascade_boundary.rend(), 
    	     is_not_creator);
 cascade_boundary.erase( i, cascade_boundary.rend());
 }
}

/**
* @brief Initializes cascade data
*
* @tparam Remove_destroyers
* @tparam Filtration_iterator
* @tparam Persistence_data
* @param sigma
* @param data
* @param empty_bd
* @param partner
*/
template< bool Remove_destroyers,
          typename Filtration_iterator, 
	  typename Persistence_data>
void initialize_cascade_data( const Filtration_iterator sigma, 
			      Persistence_data & data, empty_bd, detail::partner){
     //typedef typename Filtration_iterator::value_type Cell;
     typedef typename Persistence_data::Chain Chain;
     //typedef typename Chain::value_type Term;
     Chain& cascade_boundary = data.cascade_boundary;
     cascade_boundary.reserve( data.bd.length( sigma));
     for( auto i = data.bd.begin( sigma); i != data.bd.end( sigma); ++i){
         if( !Remove_destroyers || is_creator( *i, data.cascade_boundary_map)){
		cascade_boundary += *i; //insertion sort 
	}
     }
      
}


/**
* @brief Initializes cascade data
*
* @tparam Remove_destroyers
* @tparam Filtration_iterator
* @tparam Persistence_data
* @param sigma
* @param data
* @param bd_init
* @param partner_and_cascade
*/
template< bool Remove_destroyers,
	  typename Filtration_iterator, 
	  typename Persistence_data>
void initialize_cascade_data( const Filtration_iterator sigma, 
			      Persistence_data & data,
			      bd_init, detail::partner_and_cascade){ 
  //typedef typename Filtration_iterator::value_type Cell;
  typedef typename Persistence_data::Chain Chain;
  typedef typename Persistence_data::Cell_chain_map Cell_chain_map;
  typedef Is_not_creator< Cell_chain_map> Remover;
  //typedef typename Chain::value_type Term;
  //Swap in the data if it matters.
  data.cascade.swap( data.cascade_map[ sigma]);
  if( Remove_destroyers){
    Remover is_not_creator( data.cascade_map);
    Chain& cascade = data.cascade;
    auto i=std::remove_if( cascade.rbegin(), cascade.rend(), is_not_creator);
    cascade.erase( i, cascade.rend());
  }
  initialize_cascade_data< Remove_destroyers, Filtration_iterator, Persistence_data>( sigma, data, bd_init(), detail::partner());

}




/**
* @brief Initializes cascade data
*
* @tparam Remove_destroyers
* @tparam Filtration_iterator
* @tparam Persistence_data
* @param cell
* @param data
* @param empty_bd
* @param partner_and_cascade
*/
template< bool Remove_destroyers, 
	  typename Filtration_iterator, 
	  typename Persistence_data>
void initialize_cascade_data( const Filtration_iterator & cell, 
			      Persistence_data & data, empty_bd, 
			      detail::partner_and_cascade){
	typedef typename Persistence_data::Chain::Term Term;
	data.cascade.add( Term( cell, 1)); 
	initialize_cascade_data<Remove_destroyers, Filtration_iterator, Persistence_data>(cell, data, empty_bd(), detail::partner());
}

/**
* @brief Stores the cascade nothing to store when we don't store cascades.
*
* @tparam Persistence_data
* @tparam Filtration_iterator
* @param data
* @param cell
* @param partner
*/
template< typename Persistence_data, typename Filtration_iterator>
void store_cascade( const Persistence_data & data, 
		    const Filtration_iterator & cell, detail::partner){}	

/**
* @brief Stores the cascade nothing to store when we don't store cascades.
*
* @tparam Persistence_data
* @tparam Filtration_iterator
* @param data
* @param sigma
* @param partner
*/
template< typename Persistence_data, typename Filtration_iterator>
void store_scaled_cascade( Persistence_data& data, 
			   const Filtration_iterator sigma, detail::partner p){}	


/**
* @brief Stores the cascade
*
* @tparam Persistence_data
* @tparam Filtration_iterator
* @param data
* @param sigma
* @param partner_and_cascade
*/
template< typename Persistence_data, typename Filtration_iterator>
void store_cascade( Persistence_data & data, 
		    const Filtration_iterator sigma, detail::partner_and_cascade pc){
	data.cascade_map[ sigma].swap( data.cascade);
}

/**
* @brief Stores the cascade
*
* @tparam Persistence_data
* @tparam Filtration_iterator
* @param data
* @param sigma
* @param partner_and_cascade
*/
template< typename Persistence_data, typename Filtration_iterator>
void store_scaled_cascade( Persistence_data & data, 
			   const Filtration_iterator sigma, 
		           detail::partner_and_cascade pc){
	const auto scalar = data.cascade_boundary.normalize();
	data.cascade *= scalar;
	data.cascade_map[ sigma].swap( data.cascade);
}

/**
* @brief Runs the persistence algorithm ala afras book chapter
* The function has many bells and whistles.
* In particular, one can change both the Input and Output Policies
* and turn on/off removal of negative rows all at compile time
* A Filtration_map[] is a level of indirection which associates
* a filtration iterator to a key_type for a Cell_to_chain_map.
* Regularly this map is just an identity.
* @param begin
* @param end
* @param bd
* @param cascade_boundary_map
* @param cascade_map
* @param fm
* @param input_policy
* @param output_policy
*/
template< bool Remove_destroyers = true,
	  typename Filtration_iterator,
	  typename Boundary_operator,
	  typename Chain_map,
	  typename Input_policy, 
	  typename Output_policy, 
	  typename Filtration_map>
std::pair< double, double> 
pair_cells( Filtration_iterator begin, Filtration_iterator end,
            Boundary_operator & bd, 
            Chain_map & cascade_boundary_map, 
	    Chain_map & cascade_map,
	    Filtration_map & fm,
	    Input_policy input_policy,
	    Output_policy output_policy){ 
	typedef typename boost::property_traits< Chain_map>::value_type Chain;
	//this should now operator on filtration pointers, so it should be fast.
	typedef typename Chain::Less Term_less;
	typedef ctl::detail::Persistence_data< Term_less, Boundary_operator, 
				  		Chain_map, Output_policy> 
					            Persistence_data;
	//typedef typename Filtration_iterator::value_type Cell_iterator; 
	typedef typename Chain::Term Term;
	double init_cascade_time = 0.0, persistence_algorithm=0.0;
	ctl::parallel::Timer timer;
	Term_less term_less; 
	Persistence_data data( term_less, bd, cascade_boundary_map, cascade_map,
			       output_policy);
	for(Filtration_iterator sigma = begin; sigma != end; ++sigma){
	   #ifdef DEBUG_PERSISTENCE
	   std::cerr << "Processing: " << (*sigma)->first << std::endl;
	   #endif 
	   timer.start();
	   //hand the column we want to operate on to our temporary.
	   cascade_boundary_map[ sigma ].swap( data.cascade_boundary);
	   initialize_cascade_data< Remove_destroyers >( fm[sigma], data, 
						   input_policy, output_policy);
	   #ifdef DEBUG_PERSISTENCE
	   std::cerr << ctl::delta << "(cascade(" << cascade_map[sigma] << ")"
	   	  << " = " << data.cascade_boundary << std::endl;
	   #endif
	   timer.stop();
	   init_cascade_time += timer.elapsed();
	   timer.start();
	   eliminate_boundaries( data);
	   if( !data.cascade_boundary.empty() ){
	    //make tau sigma's partner
	    const Term& tau = data.cascade_boundary.youngest();
	   #ifdef DEBUG_PERSISTENCE
	    std::cerr << "Pairing to: " << fm[ tau.cell()] << std::endl;
	   #endif
	    store_scaled_cascade( data, sigma, output_policy);  
	    //make sigma tau's partner
	    cascade_boundary_map[ tau ].emplace( fm[ sigma], 1); 
	   } else{
	    store_cascade( data, sigma, output_policy); 
	   }
	   cascade_boundary_map[ sigma ].swap( data.cascade_boundary);
	   timer.stop();
	   persistence_algorithm += timer.elapsed();
	}
	return std::make_pair( init_cascade_time, persistence_algorithm);
}
} //end namespace detail
/**
* @brief Runs the persistence algorithm ala afras book chapter
* The function has many bells and whistles.
* In particular, one can change both the Input and Output Policies
* and turn on/off removal of negative rows all at compile time
* A Filtration_map[] is a level of indirection which associates
* a filtration iterator to a key_type for a Cell_to_chain_map.
* Regularly this map is just an identity.
*
* @param begin
* @param end
* @param bd
* @param cascade_boundary_map
* @param chain_data_initialized
* @param map
*/
template< bool Remove_destroyers=true,
	  typename Filtration_iterator, 
	  typename Boundary_operator,
	  typename Chain_map, 
	  typename Filtration_map = typename Boundary_operator::Filtration_map>
std::pair< double,double> 
persistence( Filtration_iterator begin,
	     Filtration_iterator end,
	     Boundary_operator & bd,
	     Chain_map & cascade_boundary_map,
	     Chain_map & cascade_map,
	     bool chain_data_initialized=false, 
	     Filtration_map map = Filtration_map()){
	if( chain_data_initialized){
	return detail::pair_cells< Remove_destroyers>( begin, end, bd, 
				cascade_boundary_map, cascade_map, map, 
			detail::bd_init(), detail::partner_and_cascade());
	}
	return detail::pair_cells< Remove_destroyers>( begin, end, bd, 
				cascade_boundary_map, cascade_map, map, 
			detail::empty_bd(), detail::partner_and_cascade());
}

/**
* @brief Runs the persistence algorithm ala afras book chapter
* The function has many bells and whistles.
* In particular, one can change both the Input and Output Policies
* and turn on/off removal of negative rows all at compile time
* A Filtration_map[] is a level of indirection which associates
* a filtration iterator to a key_type for a Cell_to_chain_map.
* Regularly this map is just an identity.
*
* @param begin
* @param end
* @param bd
* @param cascade_boundary_map
* @param chain_data_initialized
* @param map
*/
template< bool Remove_destroyers=true,
	  typename Filtration_iterator, 
	  typename Boundary_operator,
	  typename Chain_map, 
	  typename Filtration_map = ctl::identity>
std::pair< double, double> 
persistence( Filtration_iterator begin,
	     Filtration_iterator end,
	     Boundary_operator & bd,
	     Chain_map & cascade_boundary_map,
	     bool chain_data_initialized=false, 
	     Filtration_map  map = Filtration_map()){
	Chain_map not_going_to_be_used = cascade_boundary_map; 
	if( chain_data_initialized){
	return detail::pair_cells< Remove_destroyers>( begin, end, bd, 
				cascade_boundary_map, not_going_to_be_used, 
				map, detail::bd_init(), detail::partner());
	}
	return detail::pair_cells< Remove_destroyers>( begin, end, bd, 
				cascade_boundary_map, not_going_to_be_used, 
				map, detail::empty_bd(), detail::partner());
}		  
} //namespace ctl

#endif //CTLIB_PERSISTENCE_H
