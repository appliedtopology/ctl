#ifndef CTLIB_IO_H
#define CTLIB_IO_H
/*******************************************************************************
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
* Released under BSD-3 License. See LICENSE
*******************************************************************************/
//STL
#include <string>
#include <fstream>

namespace ctl{
constexpr char delta[] = "\u2202";
constexpr char sigma[] = "\u03C3";
constexpr char tau[] = "\u1D6D5";
constexpr char otimes[] = "\u2297";
constexpr char oplus[] = "\u2295";

struct identity {
    template<typename U>
    inline constexpr auto operator()(U&& v) const noexcept
        -> decltype(std::forward<U>(v))
    {
        return std::forward<U>(v);
    }
    template<typename U>
    inline constexpr U& operator[](U& v) const noexcept { return v; }

    template<typename X, typename U>
    constexpr auto operator()(X && y, U&& v) const noexcept
        -> decltype(std::forward<U>(v))
    {
        return std::forward<U>(v);
    }
 
    template<typename Cell, typename U>
    constexpr bool operator()(Cell && c, U&& v, bool f) const noexcept {
        return true;
    }

};

template< typename Stream>
bool open_file( Stream & in, const char* file_name){
	in.open( file_name);
	return in.is_open();
}
template< typename Stream>
void close_file( Stream & in){ in.close(); }

template< typename Stream>
bool get_line( Stream & in, std::string & line, std::size_t & line_num){
	while( in.good()){
		std::getline( in, line);
		++line_num;
		switch( line[0]){
			case '#':
			case '%':
			case '\0':
			break;
			default:		
			return true;
		}
	}
	return false;
}

} //namespace ctl

#endif //CTLIB_IO_H
