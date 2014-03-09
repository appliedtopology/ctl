#ifndef CTLIB_TERM_H
#define CTLIB_TERM_H


#include "finite_field/finite_field.h"
#include "term/term_tags.h"

//exported functionality
namespace ctl{

template< typename Cell_, typename Coefficient_>
class Term {
	private:
		typedef Term< Cell_, Coefficient_> Self;
	public:
		typedef Cell_ Cell;
		typedef Coefficient_ Coefficient;
		typedef _ctl::term_non_z2_tag coeff_tag;
	public:	
		Term() {}

		Term( Cell & cell): cell_( cell), pos_( 0), coeff_( 1) {}

		Term( const Cell & cell, 
		      const Coefficient & coeff, const std::size_t pos):
			cell_( cell), pos_(pos), coeff_( coeff) {}

		Term( const Term & from): cell_( from.cell_),
					  pos_( from.pos_),
					  coeff_( from.coeff_) {}

		Term( Term && from): cell_( std::move( from.cell_)),
				     pos_( std::move( from.pos_)),
				     coeff_( std::move( from.coeff_)){}

		Coefficient coefficient() const { return coeff_; }
		void coefficient( const Coefficient n) { coeff_ = n; }
		void coefficient( const Coefficient n, 
				         const bool f) { coeff_ *= n; }
		//we may store a filtration index.
		const std::size_t pos() const { return pos_; }
		void pos( const std::size_t p) { pos_ = p; }
		
		Cell&            cell()       { return cell_; }
		const Cell&      cell() const { return cell_; }
			
		Self& operator=( const Self & from) { 
			cell_=from.cell_;
			coeff_ = from.coeff_;
			pos_ = from.pos_;
			return *this; 
		}
 		Self& operator=( const Self && from) { 
			cell_= std::move( from.cell_);
			coeff_ = std::move( from.coeff_);
			pos_ = std::move( from.pos_);
			return *this; 
		}
		bool operator==( const Self & from) const { 
			return cell_ == from.cell_;
		}

		bool operator!=( const Self & from) const { 
			return cell_ != from.cell_;
		}
		template< typename T>
		bool operator< ( const T & t) const { return cell_ < t.cell(); } 
		Term& operator+( const Coefficient & a){
			coeff_+=a;
			return *this;	
		}
		
	private:
		Cell cell_;
		std::size_t pos_;
		Coefficient coeff_;
		
	public:
	template <typename Ce, typename Co > 
	struct rebind {
  		typedef Term< Ce, Co> Term;
	};
};  //class Term

//Term specialization for Z_2
template< typename Cell_>
class Term< Cell_, ctl::Finite_field< 2> > {
	public:
		typedef Cell_ Cell;
		typedef ctl::Finite_field< 2> Coefficient;
		typedef _ctl::term_z2_tag coeff_tag;
	private:
		typedef Term< Cell_, Coefficient > Self;
	public:
		Term() {}
		Term( const Self & from): 
		cell_( from.cell_), pos_( from.pos_){}
		Term( const Self && from): cell_( std::move( from.cell_)), 
					   pos_( std::move( from.pos_)) {}
		Term( const Cell & cell): cell_( cell), pos_( 0) {}	
		Term( const Cell & cell, const Coefficient & coeff, 
		      const std::size_t pos): cell_( cell), pos_( pos) {}	
		Cell& cell() { return cell_; }
		const Cell& cell() const { return cell_; }
		const Coefficient coefficient() const { return Coefficient( 1);}
		//for internal use to couple filtration order to our otherwise 
		//unordered objects.
		const std::size_t pos() const { return pos_; }
		void pos( const std::size_t p) { pos_ = p; }
		
		template< typename C>
		void coefficient( const C & n) const {}

		template< typename T>
		bool operator< ( const T & t) const { return cell_ < t.cell(); } 
	
		Self& operator=( const Self & from) { 
			cell_=from.cell_;
			pos_=from.pos_;
			return *this; 
		}
		Self& operator=( const Self && from) { 
			cell_= std::move( from.cell_);
			pos_= std::move( from.pos_);
			return *this; 
		}

		//we ignore coefficients for correctness of
		//sentinal values in the boundary operator
		//otherwise we might not know we need to end
		bool operator==( const Self & from) const { 
			return (cell_ == from.cell_); 
		}
		bool operator!=( const Self & from) const { 
			return (cell_ != from.cell_); 
		}
	private:
		Cell cell_;
		std::size_t pos_;
	public:
	template <typename _Ce, typename _Co> 
	struct rebind { typedef Term<_Ce, _Co> Term; };

}; //Class Term specialization for Z2
} //namespace ctl

//non-exported functionality
namespace {} //anonymous namespace

#endif //CTLIB_TERM_H
