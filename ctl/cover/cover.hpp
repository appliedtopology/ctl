#ifndef CTL_COVER_HPP
#define CTL_COVER_HPP
#include <vector>
#include <functional>
#include <ctl/cell_complex/cell_complex.hpp>

namespace ctl {
template< typename Complex>
decltype(auto)
open_star_cover(const Complex& K, const std::function<std::size_t(std::size_t)>& f){
	ctl::Simplicial_complex<> nerve;
	std::vector< ctl::Abstract_simplex> map_to_nerve;
	map_to_nerve.reserve(K.size());
	for(const auto& sigma : K){
		ctl::Abstract_simplex tau;
		for( const auto& vtx: sigma.first){ tau.insert(f(vtx)); }
		const auto& tau_cell = nerve.insert_closed_cell( tau);
		//TODO: These should be references..
		map_to_nerve.emplace_back(tau_cell.first->first);
	}
	return std::make_pair(nerve, map_to_nerve);
}

}//end namespace ctl
#endif //CTL_COVER_HPP
