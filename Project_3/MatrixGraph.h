#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Graph.h"

class MatrixGraph : public Graph {
private:
	int** m_Mat;

public:
	MatrixGraph(bool type, int size);
	~MatrixGraph();

	void getundirectedEdges(int vertex, map<int, int>* m); // Store undirected edge information
	void getAdjacentEdges(int vertex, map<int, int>* m); // Store edge information with direction
	void insertEdge(int from, int to, int weight);
	bool printGraph(ofstream& fout);
};

#endif