#ifndef _CTL_COVER_STATS_H_
#define _CTL_COVER_STATS_H_
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
* Copyright (C) Ryan H. Lewis 2011 <me@ryanlewis.net>
*******************************************************************************
* ********** BSD-3 License ****************
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, 
* this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software without 
* specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
*******************************************************************************/
// STL
#include <set>
//#include <algorithm> // not sure if algorithm,sstream are necessary yet
//#include <iterator> 
//#include <map> 
//#include <utility>
//#include <iostream>
//#include <sstream>

// BGL
//#include <boost/graph/adjacency_list.hpp> 
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/properties.hpp>
//#include <boost/graph/connected_components.hpp>

//// PROJECT
//#include "io.h" 
//#include "nbhd_graph.h"
//#include "one_skeleton.h"
//make this 'generic' in the arr variable?
namespace ctl { 
namespace parallel{
template<typename Timer>
struct Cover_stats{
	Cover_stats(): timer(), init_time(0.0), 
	partition_time(0.0), cover_time(0.0),
	open_time(0.0), close_time(0.0){}
	Timer timer;
	double init_time;
	double partition_time;
	double cover_time;
	double open_time;
	double close_time;
 	int num_threads;
};



template<typename Stat>
void make_prob(Stat &  arr, int & size){
	double total = 0.0;
	for (int i = 0; i < size; ++i){
		total += arr[i];
	}
	for (int i = 0; i < size; ++i){
		arr[i] /= total;
	}
}
/**
void simplestats(double * arr, int & size, double & mean, double & var){
	mean = 0.0;
	for (int i = 0; i < size; ++i){
		mean = mean + arr[i];
	}
	mean = mean/((double)size);
	var = 0.0;
	for (int i = 0; i < size; ++i){
		double tmp = arr[i] - mean;
		var = var + tmp*tmp;
	}
	var = var/((double)size);
}**/
template<typename Stat>
void stats(Stat & arr, int & size,
           double & variance1, double & rel_var, double & expVal1, 
	   double & uniform_rel1, double & size_ratio){
       double expVal = 0.0;
	double	variance = 0.0;
	double uniform_rel = 0.0;
	double max = arr[0];
	double min = arr[0];
        for (int i = 0; i < size;++i){
        	expVal += ((double)(i+1))*((double)arr[i]);
		max = std::max(max,arr[i]);
		min = std::min(min,arr[i]);
		variance += ((double)((i+1)*(i+1)))*((double)arr[i]);
        }
	variance -=  (expVal)*(expVal);
	size_ratio = max/min;
	double uniform_mean = ((double)size + 1.0)/2.0; 
	uniform_rel = (expVal) / uniform_mean;
	uniform_rel1 = uniform_rel;
	expVal1 = expVal;
	variance1 = variance;
	double uni_var = (((double)(size*size))-1.0)/12.0;
	rel_var = ((double)variance)/(uni_var);
	
}
bool test_stats(){
	int size =  2;
	double  arr[2];
	for (int i = 0; i < size; ++i){
		arr[i] = 1.0/((double)size);
	}
	double expVal, var,relvar, uniform_rel, ratio;
	stats(arr,size,var,relvar,expVal,uniform_rel,ratio);
	std::cout << expVal << " " << var << " " << uniform_rel << std::endl;
	return true;
}

template<typename Complex>
void print_partition_header(Complex & c){
	std::cout << "partition size" << " , ";
        std::cout << "edgecut" << " , ";
        std::cout << "relative edgecut" << " , ";
        std::cout << "relative expected value" << " ,";
        std::cout << "expected value" << " ," ;
        std::cout << "variance " << " , ";
        std::cout << "relative variance" << " , ";
	std::cout << "ratio largest/smallest" << " , " ;
}
template<typename Complex>
void print_cover_header(Complex & c){
        std::cout << "relative expected value" << " ,";
        std::cout << "expected value" << " ," ;
        std::cout << "variance " << " , ";
        std::cout << "relative variance" << " , ";
	std::cout << "ratio largest/smallest" << " , " ;
        std::cout << "intersection size (%) "  << " , ";
}
template<typename Complex>
void print_header(Complex & c){
        std::cout << "filename" << " , ";
        std::cout << "simplices" << " , ";
        std::cout << "vertices" << " ,";
        std::cout << "partition time" << " ," ;
        std::cout << "covering time" << " , ";
        std::cout << "entire time" << " , ";
        std::cout << "edges"  << " , ";
        std::cout << "edge %" << " , ";
	std::cout << ",";
        print_partition_header(c);
	std::cout << ",";
        print_cover_header(c);
	std::cout << std::endl;
}




//TODO: fix precision of double output
void partition_results(int & numparts,
		      int & edgecut,
		      double & rel_edgecut,
		      double & part_relExpVal,
		      double & part_expVal, 
		      double & part_var, 
                      double & part_relv,
		      double & part_ratio){
	std::cout << ",";
	std::cout << numparts << " , ";
	std::cout << edgecut << " , ";
	std::cout << rel_edgecut << "% " << " , ";
	std::cout << part_relExpVal << " , ";
	std::cout << part_expVal << " , ";
	std::cout << part_var << " , ";
	std::cout << part_relv << " , ";
	std::cout << part_ratio << " , "; 
	std::cout <<  " , "; 
}

void cover_results(double & coverTime, int & numcovers, 
		  double & cov_relExpVal, double & cov_expVal, 
		  double & cov_var, double & cov_relv,
		  double & intersection, double & cover_rat, 
		  double & inter_ratio){
	std::cout <<  cov_relExpVal << " , ";
	std::cout <<  cov_expVal << " , ";
	std::cout <<  cov_var << " , ";	
	std::cout <<  cov_relv << " , ";
	std::cout << cover_rat << " , ";	
	std::cout <<  intersection*100 << "%" << " , ";
	std::cout << std::endl;
}
/**
 * Cover size distributions
 * Cover intersection distributions
 */
template < typename Complex, typename Stat>
void generate_cover_stats(Complex & complex, Stat & intersect,Stat & coverDist,
					double & intersection, int & numcovers){
	typedef typename Complex::iterator cell_it;
	intersection= 0;	
	int cross_sheet = numcovers-1;
	for (cell_it it = complex.begin(); it !=complex.end(); ++it){
		int partition_set = it->second.partition_set;
		bool crossing = it->second.crossing;
		coverDist[partition_set]++;
		if (partition_set != cross_sheet && crossing){
			 coverDist[cross_sheet]++;
			 intersection += 1.0;
			 if(intersect[partition_set]  == 0){
				intersect[partition_set] += 1.0;
		 	 }else{
				intersect[partition_set] = 1.0;
			 }
		}
	}
	intersection /= complex.size();
}

/**
 * Partition distibution
 **/

template < typename Complex, typename Stat> 
void generate_partition_stats(Complex & complex, Stat & partDist, 
					int & num_vertices, int & num_edges){	
	typedef typename Complex::iterator cell_it;	
	num_vertices=0;
	num_edges=0;
	for (cell_it it = complex.begin(); it != complex.end(); ++it){
		if(it->first.dimension() == 0){
			/**
			 * Since sets are sorted and each vertex
			 * must be in a partition (whose number is lower than
			 * an intersection, the first element always points to
			 * the partition the vertex is in.
			 */ 
			partDist[it->second.partition_set] += 1.0; 
			num_vertices++;
		}
		else if(it->first.dimension()==1){
		num_edges++;
		}
	}
}

template<typename Str, typename Complex>
void print_stats_preamble(Str & filename, Complex & complex, int & num_vertices,
		     double & partTime, double & coverTime, double & wholeTime, 
		     int & num_edges){
        std::cout << filename << " , ";
        std::cout << complex.size() << " , ";
        std::cout << num_vertices << " ,";
        std::cout << partTime << " ," ;
        std::cout << coverTime << " , ";
        std::cout << wholeTime << " , ";
        std::cout << num_edges << " , ";
        int v = num_vertices;
        std::cout << ((double)num_edges/(double)(v*(v-1))*100) << "% , ";
}
 
		
template<typename Str, typename Complex>
void generate_stats(Str & filename, Complex & complex,
				   int & numparts, int & numcovers,
				   int & edgecut, double & partTime,
				   double & coverTime, double & wholeTime){
	//typedef typename Complex::iterator cell_it;
	//typedef typename std::set<int> coverType; //need more genericity for
	//typedef typename coverType::iterator covIt;
	std::vector< double > partDist(numparts);
	std::vector< double > coverDist(numcovers);
	std::vector< double > intersect(numparts);
	
	int num_vertices,num_edges;
	double intersection;
        double cov_relExpVal = 0, cov_expVal = 0, cov_var=999, cov_relv=0, 
								cov_rat=0;
        double intr_relExpVal = 0, intr_expVal = 0, intr_var=999,intr_relv=0, 
								 intr_rat=0;
        double part_relExpVal = 0, part_expVal = 0, part_var=999,part_relv=0, 
								 part_rat=0;
	for (int i = 0; i < numcovers; ++i){
		 coverDist[i]=0;		
	}
	for (int i = 0; i < numparts; ++i){ 
		partDist[i]=0;	
		intersect[i]=0;	
	}

	generate_partition_stats(complex,partDist,num_vertices,num_edges);
	generate_cover_stats(complex,intersect,coverDist,
						intersection,numcovers);

	make_prob(coverDist,numcovers);
	make_prob(partDist,numparts);
	make_prob(intersect,numparts);
	stats(coverDist,numcovers,cov_var,cov_relv,cov_expVal,cov_relExpVal,
								cov_rat);
	stats(partDist,numparts,part_var,part_relv, part_expVal,
						part_relExpVal,part_rat);
	stats(intersect,numparts,intr_var,intr_relv,
					intr_expVal,intr_relExpVal,intr_rat);
	double rel_edgecut = ((double) edgecut / (double) num_edges)*100.0;
	print_header(complex);
	print_stats_preamble(filename, complex,num_vertices,partTime, coverTime,
							 wholeTime, num_edges);
        partition_results(numparts,edgecut,rel_edgecut,part_relExpVal,
				part_expVal,part_var,part_relv,part_rat);
	cover_results(coverTime, numcovers,cov_relExpVal,cov_expVal,
					cov_var,cov_relv,intersection,
							cov_rat,intr_rat);
} 
} //end namespace parallel
} //end namespace ctl
#endif
