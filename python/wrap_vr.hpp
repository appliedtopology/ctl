#include <ctl/vr/vr.hpp>

template< typename T>
class Matrix_view {
    typedef T* pointer;
    typedef T* const const_pointer;
    public:
    typedef T value_type;	
    typedef pointer iterator;
    typedef const_pointer const_iterator;    
    typedef T& reference;
    typedef typename std::add_const< reference>::type const_reference;
    typedef typename std::size_t size_t;
 
    public:
    
    Matrix_view( pointer d, size_t m, size_t n):
    m_( m), n_( n), data_( d){}

    reference operator() (size_t i, size_t j) { return data_[ at( i, j)]; }
    const_reference operator() (size_t i, size_t j) const { return data_[ at( i, j)]; }

    iterator 	   begin() { return data_; }
    const_iterator begin() const { return data_; }
  
    iterator 	   end() { return data_ + m_*n_; }
    const_iterator end() const { return data_ + m_*n_; }

    iterator 	   begin( std::size_t i) { return begin() + at( i) ; }
    const_iterator begin( std::size_t i) const { return begin() + at( i); }
  
    iterator 	   end( std::size_t i) { return begin() + at( i+1); }
    const_iterator end( std::size_t i) const { return begin() + at( i+1); }

    size_t m() const { return m_; }			
    size_t n() const { return n_; }
    
    size_t size() const { return m_; } 
    size_t capacity() const { return m_*n_; }
    pointer data() { return data_; }
    const_pointer data() const { return data_; }

    private:
    inline size_t at( size_t i, size_t j) const { return i*n_ + j; }
    inline size_t at( size_t i) const { return i*n_; }

    private:
    size_t m_;
    size_t n_;
    pointer data_;
}; //end Matrix_view class

ctl::Simplicial_complex<>
vr_wrapper(py::array_t<double> array, double epsilon, std::size_t dimension) { 
   auto info = array.request();
   if( info.ndim != 2){
    throw std::runtime_error("Number of dimensions must be two");
   }
   Matrix_view<double> view( (double*)info.ptr, info.shape[0], info.shape[1]);
   return ctl::vr( view, epsilon, dimension);
}

// Creates a Python class for an `Abstract_simplex`. 
void wrap_vr(py::module &mod){
  mod.def("vr", &vr_wrapper, "compute the vr complex of a set of points. returns list of simplices",  py::keep_alive<0,1>());
}
