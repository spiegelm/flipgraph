/* ---------------------------------------------------------------------- *
 * flipgraph.hpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

#ifndef __FG_FLIP_GRAPH__
#define __FG_FLIP_GRAPH__

#include <vector>
#include <queue>
#include <map>
#include <utility>

// computes the flip graph on all triangulations with n vertices.
void compute_flip_graph(int n, std::vector<std::vector<int> >& graph);

#endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */