#ifndef CTLIB_TERM_LESS_H
#define CTLIB_TERM_LESS_H
/*******************************************************************************
* -Academic Honesty-
* Plagarism: The unauthorized use or close imitation of the language and 
* thoughts of another author and the representation of them as one's own 
* original work, as by not crediting the author. 
* (Encyclopedia Britannica, 2008.)
*
* You are free to use the code according to the license below, but, please
* do not commit acts of academic dishonesty. We strongly encourage and request 
* that for any [academic] use of this source code one should cite one the 
* following works:
* 
* \cite{hatcher, z-ct-10}
* 
* See ct.bib for the corresponding bibtex entries. 
* !!! DO NOT CITE THE USER MANUAL !!!
*******************************************************************************
* Copyright (C) Ryan H. Lewis 2014 <me@ryanlewis.net>
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
//exported functionality
namespace ctl{

template< typename Cell_less>
struct Term_cell_less{
   template< typename Term>
   bool operator()( const Term & l, const Term & r) const { 
  	return Cell_less( l.cell(), r.cell()); 
   } 
}; //struct Term_less

/*
//we mirror pos_() from l.cell()->second.pos() in the term
//so that we avoid a dereference.
struct Term_pos_less{
   template< typename Term>
   bool operator()( const Term & l, const Term & r) const { 
   	return l.pos() < r.pos();
   }
};// struct Term_pos_less
*/
} //namespace ctl

#endif //CTLIB_TERM_LESS_H
