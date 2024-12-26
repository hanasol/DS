#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_Mat = new int* [size];
	for (int i = 0; i < size; i++)
	{
		m_Mat[i] = new int[size];
		memset(m_Mat[i], 0, sizeof(int) * size); // Initialize the matrix with zeros
	}
}

MatrixGraph::~MatrixGraph()
{
	for (int i = 0; i < getSize(); i++)
	{
		delete[] m_Mat[i]; // Release the memory allocated for each row
	}
	delete[] m_Mat; // Release the memory allocated for the matrix
}

void MatrixGraph::getundirectedEdges(int vertex, map<int, int>* m) {
	for (int i = 0; i < m_Size; i++) {
		for (int j = 0; j < m_Size; j++) {
			if (i == j) // Skip self-loops
				continue;
			if (i == vertex && m_Mat[i][j] != 0) // Outgoing edges from the vertex
				m->insert(make_pair(j, m_Mat[i][j]));
			if (j == vertex && m_Mat[i][j] != 0) // Incoming edges to the vertex
				m->insert(make_pair(i, m_Mat[i][j]));
		}
	}
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	for (int j = 0; j < m_Size; j++) {
		if (m_Mat[vertex][j] != 0) { // If there is an edge connected to the vertex
			m->insert(make_pair(j, m_Mat[vertex][j])); // Store the edge's vertex and weight
		}
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)
{
	m_Mat[from][to] = weight; // Insert an edge with the specified weight
}

bool MatrixGraph::printGraph(ofstream& fout)
{
	if (m_Size < 0)
		return 0;

	fout << "======== PRINT ========\n" << '\t';
	for (int i = 0; i < m_Size; i++)
	{
		fout << "[" << i << "]" << '\t'; // Print column headers
	}
	fout << endl;

	for (int i = 0; i < m_Size; i++)
	{
		fout << "[" << i << "]"; // Print row headers
		for (int j = 0; j < m_Size && fout << '\t'; j++)
		{
			fout << m_Mat[i][j]; // Print the adjacency matrix values
		}
		fout << endl;
	}
	fout << "=======================\n\n";
	return 1;
}
