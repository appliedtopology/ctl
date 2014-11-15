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
