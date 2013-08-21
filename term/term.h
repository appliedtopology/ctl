#ifndef TERM_H
#define TERM_H

//exported functionality
namespace ct{

template< typename _Cell, typename _Coefficient>
class Term {
	private:
		typedef Term< _Cell, _Coefficient> Self;
	public:
		typedef _Cell Cell;
		typedef _Coefficient Coefficient;
		Term() {}
		Term( Cell & cell_, Coefficient & coeff_): 
			_cell( cell_), _coeff( coeff_) {}	
		Coefficient coefficient() const { return _coeff; }
		void coefficient( Coefficient n) { _coeff = n; }
		
		Cell& cell() { return _cell; }
		const Cell& cell() const { return _cell; }
		void cell( Cell & cell_) { _cell=cell_; } 
	private:
		Cell _cell;
		Coefficient _coeff;
};  //class Term


} //namespace ct

//non-exported functionality
namespace {} //anonymous namespace

#endif //TERM_H
