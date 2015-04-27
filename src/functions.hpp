/* ---------------------------------------------------------------------- *
 * functions.hpp
 * ---------------------------------------------------------------------- */

 #ifndef __FG_FUNCTIONS__
 #define __FG_FUNCTIONS__

 #include <vector>

 typedef std::vector<std::vector<int> > Graph;

// computes the eccentricity of the vertex with the specified index in the given graph
 int vertex_eccentricity(Graph& graph, int index);

// computes the diameter of the specified graph
 int graph_diameter(Graph& graph);

 #endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */