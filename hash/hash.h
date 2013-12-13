#ifndef CTLIB_HASH_H
#define CTLIB_HASH_H

//non-exported functionality 
namespace {} //anon. namespace

namespace ct {
	template< typename T>
	struct Hash{
		//TODO: make this smarter..
		std::size_t operator()( const T & key) const{
			std::size_t hash=0;
			std::size_t p=37;
			for(auto i = key.begin(); i != key.end(); ++i){
				hash += p*hash + *i;
			}
			return hash;
		}
	}; //class Hash
} //namespace ct
#endif //CTLIB_HASH_H
