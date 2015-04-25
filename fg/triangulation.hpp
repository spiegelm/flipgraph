/* ---------------------------------------------------------------------- *
 * triangulation.hpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

#ifndef __FG_TRIANGULATION__
#define __FG_TRIANGULATION__

#include <vector>
#include <iostream>

/* ---------------------------------------------------------------------- *
 * forward declarations
 * ---------------------------------------------------------------------- */

class Vertex;
class Edge;
class Triangulation;
class Code;

/* ---------------------------------------------------------------------- *
 * declaration of the vertex class
 * ---------------------------------------------------------------------- */

 class Vertex {
    // the label
    int label_;

    // an incident edge
    Edge* edge_;

public:
    // sets this vertex's label to the specified label
    void set_label(int label);

    // makes the specified edge incident to this vertex
    void set_edge(Edge* edge_);

    // returns this vertex's label
    int label() const;

    // returns an edge incident to this vertex
    Edge* edge() const;
 };

/* ---------------------------------------------------------------------- *
 * declaration of the edge class
 * ---------------------------------------------------------------------- */ 

class Edge {
    // the target vertex
    Vertex* target_;

    // the twin edge
    Edge* twin_;

    // the previous edge of the incident face
    Edge* prev_;

    // the next edge of the incident face
    Edge* next_;

public:
    // sets the target vertex to the specified vertex
    void set_target(Vertex* vertex);

    // sets the twin edge to the specified edge
    void set_twin(Edge* edge);

    // sets the next edge to the specified edge
    void set_prev(Edge* edge);

    // sets the next edge to the specified edge
    void set_next(Edge* edge);

    // returns the target vertex
    Vertex* target() const;

    // returns the twin edge
    Edge* twin() const;

    // returns the previous edge of the incident face
    Edge* prev() const;

    // returns the next edge of the incident face
    Edge* next() const;
};

/* ---------------------------------------------------------------------- *
 * declaration of the triangulation class
 * ---------------------------------------------------------------------- */

class Triangulation {
public:
    // constructor that builds a canonical triangulation with n vertices
    Triangulation(int n);

    // constructor that builds a triangulation from the specified code
    Triangulation(const Code& code);

    // constructor that copies the specified triangulation
    Triangulation(const Triangulation& triangulation);

    // destructor
    ~Triangulation();

private:
    // the list of vertices
    std::vector<Vertex*> vertices_;

    // the list of edges
    std::vector<Edge*> edges_;

    // creates and returns a new vertex
    Vertex* new_vertex();

    // creates and returns a new edge
    Edge* new_edge();

    // makes the two specified edges twins
    void make_twins(Edge* edge_a, Edge* edge_b);

    // makes the two specified edges consecutive
    void make_consecutive(Edge* edge_a, Edge* edge_b);

    // builds a triangle with the three specified edges
    void make_triangle(Edge* edge_a, Edge* edge_b, Edge* edge_c);

    // builds a triangle with the three specified edges and vertices
    void make_triangle(Edge* edge_a, Edge* edge_b, Edge* edge_c,
                       Vertex* vertex_a, Vertex* vertex_b, Vertex* vertex_c);

    // performs an e3-expansion at the specified edge
    void expand_three(Edge* e);

    // builds a canonical triangulation with n vertices
    void make_canonical(int n);

    // builds a triangulation form the specified code
    void build_from_code(const Code& code);

    // builds a copy of the specified triangulation
    void copy(const Triangulation& triangulation);

    // removes all vertices and edges from the triangulation
    void clear();

public:
    // returns the number of vertices
    int order() const;

    // returns the number of edges
    int size() const;

    // returns the i-th vertex
    Vertex* vertex(int i) const;

    // returns the i-th edge
    Edge* edge(int i) const;

    // returns the edge from the first specified vertex to the second
    Edge* edge(Vertex* vertex_a, Vertex* vertex_b) const;

    // returns whether the specified edge or its twin is representative
    bool is_representative(Edge* edge);

    // returns whether the specified edge is flippable or not
    bool is_flippable(Edge* edge);

    // flips the specified edge
    void flip(Edge* edge);
};

/* ---------------------------------------------------------------------- *
 * declaration of the code class
 * ---------------------------------------------------------------------- */

class Code {
public:
    // constructor that computes code from the specified triangulation
    Code(const Triangulation& triangulation);

    // constructor that computes code from specified triangulation starting
    // at the specified edge
    Code(const Triangulation& triangulation, Edge* edge);

    // constructor that copies the specified code
    Code(const Code& code);

    // destructor
    ~Code();

private:
    // the char array representing the actual code
    unsigned char* code_;

    // the length of this code
    int length_;

    // initializes this code for the specified triangulation
    void initialize(const Triangulation& triangulation);

    // updates the this code for the specified triangulation starting at
    // the specified edge and using the specified orientation.
    // the code is updated if the new code is lexicographically smaller
    void update(const Triangulation& triangulation, Edge* edge, bool clockwise);

    // computes the code for the specified triangulation
    void compute_code(const Triangulation& triangulation);

    // computes the code for the specified triangulation starting at the
    // specified edge
    void compute_code(const Triangulation& triangulation, Edge* edge);

public:
    // sets the i-th symbol of the code
    void set_symbol(int i, unsigned char symbol);

    // returns the i-th symbol of the code
    unsigned char symbol(int i) const;

    // returns the lenghh of the code
    int length() const;

    // returns whether the code is equal to the specified code or not
    bool operator ==(const Code& other) const;

    // returns whether the code is not equal to the specified code or not
    bool operator !=(const Code& other) const;

    // returns whether the code lexicographically smaller than the specified
    // code or not
    bool operator <(const Code& other) const;

    // returns whether the code is lexicographically smaller than or
    // equal to the specified code or not
    bool operator <=(const Code& other) const;

    // returns whether the code is lexicographically greater than the specified
    // code or not
    bool operator >(const Code& other) const;

    // returns whether the code is lexicographically greater than or
    // equal to the specified code or not
    bool operator >=(const Code& other) const;
};

/* ---------------------------------------------------------------------- *
 * function declarations
 * ---------------------------------------------------------------------- */

// writes the specified triangulation to the specified stream
void write_triangulation(Triangulation& triangulation, std::ostream& output_stream);

// writes the specified code to the specified stream
void write_code_alpha(Code& code, std::ostream& output_stream);

#endif

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */