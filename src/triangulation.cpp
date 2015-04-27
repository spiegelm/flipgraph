/* ---------------------------------------------------------------------- *
 * triangulation.cpp
 *
 * author: jerome dohrau
 * ---------------------------------------------------------------------- */

#include "triangulation.hpp"

#include <queue>
#include <map>

/* ---------------------------------------------------------------------- *
 * implementation of the vertex class
 * ---------------------------------------------------------------------- */

void Vertex::set_label(int label) {
    label_ = label;
}

void Vertex::set_edge(Edge* edge) {
    edge_ = edge;
}

int Vertex::label() const {
    return label_;
}

Edge* Vertex::edge() const {
    return edge_;
}

/* ---------------------------------------------------------------------- *
 * implementation of the edge class
 * ---------------------------------------------------------------------- */

void Edge::set_target(Vertex* vertex) {
    target_ = vertex;
}

void Edge::set_twin(Edge* edge) {
    twin_ = edge;
}

void Edge::set_prev(Edge* edge) {
    prev_ = edge;
}

void Edge::set_next(Edge* edge) {
    next_ = edge;
}

Vertex* Edge::target() const {
    return target_;
}

Edge* Edge::twin() const {
    return twin_;
}

Edge* Edge::prev() const {
    return prev_;
}

Edge* Edge::next() const {
    return next_;
}

/* ---------------------------------------------------------------------- *
 * implementation of the triangulation class
 * ---------------------------------------------------------------------- */

Triangulation::Triangulation(int n) {
    make_canonical(n);
}

Triangulation::Triangulation(const Code& code) {
    build_from_code(code);
}

Triangulation::Triangulation(const Triangulation& triangulation) {
    copy(triangulation);
}

Triangulation::~Triangulation() {
    clear();
}

Vertex* Triangulation::new_vertex() {
    Vertex* vertex = new Vertex();
    vertices_.push_back(vertex);
    return vertex;
}

Edge* Triangulation::new_edge() {
    Edge* edge = new Edge();
    edges_.push_back(edge);
    return edge;
}

void Triangulation::make_twins(Edge* edge_a, Edge* edge_b) {
    edge_a->set_twin(edge_b);
    edge_b->set_twin(edge_a);
}

void Triangulation::make_consecutive(Edge* edge_a, Edge* edge_b) {
    edge_a->set_next(edge_b);
    edge_b->set_prev(edge_a);
}

void Triangulation::make_triangle(Edge* edge_a, Edge* edge_b, Edge* edge_c) {
    make_consecutive(edge_a, edge_b);
    make_consecutive(edge_b, edge_c);
    make_consecutive(edge_c, edge_a);
}

void Triangulation::make_triangle(Edge* edge_a, Edge* edge_b, Edge* edge_c,
                                  Vertex* vertex_a, Vertex* vertex_b, Vertex* vertex_c) {
    make_triangle(edge_a, edge_b, edge_c);
    vertex_a->set_edge(edge_c);
    vertex_b->set_edge(edge_a);
    vertex_c->set_edge(edge_b);
    edge_a->set_target(vertex_c);
    edge_b->set_target(vertex_a);
    edge_c->set_target(vertex_b);
}

void Triangulation::expand_three(Edge* edge) {
    Edge* edge_ab = edge;
    Edge* edge_bc = edge->next();
    Edge* edge_ca = edge->prev();
    Vertex* vertex_a = edge_ca->target();
    Vertex* vertex_b = edge_ab->target();
    Vertex* vertex_c = edge_bc->target();
    Vertex* vertex_d = new_vertex();
    
    make_triangle(edge_ab, new_edge(), new_edge(), vertex_d, vertex_a, vertex_b);
    make_triangle(edge_bc, new_edge(), new_edge(), vertex_d, vertex_b, vertex_c);
    make_triangle(edge_ca, new_edge(), new_edge(), vertex_d, vertex_c, vertex_a);
    make_twins(edge_ab->next(), edge_bc->prev());
    make_twins(edge_bc->next(), edge_ca->prev());
    make_twins(edge_ca->next(), edge_ab->prev());
}

