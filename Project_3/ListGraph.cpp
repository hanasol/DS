#include "ListGraph.h"

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_Type = type;
	m_List = new map<int, int>[size]; // Initialize an array of maps to store adjacency lists
}

ListGraph::~ListGraph()
{
	delete[] m_List; // Release the memory allocated for the adjacency list
}

void ListGraph::getundirectedEdges(int vertex, map<int, int>* m) {
	map<int, int>::iterator iter;
	for (int i = 0; i < m_Size; i++) {
		for (iter = m_List[i].begin(); iter != m_List[i].end(); iter++) {
			if (i == vertex) { // Outgoing edges from the vertex
				m->insert(make_pair((*iter).first, (*iter).second));
			}
			else if (iter->first == vertex) { // Incoming edges to the vertex
				m->insert(make_pair(i, (*iter).second));
			}
		}
	}
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	map<int, int>::iterator iter;
	for (iter = m_List[vertex].begin(); iter != m_List[vertex].end(); iter++)
		m->insert(make_pair((*iter).first, (*iter).second)); // Store information about vertices and weights connected to the given vertex
}

void ListGraph::insertEdge(int from, int to, int weight)
{
	m_List[from].insert(make_pair(to, weight)); // Insert an edge with the specified weight into the adjacency list
}

bool ListGraph::printGraph(ofstream& fout)
{
	fout << "======== PRINT ========\n";
	for (int i = 0; i < m_Size; i++)
	{
		fout << "[" << i << "]";

		for (auto it_ = m_List[i].begin(); it_ != m_List[i].end() && fout << " -> "; it_++)
		{
			fout << "(" << it_->first << "," << it_->second << ")"; // Print connected vertices and their weights
		}
		fout << endl;
	}
	fout << "=======================\n\n";
	return 1;
}
