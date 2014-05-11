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

    const std::size_t nerve_id_1 = c1->first.first->second.id();
    const std::size_t nerve_id_2 = c2->first.first->second.id();

    //assumes the nerve has been built closed. 
    return (nerve_id_1 < nerve_id_2) || (!(nerve_id_1 > nerve_id_2) && 
	   (c1->first.second_cell() < c2->first.second_cell()));
  }
}; // class Product_first_less

template< typename Cell_iterator>
struct Product_second_less  {
  typedef Cell_iterator first_argument_type;
  typedef Cell_iterator second_argument_type;
  typedef         bool result_type;
  private:
  typedef typename Cell_iterator::value_type::first_type Product_cell;
  typedef typename Product_cell::Cell_iterator1 First_cell_iterator;
  typedef typename Product_cell::Cell_iterator2 Second_cell_iterator;
  public:
  bool operator()( const Cell_iterator& c1,
		   const Cell_iterator& c2) const {

    const std::size_t nerve_id_1 = c1->first.first->second.id();
    const std::size_t nerve_id_2 = c2->first.first->second.id();
    //assumes the nerve has been built closed.
    //also it assumes pointer arithmetic fancyness in or statement to avoid
    //second cell < other_cell comparison 
    return (c1->first.second_cell() < c2->first.second_cell()) || 
		((c1->first.second == c2->first.second) &&  
			nerve_id_1 < nerve_id_2);
  }
}; // class Product_second_less
} //end namespace ctl
#endif //product_cell_less
