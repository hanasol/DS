#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>
#include <stack>

using namespace std;

#define MAX 999999

class Graph {
protected:
	bool m_Type; // 0:List 1:Matrix
	int m_Size;

public:
	Graph(bool type, int size);
	virtual ~Graph(); // To call the destructor according to the list format, virtual destructor declaration

	bool getType();
	int getSize();

	virtual void getundirectedEdges(int vertex, map<int, int>* m) = 0;
	virtual void getAdjacentEdges(int vertex, map<int, int>* m) = 0;
	virtual void insertEdge(int from, int to, int weight) = 0;
	virtual	bool printGraph(ofstream& fout) = 0;
};

#endif