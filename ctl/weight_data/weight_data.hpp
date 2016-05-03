#ifndef CTLIB_WEIGHT_DATA_H
#define CTLIB_WEIGHT_DATA_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
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