void Triangulation::make_canonical(int n) {
    Vertex* vertex_a = new_vertex();
    Vertex* vertex_b = new_vertex();
    Vertex* vertex_c = new_vertex();
    Edge* edge_ab = new_edge();
    Edge* edge_bc = new_edge();
    Edge* edge_ca = new_edge();
    Edge* edge_ba = new_edge();
    Edge* edge_cb = new_edge();
    Edge* edge_ac = new_edge();
    
    // build first triangle
    make_triangle(edge_ab, edge_bc, edge_ca, vertex_c, vertex_a, vertex_b);
    make_triangle(edge_ac, edge_cb, edge_ba, vertex_b, vertex_a, vertex_c);
    make_twins(edge_ab, edge_ba);
    make_twins(edge_bc, edge_cb);
    make_twins(edge_ca, edge_ac);
    
    // apply e3-expansion n-3 times
    for (int i = 3; i < n; ++i) { expand_three(edge_ab); }
}

void Triangulation::build_from_code(const Code& code) {
    int l = code.length();
    int n = (int) code.symbol(0);
    
    for (int i = 0; i < n; ++i) { new_vertex(); }
    
    int index = 0;
    for (int i = 0; i < n; ++i) {
        Vertex* vertex_a = vertex(i);
        
        Edge* first = nullptr;
        Edge* last = nullptr;
        
        while (code.symbol(++index)) {
            int j = (int) (code.symbol(index) - 1);
            Vertex* vertex_b = vertex(j);
            
            Edge* current;
            if (i < j) {
                // create new edge
                current = new_edge();
                current->set_target(vertex_b);
                make_twins(current, new_edge());
                current->twin()->set_target(vertex_a);
            } else {
                // get existing edge
                current = edge(vertex_b, vertex_a)->twin();
            }
            
            if (first == nullptr) { first = current; }
            if (last != nullptr) { make_consecutive(last, current); }
            last = current->twin();
        }
        
        make_consecutive(last, first);
        vertex_a->set_edge(first);
    }
}

void Triangulation::copy(const Triangulation& triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();
    std::map<Vertex*, Vertex*> vertex_map;
    std::map<Edge*, Edge*> edge_map;
    
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        Vertex* copy = new_vertex();
        vertex_map[vertex] = copy;
    }
    
    for (int i = 0; i < m; ++i) {
        Edge* edge = triangulation.edge(i);
        Edge* copy = new_edge();
        edge_map[edge] = copy;
    }
    
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        Vertex* copy = vertex_map[vertex];
        copy->set_label(vertex->label());
        copy->set_edge(edge_map[vertex->edge()]);
    }
    
    for (int i = 0; i < m; ++i) {
        Edge* edge = triangulation.edge(i);
        Edge* copy = edge_map[edge];
        copy->set_target(vertex_map[edge->target()]);
        copy->set_twin(edge_map[edge->twin()]);
        copy->set_prev(edge_map[edge->prev()]);
        copy->set_next(edge_map[edge->next()]);
    }
}

void Triangulation::clear() {
    int n = (int) vertices_.size();;
    int m = (int) edges_.size();
    for (int i = 0; i < n; ++i) { delete vertices_[i]; }
    for (int j = 0; j < m; ++j) { delete edges_[j]; }
    edges_.clear();
    vertices_.clear();
}

int Triangulation::order() const {
    return (int) vertices_.size();
}

int Triangulation::size() const {
    return (int) edges_.size();
}

Vertex* Triangulation::vertex(int i) const {
    return vertices_[i];
}

Edge* Triangulation::edge(int i) const {
    return edges_[i];
}

Edge* Triangulation::edge(Vertex* vertex_a, Vertex* vertex_b) const {
    Edge* first = vertex_a->edge();
    Edge* current = first;
    do {
        if (current->target() == vertex_b) { return current; }
        current = current->twin()->next();
    } while (current != first);
    return nullptr;
}

bool Triangulation::is_representative(Edge* edge) {
    return edge < edge->twin();
}

bool Triangulation::is_flippable(Edge* edge) {
    Vertex* vertex_a = edge->next()->target();
    Vertex* vertex_b = edge->twin()->next()->target();
    Edge* first = vertex_a->edge();
    Edge* current = first;
    
    // check whether vertex_a and vertex_b are adjacent
    do {
        if (current->target() == vertex_b) { return false; }
        current = current->twin()->next();
    } while (current != first);
    return true;
}

