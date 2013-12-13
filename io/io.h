#ifndef CTLIB_IO_H
#define CTLIB_IO_H
#include <string>

namespace ct{

template< typename Stream>
bool open_file( Stream & in, const char* file_name){
	in.open( file_name);
	return in.is_open();
}
template< typename Stream>
void close_file( Stream & in){ in.close(); }

template< typename Stream>
bool get_line( Stream & in, std::string & line, std::size_t line_num){
	while(in.good()){
		in.getline( line);
		++line_num;
		switch( line[0]){
			case '#':
			case '%':
			case '\0':
			case '{':
			case '}':
			case '\\':
			break;
			default:		
			return true;
		}
	}
	return false;
}

} //namespace ct

#endif //CTLIB_IO_H
