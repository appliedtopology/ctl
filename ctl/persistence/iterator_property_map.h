#ifndef CTLIB_ITERATOR_PROPERTY_MAP_H
#define CTLIB_ITERATOR_PROPERTY_MAP_H
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
* \cite{kuhl-design-96, z-ct-10}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
*******************************************************************************/
#include <boost/property_map/property_map.hpp>

//code here modeled after:

namespace ctl {
//=========================================================================
  // Adapter to turn a RandomAccessIterator into a property map
  template <class RandomAccessIterator, 
    class IndexMap
#ifdef BOOST_NO_STD_ITERATOR_TRAITS
    , class T, class R
#else
    , class T = typename std::iterator_traits<RandomAccessIterator>::value_type
    , class R = typename std::iterator_traits<RandomAccessIterator>::reference
#endif
     >
  class iterator_property_map
    : public boost::put_get_helper< R, 
        iterator_property_map<RandomAccessIterator, IndexMap,
        T, R> >
  {
  public:
    typedef typename boost::property_traits<IndexMap>::key_type key_type;
    typedef T value_type;
    typedef R reference;
    typedef boost::lvalue_property_map_tag category;

    inline iterator_property_map(
      RandomAccessIterator cc = RandomAccessIterator(), 
      const IndexMap& _id = IndexMap() ) 
      : iter(cc), index(_id) { }
    template< typename key_type_>
    inline R operator[](key_type_ v) const { return *(iter + get(index, v)) ; }
  protected:
    RandomAccessIterator iter;
    IndexMap index;
  };

#if !defined BOOST_NO_STD_ITERATOR_TRAITS
  template <class RAIter, class ID>
  inline iterator_property_map<
    RAIter, ID,
    typename std::iterator_traits<RAIter>::value_type,
    typename std::iterator_traits<RAIter>::reference>
  make_iterator_property_map(RAIter iter, ID id) {
    boost::function_requires< boost::RandomAccessIteratorConcept<RAIter> >();
    typedef iterator_property_map<
      RAIter, ID,
      typename std::iterator_traits<RAIter>::value_type,
      typename std::iterator_traits<RAIter>::reference> PA;
    return PA(iter, id);
  }
#endif
  template <class RAIter, class Value, class ID>
  inline iterator_property_map<RAIter, ID, Value, Value&>
  make_iterator_property_map(RAIter iter, ID id, Value) {
    boost::function_requires< boost::RandomAccessIteratorConcept<RAIter> >();
    typedef iterator_property_map<RAIter, ID, Value, Value&> PMap;
    return PMap(iter, id);
  }
} //end namespace ctl
#endif //CTLIB_ITERATOR_PROPERTY_MAP
