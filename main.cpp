#include "fg/options.hpp"
#include "fg/flipgraph.hpp"

#include <vector>
#include <iostream>
#include <fstream>

const int default_n = 4;

int main(int argc, char* argv[]) {
    // option -n: number of vertices
    char* option_n = get_cmd_option(argc, argv, "-n");
    int n = (option_n) ? std::stoi(option_n) : default_n;
    
    // option -o: output file
    bool file_output = false;
    std::ofstream file_stream;
    char* option_o = get_cmd_option(argc, argv, "-o");
    if (option_o) {
        file_output = true;
        file_stream.open(option_o);
    }
    std::ostream& output_stream = file_output ? file_stream : std::cout;

    std::vector<std::vector<int> > graph;
    compute_flip_graph(n, graph);
    write_flip_graph(graph, output_stream);

    // close output stream
    if (file_output) { file_stream.close(); }

    return 0;
}