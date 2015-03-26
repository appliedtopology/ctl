#ifndef CTLIB_WEIGHT_DATA_H
#define CTLIB_WEIGHT_DATA_H
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

//non-exported functionality
namespace ctl {
namespace detail{} // end namespace detail
} //ctl namespace

//exported functionality
namespace ctl{

template< typename Weight_, typename Weight_less_ = std::less< Weight_> >
class Weight_data {
public:
  typedef Weight_ Weight;
  typedef Weight_less_ Weight_less;
  typedef std::numeric_limits< Weight_> limits;
  Weight_data(): weight_( Weight( limits::infinity())), less() {}
  Weight_data( const Weight& weight) : weight_( weight), less() {}
  Weight_data( const Weight_data &from) : weight_( from.weight_), less() {}
  Weight_data( Weight_data &&from) : weight_( std::move( from.weight_)), less() {}


  
  bool operator<( const Weight_data & d) const {
	return less( weight_, d.weight_); 
  }
  // assignment operator
  Weight_data& operator=( const Weight_data& from) {
    weight_ = from.weight_;
    return *this;
  }
  // assignment operator
  Weight_data& operator=( Weight_data&& from) {
    weight_ = std::move( from.weight_);
    return *this;
  }
  void uninitialize_weight() { weight_ = limits::infinity(); }
  bool initialized() const { return !(weight_ == limits::infinity()); }  
  Weight&        weight()         { return weight_; }
  void 		 weight( const Weight & w) { weight_ = w; }
  const Weight   weight() const   { return weight_; }
private:
  Weight_ weight_;
  Weight_less_ less;
}; // class Weight_data

} //namespace ctl


#endif //CTLIB_WEIGHT_DATA_H