void Triangulation::flip(Edge* edge) {
    Edge* twin = edge->twin();
    Vertex* vertex_t = edge->target();
    Vertex* vertex_s =twin->target();
    Vertex* vertex_a = edge->next()->target();
    Vertex* vertex_b = twin->next()->target();
    Edge* edge_ta = edge->next();
    Edge* edge_as = edge->prev();
    Edge* edge_sb = twin->next();
    Edge* edge_bt = twin->prev();
    
    make_triangle(edge, edge_bt, edge_ta);
    make_triangle(twin, edge_as, edge_sb);
    edge->set_target(vertex_b);
    twin->set_target(vertex_a);
    if (vertex_t->edge() == twin) { vertex_t->set_edge(edge_ta); }
    if (vertex_s->edge() == edge) { vertex_s->set_edge(edge_sb); }
}

/* ---------------------------------------------------------------------- *
 * impementation of the code class
 * ---------------------------------------------------------------------- */

Code::Code(const Triangulation& triangulation) {
    compute_code(triangulation);
}

Code::Code(const Triangulation& triangulation, Edge* edge) {
    compute_code(triangulation, edge);
}

Code::Code(const Code& code) {
    length_ = code.length_;
    code_ = new unsigned char[length_];
    for (int i = 0; i < length_; ++i) { code_[i] = code.symbol(i); }
}

Code::~Code() {
    delete[] code_;
}

void Code::initialize(const Triangulation& triangulation) {
    int n = triangulation.order();
    int m = triangulation.size();
    length_ = n + m + 1;
    code_ = new unsigned char[length_];
    for (int i = 0; i < length_; ++i) { code_[i] = n; }
}

void Code::update(const Triangulation& triangulation, Edge* edge, bool clockwise) {
    int n = triangulation.order();
    for (int i = 0; i < n; ++i) {
        Vertex* vertex = triangulation.vertex(i);
        vertex->set_label(0);
    }

    int index = 1;
    int label = 1;
    bool smaller = false;
    std::queue<Edge*> queue;

    // breadth first search
    edge->target()->set_label(label++);
    queue.push(edge);
    while (!queue.empty()) {
        Edge* first = queue.front()->twin();
        Edge* current = first;
        queue.pop();

        do {
            Vertex* vertex = current->target();
            if (vertex->label() == 0) {
                vertex->set_label(label++);
                queue.push(current);
            }

            int symbol = vertex->label();
            if (smaller) {
                code_[index] = symbol;
            } else if (symbol < code_[index]) {
                smaller = true;
                code_[index] = symbol;
            } else if (symbol > code_[index]) {
                return;
            }

            index++;
            current = clockwise ? current->twin()->next() : current->prev()->twin();
        } while (current != first);

        // mark end of current vertex's phase
        if (code_[index] > 0) { smaller = true; }
        code_[index++] = 0;
    }
}

void Code::compute_code(const Triangulation& triangulation) {
    initialize(triangulation);
    int m = triangulation.size();
    for (int i = 0; i < m; ++i) {
        Edge* edge = triangulation.edge(i);
        update(triangulation, edge, true);
        update(triangulation, edge, false);
    }
}

void Code::compute_code(const Triangulation& triangulation, Edge* edge) {
    initialize(triangulation);
    Edge* twin = edge->twin();
    update(triangulation, edge, true);
    update(triangulation, edge, false);
    update(triangulation, twin, true);
    update(triangulation, twin, false);
}

void Code::set_symbol(int i, unsigned char symbol) {
    code_[i] = symbol;
}

unsigned char Code::symbol(int i) const {
    return code_[i];
}

int Code::length() const {
    return length_;
}

bool Code::operator ==(const Code& other) const {
    if (length_ != other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] != other.code_[i]) { return false; }
    }
    return true;
}

bool Code::operator !=(const Code& other) const {
    return !(*this == other);
}

bool Code::operator <(const Code& other) const {
    if (length_ < other.length_) { return true; }
    if (length_ > other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] < other.code_[i]) { return true; }
        if (code_[i] > other.code_[i]) { return false; }
    }
    return false;
}

bool Code::operator <=(const Code& other) const {
    if (length_ < other.length_) { return true; }
    if (length_ > other.length_) { return false; }
    for (int i = 0; i < length_; ++i) {
        if (code_[i] < other.code_[i]) { return true; }
        if (code_[i] > other.code_[i]) { return false; }
    }
    return true;
}

bool Code::operator >(const Code& other) const {
    return other < *this;
}

bool Code::operator >=(const Code& other) const {
    return other <= *this;
}

/* ---------------------------------------------------------------------- *
 * end of file
 * ---------------------------------------------------------------------- */