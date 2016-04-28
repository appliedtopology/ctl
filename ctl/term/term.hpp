#ifndef CTLIB_TERM_H
#define CTLIB_TERM_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
*******************************************************************************/

#include <ctl/finite_field/finite_field.hpp>
#include <ctl/term/term_tags.hpp>

//exported functionality
namespace ctl{

template< typename Cell_, typename Coefficient_>
class Term {
	private:
		typedef Term< Cell_, Coefficient_> Self;
	public:
		typedef Cell_ Cell;
		typedef Coefficient_ Coefficient;
		typedef ctl::detail::term_non_z2_tag coeff_tag;
	public:	
		Term() {}
		Term( Cell && cell): cell_( cell), coeff_( 1) {}
		Term( const Cell & cell, 
		      const Coefficient & coeff):
			cell_( cell), coeff_( coeff) {}
		Term( std::pair<Cell, Coefficient> &pair): cell_( pair.first), coeff_( pair.second) {}
	
                Term( const Term & from): cell_( from.cell_),
					  coeff_( from.coeff_) {}

		Term( Term && from): cell_( std::move( from.cell_)), 
				     coeff_( std::move( from.coeff_)){}

		Coefficient coefficient() const { return coeff_; }
		void coefficient( const Coefficient n) { coeff_ = n; }
		void coefficient( const Coefficient n, 
				         const bool f) { coeff_ *= n; }
		Cell&            cell()       { return cell_; }
		const Cell&      cell() const { return cell_; }
			
		Self& operator=( const Self & from) { 
			cell_=from.cell_;
			coeff_ = from.coeff_;
			//pos_ = from.pos_;
			return *this; 
		}
 		Self& operator=( const Self && from) { 
			cell_= std::move( from.cell_);
			coeff_ = std::move( from.coeff_);
			return *this; 
		}
		template< typename Coeff>
		Self& operator*=( const Coeff& c){
			coeff_ *= c;
			return *this;
		}
		bool operator==( const Self & from) const { 
			return cell_ == from.cell_;
		}

		bool operator!=( const Self & from) const { 
			return cell_ != from.cell_;
		}
		template< typename T>
		inline bool operator< ( const T & t) const { return cell_ < t.cell(); } 
		Term& operator+( const Coefficient & a){
			coeff_+=a;
			return *this;	
		}

		template< typename T>
		inline Self operator*( T k) const{
			return Self( cell_, k*coeff_);
		} 
		
	private:
		Cell cell_;
		Coefficient coeff_;
		
	public:
	template <typename Ce, typename Co > 
	struct rebind {
  		typedef Term< Ce, Co> T;
	};
};  //class Term

//Term specialization for Z_2
template< typename Cell_>
class Term< Cell_, ctl::Finite_field< 2> > {
	public:
		typedef Cell_ Cell;
		typedef ctl::Finite_field< 2> Coefficient;
		typedef ctl::detail::term_z2_tag coeff_tag;
	private:
		typedef Term< Cell_, Coefficient > Self;
	public:
		Term() {}
		Term( const Self & from): cell_( from.cell_){}
		Term( const Self && from): cell_( std::move( from.cell_)){}
		Term( const Cell & cell): cell_( cell) {}	

		Term( std::pair<Cell, Coefficient> &pair): cell_( pair.first) {}
                Term( const Cell & cell, const Coefficient & coeff): 
		cell_( cell) {}	
		Cell& cell() { return cell_; }
		const Cell& cell() const { return cell_; }
		const Coefficient coefficient() const { return Coefficient( 1);}
		template< typename C>
		void coefficient( const C & n) const {}

		template< typename T>
		bool operator< ( const T & t) const { return cell_ < t.cell(); }


		Self& operator=( const Self & from) { 
			cell_=from.cell_;
			return *this; 
		}
		Self& operator=( const Self && from) { 
			cell_= std::move( from.cell_);
			return *this; 
		}
		/**
		 * These things aren't strictly speaking correct.
		 */
		template< typename T>
		Self& operator*=( const T& c){
			return *this;
		}
		template< typename T>
		inline Self operator*( T k) const{
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
	public:
	template <typename _Ce, typename _Co> 
	struct rebind { typedef Term<_Ce, _Co> T; };

}; //Class Term specialization for Z2

namespace detail{
template<typename T, typename = void>
struct is_iterator { static constexpr bool value = false; };
template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
{ static constexpr bool value = true; };

template<typename Stream, class T, typename std::enable_if<!is_iterator<T>::value, T>::type* = nullptr> 
void  print_cell(Stream& out, const T & t){ out << t; }
template<typename Stream, class T, typename std::enable_if<is_iterator<T>::value, T>::type* = nullptr> 
void  print_cell(Stream& out, const T & t){ out << t->first; }
}

template< typename Stream, typename Ce, typename Co>
Stream& 
operator<<(Stream&out, const Term<Ce, Co>&term){
	if( term.coefficient() != Co(1)){
		out << term.coefficient() << "*";
        	out << "[";	
	}
	detail::print_cell( out, term.cell());
	if( term.coefficient() != Co(1)){
        	out << "]";	
	}
	return out;
}
} //namespace ctl

template< typename Cell, typename Coefficient, typename T>
ctl::Term< Cell, Coefficient> 
operator*( T k, const ctl::Term< Cell, Coefficient> & m){ return m*k; }

#endif //CTLIB_TERM_H
