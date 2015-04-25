#include "fg/flipgraph.hpp"

#include <iostream>

int main() {
    int n = 10;

    std::vector<std::vector<int> > graph;
    compute_flip_graph(n, graph);

    int size = (int) graph.size();
    for (int i = 0; i < size; ++i) {
        int degree = (int) graph[i].size();
        std::cout << i << ":";
        for (int j = 0; j < degree; ++j) {
            std::cout << " " << graph[i][j];
        }
        std::cout << std::endl;
    }

    return 0;
}