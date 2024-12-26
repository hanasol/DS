#ifndef _LIST_H_
#define _LIST_H_

#include "Graph.h"

class ListGraph : public Graph {
private:
    map<int, int>* m_List; // Adjacency list to store edges

public:
    ListGraph(bool type, int size); // Constructor to initialize the graph
    ~ListGraph(); // Destructor to free allocated resources

    void getundirectedEdges(int vertex, map<int, int>* m); // Store information about undirected edges
    void getAdjacentEdges(int vertex, map<int, int>* m); // Store information about directed edges
    void insertEdge(int from, int to, int weight); // Insert an edge with a specified weight
    bool printGraph(ofstream& fout); // Print the graph structure
};

#endif
