#ifndef CTLIB_TERM_LESS_H
#define CTLIB_TERM_LESS_H
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
*******************************************************************************/

//exported functionality
namespace ctl{

template< typename Cell_less_>
struct Term_cell_less{
   typedef Cell_less_ Cell_less;
   Term_cell_less(){}
   Term_cell_less( const Cell_less & l): less( l){} 
   template< typename Term>
   inline bool operator()( const Term & l, const Term & r) const { 
       return less( l.cell(), r.cell()); 
   } 
   Cell_less less;
}; //struct Term_less

template< typename Cell_less_>
struct Filtration_term_cell_less{
   typedef Cell_less_ Cell_less;
   Filtration_term_cell_less(){}
   Filtration_term_cell_less( const Cell_less & l): less( l){} 
   template< typename Term>
   inline bool operator()( const Term & l, const Term & r) const { 
       return less( *(l.cell()), *(r.cell())); 
   } 
   Cell_less less;
}; //struct Term_less


} //namespace ctl

#endif //CTLIB_TERM_LESS_H
