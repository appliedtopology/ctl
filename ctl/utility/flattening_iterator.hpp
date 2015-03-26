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
template <typename OuterIterator>
class flattening_iterator : public boost::iterator_facade<
flattening_iterator< OuterIterator>, 
typename OuterIterator::value_type::iterator::value_type,
boost::forward_traversal_tag> {
public:

    typedef OuterIterator                                outer_iterator;
    typedef typename OuterIterator::value_type::iterator inner_iterator;

    flattening_iterator() { }
    flattening_iterator(outer_iterator it) : outer_it_(it), outer_end_(it) { }
    flattening_iterator(outer_iterator it, outer_iterator end) 
        : outer_it_(it), 
          outer_end_(end){ 
        if (outer_it_ == outer_end_) { return; }
        inner_it_ = outer_it_->begin();
        increment(); 
    }

private:
   friend class boost::iterator_core_access;

    bool equal(const flattening_iterator& other) const {
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
            if (outer_it_ != outer_end_) 
                inner_it_ = outer_it_->begin();
     }
    }

    outer_iterator outer_it_;
    outer_iterator outer_end_;
    inner_iterator inner_it_;
};

} //end namespace detail


template <typename Iterator>
flattening_iterator<Iterator> flatten_end(Iterator end)
{
    return flattening_iterator<Iterator>(end, end);
}

template <typename Iterator>
flattening_iterator<Iterator> flatten_end(Iterator begin, Iterator end)
{
    return flattening_iterator<Iterator>(end, end);
}

template <typename Iterator>
flattening_iterator<Iterator> flatten_begin(Iterator begin, Iterator end)
{
    return flattening_iterator<Iterator>(begin, end);
}
} //end namespace ctl

#endif //CTLIB_FLATTENING_ITERATOR_H
