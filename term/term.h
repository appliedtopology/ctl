#ifndef TERM_H
#define TERM_H
#include "finite_field/finite_field.h"

//exported functionality
namespace ctl{

template< typename _Cell, typename _Coefficient>
class Term {
	private:
		typedef Term< _Cell, _Coefficient> Self;
	public:
		typedef _Cell Cell;
		typedef _Coefficient Coefficient;
		
		Term() {}
		Term( Cell & cell_): _cell( cell_), _coeff( 1) {};
		Term( Cell & cell_, Coefficient & coeff_, std::size_t pos_=0): 
			_cell( cell_), _coeff( coeff_), _pos(pos_)  {}
		Term( const Term & from): _cell( from._cell),
					  _coeff( from._coeff), 
					  _pos( from._pos) {};	
		Coefficient coefficient() const { return _coeff; }
		void coefficient( Coefficient n) { _coeff = n; }
		
		Cell&            cell()       { return _cell; }
		const Cell&      cell() const { return _cell; }
		const std::size_t pos() const { return _pos; }

		Self& operator=( const Self & from) { 
			_cell=from._cell;
			_coeff = from._coeff;
			_pos = from._pos;
			return *this; 
		} 
		bool operator==( const Self & from){ 
			return _cell == from._cell && _coeff == from._coeff;
		}
		
	private:
		Cell _cell;
		Coefficient _coeff;
		std::size_t _pos;
	public:
	template <typename _Ce, typename _Co > 
	struct rebind {
  		typedef Term<_Ce, _Co> Term;
	};
};  //class Term

//Term specialization for Z_2
template< typename _Cell >
class Term< _Cell, ctl::Finite_field< 2> > {
	public:
		typedef _Cell Cell;
		typedef ctl::Finite_field< 2> Coefficient;
	private:
		typedef Term< _Cell, Coefficient > Self;
	public:
		
		Term() {}
		Term( const Cell & cell_): _cell( cell_), _pos( 0) {}	
		Term( const Cell & cell_, const Coefficient & C, 
		      const std::size_t pos_ ): _cell( cell_), _pos( pos_) {}	
		Cell& cell() { return _cell; }
		const Cell& cell() const { return _cell; }
		Coefficient coefficient() const { return 1; }
		const std::size_t pos() const { return pos; }
		template< typename C>
		void coefficient( const C & n) const {}
	
		Self& operator=( const Self & from) { 
			_cell=from._cell;
			return *this; 
		} 
		bool operator==( const Self & from) const { 
			return (_cell == from._cell); 
		}
		bool operator<( const Self & from) const { 
			return (_cell < from.cell); 
		}
		
	private:
		Cell _cell;
		std::size_t _pos;
	public:
	template <typename _Ce, typename _Co> 
	struct rebind { typedef Term<_Ce, _Co> Term; };

}; //Class Term specialization for Z2
} //namespace ct

//non-exported functionality
namespace {} //anonymous namespace

#endif //TERM_H
