#ifndef CTLIB_IO_H
#define CTLIB_IO_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the below liscence, but please
* do not commit acts of academic dishonesty. We encourage and request that 
* for any academic use of this source code one should cite the following 
* papers:
* 
* \cite{$bibtex_names_here}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) $NAMES_OF_AUTHORS $YEARS_FIRST_CREATED <$emails>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program in a file entitled COPYING; if not, write to the 
* Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************************
*******************************************************************************/
//STL
#include <string>
#include <fstream>

namespace ctl{
constexpr char delta[] = "\u2202";
constexpr char sigma[] = "\u03C3";
constexpr char tau[] = "\u1D6D5";

struct identity {
    template<typename U>
    constexpr auto operator()(U&& v) const noexcept
        -> decltype(std::forward<U>(v))
    {
        return std::forward<U>(v);
    }
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
