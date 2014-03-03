#ifndef CTLIB_TERM_H
#define CTLIB_TERM_H


#include "finite_field/finite_field.h"
#include "term/term_tags.h"

//exported functionality
namespace ctl{

template< typename _Cell, typename _Coefficient>
class Term {
	private:
		typedef Term< _Cell, _Coefficient> Self;
	public:
		typedef _Cell Cell;
		typedef _Coefficient Coefficient;
		typedef _ctl::term_non_z2_tag coeff_tag;
	public:	
		Term() {}

		Term( Cell & cell_): _cell( cell_), _pos( 0), _coeff( 1) {}

		Term( const Cell & cell_, 
		      const Coefficient & coeff_, const std::size_t pos_):
			_cell( cell_), _pos(pos_), _coeff( coeff_) {}

		Term( const Term & from): _cell( from._cell),
					  _pos( from._pos),
					  _coeff( from._coeff) {}

		Term( Term && from): _cell( std::move( from._cell)),
				     _pos( std::move( from._pos)),
				     _coeff( std::move( from._coeff)){}

		Coefficient coefficient() const { return _coeff; }
		void coefficient( const Coefficient n) { _coeff = n; }
		void coefficient( const Coefficient n, 
				         const bool f) { _coeff *= n; }
		//we may store a filtration index.
		const std::size_t pos_() const { return _pos; }
		void pos_( const std::size_t _p) { _pos = _p; }
		
		Cell&            cell()       { return _cell; }
		const Cell&      cell() const { return _cell; }
			
		Self& operator=( const Self & from) { 
			_cell=from._cell;
			_coeff = from._coeff;
			return *this; 
		}
 
		bool operator==( const Self & from) const { 
			return _cell == from._cell;
		}

		bool operator!=( const Self & from) const { 
			return _cell != from._cell;
		}

		Term& operator+( const Coefficient & a){
			_coeff+=a;
			return *this;	
		}
		
	private:
		Cell _cell;
		std::size_t _pos;
		Coefficient _coeff;
		
	public:
	template <typename _Ce, typename _Co > 
	struct rebind {
  		typedef Term<_Ce, _Co> Term;
	};
};  //class Term

//Term specialization for Z_2
template< typename _Cell>
class Term< _Cell, ctl::Finite_field< 2> > {
	public:
		typedef _Cell Cell;
		typedef ctl::Finite_field< 2> Coefficient;
		typedef _ctl::term_z2_tag coeff_tag;
	private:
		typedef Term< _Cell, Coefficient > Self;
	public:
		Term() {}
		Term( const Cell & cell_): _cell( cell_), _pos( 0) {}	
		Term( const Cell & cell_, const Coefficient & C, 
		      const std::size_t pos_ ): _cell( cell_), _pos( pos_) {}	
		Cell& cell() { return _cell; }
		const Cell& cell() const { return _cell; }
		const Coefficient coefficient() const { return 1; }
		//for internal use to couple filtration order to our otherwise 
		//unordered objects.
		const std::size_t pos_() const { return _pos; }
		void pos_( const std::size_t _p) { _pos = _p; }
		
		template< typename C>
		void coefficient( const C & n) const {}
	
		Self& operator=( const Self & from) { 
			_cell=from._cell;
			return *this; 
		}
		//we ignore coefficients for correctness of
		//sentinal values in the boundary operator
		//otherwise we might not know we need to end
		bool operator==( const Self & from) const { 
			return (_cell == from._cell); 
		}
		bool operator!=( const Self & from) const { 
			return (_cell != from._cell); 
		}
	private:
		Cell _cell;
		std::size_t _pos;
	public:
	template <typename _Ce, typename _Co> 
	struct rebind { typedef Term<_Ce, _Co> Term; };

}; //Class Term specialization for Z2
} //namespace ctl

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_TERM_H
