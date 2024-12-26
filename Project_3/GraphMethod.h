#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"

bool BFS(Graph* graph, int vertex, ofstream& fout); // Perform Breadth-First Search starting from the given vertex
bool DFS(Graph* graph, int vertex, ofstream& fout); // Perform Depth-First Search starting from the given vertex
void DFS_R(Graph* graph, vector<bool>& visit, int vertex, ofstream& fout); // Recursive helper function for Depth-First Search

void insertion_sort(vector<pair<int, pair<int, int>>>& edges, int left, int right, int pivot); // Perform insertion sort for edges
int partition(vector<pair<int, pair<int, int>>>& edges, int left, int right); // Partition function for quicksort
void kruskal_sort(vector<pair<int, pair<int, int>>>& edges, int left, int right); // Sort edges using quicksort for Kruskal's algorithm
bool Kruskal(Graph* graph, ofstream& fout); // Implement Kruskal's algorithm for Minimum Spanning Tree
int getParent(int* mst, int vertex); // Return the root of the set containing the given node
void Union(int* mst, int ver1, int ver2); // Connect two nodes into the same set
bool isCycle(int* mst, int ver1, int ver2); // Determine whether two nodes form a cycle

int length(Graph* graph, int v, int u); // Return the weight of the edge from vertex v to vertex u
int choose(int* dist, bool* s, int size, int vertex); // Return the unvisited vertex with the smallest distance
bool Dijkstra(Graph* graph, int vertex, ofstream& fout); // Perform Dijkstra's algorithm for shortest path

bool Bellmanford(Graph* graph, int s_vertex, int e_vertex, ofstream& fout); // Perform Bellman-Ford algorithm for shortest path
bool FLOYD(Graph* graph, ofstream& fout); // Perform Floyd-Warshall algorithm for all-pairs shortest paths

#endif
