# Flipgraph

Flipgraph is a program for generation of flip graphs. It is part of my Master's thesis.

## Generating a Flip Graph

The following code snippet shows how to compute the flip graph for triangulations with `n` vertices.

```
vector<vector<int> > flip_graph;
compute_flip_graph(n, flip_graph);
```

## Working with Triangulations

Create a canonical triangulation with `n` vertices.
```
Triangulation triangulation(n);
```

Flip the edge `edge` of the triangulation `triangualtion`.
```
if (triangulation.is_flippable(edge)) {
  triangulation.flip(edge);
}
```

Checking whether two triangualtions `triangualtion_a` and `triangulation_b` are isomorphic.
```
Code code_a(triangulation_a);
Code code_b(triangulation_b);
bool isomorphic = (code_a == code_b);
```

## What are Triangulations, Edge Flips and Flip Graphs?

A maximal planar graph is called a *triangulation* since all of its faces are triangles.
We consider abstract unlabeled triangulations, that is, we do not distinguish between triangulations in the same isomorphism class.
Any edge *{u,v}* in a triangulation *T* has two incident triangles, say *uva* and *vub*.
A *flip* operation consists of removing the edge *{u,v}* and then inserting the edge *{a,b}* instead.
However, the edge *{u,v}* is *flippable* only if the edge *{a,b}* is not already present in *T*.
The induced *flip graph* *F = (V,E)* is the graph where *V* is the set of all triangulations on *n* vertices.
Moreover, for all triangulations *T* and *T'*, the edge *{T,T'}* is contained in *E* if and only if *T* can be transformed into *T'* by a single flip.
