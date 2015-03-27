#ifndef CTL_FLATTENING_ITERATOR_H
#define CTL_FLATTENING_ITERATOR_H
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
* Released under BSD-3 License. See LICENSE for more details
*******************************************************************************/
#include <iterator>
namespace ctl{ 
namespace detail{
// A forward iterator that "flattens" a container of containers.  For example,
// a vector<vector<int>> containing { { 1, 2, 3 }, { 4, 5, 6 } } is iterated as
// a single range, { 1, 2, 3, 4, 5, 6 }.
template <typename OuterIterator, typename InnerIterator> 
class flattening_iterator_base : public boost::iterator_facade<
flattening_iterator_base< OuterIterator, InnerIterator>,
typename InnerIterator::value_type, 
boost::forward_traversal_tag, 
typename std::iterator_traits< InnerIterator>::reference> {
public:

    typedef OuterIterator                                outer_iterator;
    typedef InnerIterator inner_iterator;

    flattening_iterator_base(){}

    template< typename O, typename I> 
    flattening_iterator_base(const flattening_iterator_base< O, I>& o): 
    inner_it_( o.inner_it_), outer_it_( o.outer_it_), outer_end_( o.outer_end_){}

    template< typename O, typename I> 
    flattening_iterator_base(flattening_iterator_base< O, I>&& o): 
    inner_it_( std::move( o.inner_it_)), outer_it_( std::move( o.outer_it_)), outer_end_( std::move( o.outer_end_)){}
 
    
    flattening_iterator_base(outer_iterator it) : inner_it_(), outer_it_(it), outer_end_(it){}
    flattening_iterator_base(inner_iterator it, outer_iterator b, outer_iterator e) : inner_it_( it), outer_it_( b), outer_end_( e) { }
    flattening_iterator_base(outer_iterator it, outer_iterator end) 
        : outer_it_(it), 
          outer_end_(end){ 
        if (outer_it_ == outer_end_) { return; }
        inner_it_ = outer_it_->begin();
        increment(); 
    }
    template< typename O, typename I>
    flattening_iterator_base& operator=(const flattening_iterator_base< O, I>& o){
	outer_it_ = o.outer_it_;
	outer_end_ = o.outer_end_;
	inner_it_ = o.inner_it_;
	return *this;
    }
 
    template< typename O, typename I>
    flattening_iterator_base& operator=(flattening_iterator_base< O, I>&& o){
	outer_it_ = std::move( o.outer_it_);
	outer_end_ = std::move( o.outer_end_);
	inner_it_ = std::move( o.inner_it_);
	return *this;
    } 

private:
   friend class boost::iterator_core_access;
   template< typename T, typename O>
   friend class flattening_iterator_base;
   typename std::iterator_traits< inner_iterator>::reference dereference() const { return *inner_it_; }

    bool equal(const flattening_iterator_base& other) const {
        if (outer_it_ != other.outer_it_){ return false; }
        if (outer_it_ != outer_end_ && 
            other.outer_it_ != other.outer_end_ &&
            inner_it_ != other.inner_it_){
            return false;
 	}
        return true;
    }

    void increment(){
      while (outer_it_ != outer_end_ && inner_it_ == outer_it_->end()){
            ++outer_it_;
            if (outer_it_ != outer_end_){
                inner_it_ = outer_it_->begin();
	    } 
    }
    }

    inner_iterator inner_it_;
    outer_iterator outer_it_;
    outer_iterator outer_end_;
};

template< typename I>
using flattening_iterator = flattening_iterator_base< I, typename I::value_type::iterator>;
template< typename I>
using const_flattening_iterator = flattening_iterator_base< I, typename I::value_type::const_iterator>; 
} //end namespace detail


template <typename Iterator>
detail::flattening_iterator<Iterator> flatten(Iterator end)
{
    return detail::flattening_iterator<Iterator>(end, end);
}

template <typename Iterator>
detail::flattening_iterator<Iterator> flatten(Iterator begin, Iterator end)
{
    return detail::flattening_iterator<Iterator>(begin, end);
}


template <typename Iterator>
detail::const_flattening_iterator<Iterator> const_flatten(Iterator end)
{
    
    return detail::const_flattening_iterator<Iterator>(end, end);
}

template <typename Iterator>
detail::const_flattening_iterator<Iterator> const_flatten(Iterator begin, Iterator end)
{
    return detail::const_flattening_iterator<Iterator>(begin, end);
}






} //end namespace ctl

#endif //CTLIB_FLATTENING_ITERATOR_H
