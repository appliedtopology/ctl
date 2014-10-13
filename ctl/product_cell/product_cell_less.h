#ifndef _CTL_PRODUCT_CELL_LESS_H_
#define _CTL_PRODUCT_CELL_LESS_H_
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
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
*******************************************************************************
* NOTES
*
*
*******************************************************************************/
namespace ctl {
// For testing
// Id comparison and cover_cell ordering
template< typename Cell_iterator>
struct Product_first_less  {
  typedef Cell_iterator first_argument_type;
  typedef Cell_iterator second_argument_type;
  typedef         bool result_type;
  bool operator()( const Cell_iterator& c1,
		   const Cell_iterator& c2) const {
    const auto&  c1_first = c1->first.first_cell();
    const auto&  c1_second = c1->first.second_cell();
   
    const auto&  c2_first = c2->first.first_cell();
    const auto&  c2_second = c2->first.second_cell();
    return (c1_first < c2_first) || ((c2_first == c1_first) && (c1_second < c2_second));
  }
}; // class Product_first_less

template< typename Cell_iterator>
struct Product_second_less  {
  typedef Cell_iterator first_argument_type;
  typedef Cell_iterator second_argument_type;
  typedef         bool result_type;
  private:
  public:
  bool operator()( const Cell_iterator& c1,
		   const Cell_iterator& c2) const {

    const auto&  c1_first = (c1)->first.first_cell();
    const auto&  c1_second = (c1)->first.second_cell();
   
    const auto&  c2_first = (c2)->first.first_cell();
    const auto&  c2_second = (c2)->first.second_cell();
    return (c1_second < c2_second) || ((c2_second == c1_second) && (c1_first < c2_first));
  }

}; // class Product_second_less
} //end namespace ctl
#endif //product_cell_less
