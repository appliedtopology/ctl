// test_clique_output_iterator.C
// Gabe Weaver
// August 19, 2008

#include <set>
#include "clique_output_iterator.h"

int main(int argc, char *argv[])
{
  typedef std::set<int> Clique;
  typedef std::set< Clique > CliqueSet;

  CliqueSet cliques;
  CliqueSet::iterator clique_iter;

  int c1[4] = { 22, 33, 44, 55 };
  int c2[6] = { 5, 10, 15, 20, 25, 30 };

  Clique clique1(c1, c1 + 4);
  Clique clique2(c2, c2 + 6);

  cliques.insert(clique1);
  cliques.insert(clique2);

  ctl::Clique_output_iterator<Clique> out; 

  for (clique_iter = cliques.begin();
       clique_iter != cliques.end();
       clique_iter++) {
    Clique clique = (Clique)(*clique_iter);
    *out++ = clique;
  }
  
}
