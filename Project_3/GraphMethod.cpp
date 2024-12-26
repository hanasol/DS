#include "GraphMethod.h"

bool BFS(Graph* graph, int vertex, ofstream& fout)
{
	if (vertex < 0 || vertex > graph->getSize()) // Vertex does not exist in the graph
		return false;
	queue<int> visit; // Queue to store vertices
	bool* visit_node = new bool[MAX + 1]; // Array to check if a vertex is visited
	for (int i = 0; i < MAX + 1; i++)
		visit_node[i] = false;
	map<int, int>::iterator iter;
	visit.push(vertex);
	visit_node[vertex] = true;
	fout << "======== BFS ========\n";
	fout << "startvertex: " << vertex << "\n";
	while (!visit.empty()) {
		map<int, int> edge;
		int v = visit.front();
		visit.pop();
		fout << v;
		graph->getundirectedEdges(v, &edge); // Store information about edges connected to vertex `v` bidirectionally
		for (iter = edge.begin(); iter != edge.end(); iter++) {
			if (!visit_node[iter->first]) { // If the vertex has not been visited
				visit.push(iter->first);
				visit_node[iter->first] = true;
			}
		}
		if (!visit.empty()) // If there are vertices to explore next
			fout << " -> ";
	}
	fout << "\n=====================\n\n";
	delete[] visit_node;
	return true;
}

bool DFS(Graph* graph, int vertex, ofstream& fout)
{
	if (vertex < 0 || vertex >= graph->getSize()) // Vertex does not exist in the graph
		return false;
	int cnt = 0;
	stack<int> visit; // Stack for DFS
	bool* visit_node = new bool[MAX + 1]; // Array to check if a vertex is visited
	for (int i = 0; i < MAX + 1; i++)
		visit_node[i] = false;
	map<int, int>::reverse_iterator iter;
	visit.push(vertex);

	fout << "======== DFS ========\n";
	fout << "startvertex: " << vertex << "\n";

	while (!visit.empty()) {
		map<int, int> edge; // Stores edge information for the current node
		int v = visit.top(); // Get the top vertex from the stack
		visit.pop();
		graph->getundirectedEdges(v, &edge); // Store bidirectional information for the current node

		if (visit_node[v])
			continue;
		fout << v;
		cnt++;
		visit_node[v] = true;
		for (iter = edge.rbegin(); iter != edge.rend(); iter++) { // Push vertices in reverse order for smaller numbers first
			if (!visit_node[iter->first]) {
				visit.push(iter->first);
			}
		}
		if (!visit.empty() && !visit_node[visit.top()]) // If the node you want to explore exists and you haven't visited it
			fout << " -> ";
	}

	fout << "\n=====================\n\n";
	delete[] visit_node;
	return true;
}

void DFS_R(Graph* graph, vector<bool>& visit, int vertex, ofstream& fout)
{
	if (visit[vertex]) // If the vertex is already visited
		return;
	visit[vertex] = true; // Mark the current node as visited
	fout << vertex;
	map<int, int>::iterator iter;
	map<int, int> edge;
	graph->getundirectedEdges(vertex, &edge); // Store bidirectional information for the current node

	for (iter = edge.begin(); iter != edge.end(); iter++) {
		if (!visit[iter->first]) { // If the vertex is not visited
			fout << " -> "; // Indicate the next node to be visited
			DFS_R(graph, visit, iter->first, fout); // Recursively call DFS on the node
		}
	}
}

int getParent(int* mst, int vertex) {
	if (mst[vertex] == vertex)
		return vertex; // If the parent node is itself
	return getParent(mst, mst[vertex]);
}

void Union(int* mst, int ver1, int ver2) {
	int root1 = getParent(mst, ver1);
	int root2 = getParent(mst, ver2); // Store roots of the two vertices
	if (root1 < root2)
		mst[root2] = root1;
	else
		mst[root1] = root2; // Connect to the parent with the smaller number
}

bool isCycle(int* mst, int ver1, int ver2) {
	return getParent(mst, ver1) == getParent(mst, ver2); // Return true if the roots are the same, indicating a cycle
}

int partition(vector<pair<int, pair<int, int>>>& edges, int left, int right) { // Divide and conquer
	int low = left + 1, high = right;
	int pivot = edges[left].first; // Use the weight of the left edge as the pivot

	while (low < high) {
		while (edges[low].first < pivot) low++; // Increment `low` until a value >= pivot is found
		while (edges[high].first > pivot) high--; // Decrement `high` until a value <= pivot is found

		if (low < high) { // If `low` and `high` have not crossed
			swap(edges[low], edges[high]); // Swap their values
		}
	}
	swap(edges[left], edges[high]); // Swap the pivot with the value at `high`

	return high; // Return the new position of the pivot
}

void kruskal_sort(vector<pair<int, pair<int, int>>>& edges, int left, int right) { // Quick sort
	if (left < right) {
		if (right - left + 1 <= 6) { // Use insertion sort if the segment size is 6 or less
			insertion_sort(edges, left, right, left);
		}
		else { // Otherwise, proceed with partitioning
			int pivot = partition(edges, left, right);
			kruskal_sort(edges, left, pivot - 1);
			kruskal_sort(edges, pivot + 1, right);
		}
	}
}

void insertion_sort(vector<pair<int, pair<int, int>>>& edges, int left, int right, int pivot) { // Insertion sort
	int n = right - left + 1, j; // Number of elements to sort
	for (int i = pivot + 1; i < pivot + n; i++) { // Sort from the pivot position onwards
		int key_first = edges[i].first; // Current weight to be inserted
		pair<int, int> key_second = edges[i].second; // Associated start and end nodes

		for (j = i - 1; j >= 0 && edges[j].first > key_first; j--) { // Traverse in reverse order and shift larger values
			edges[j + 1] = edges[j];
		}
		edges[j + 1].first = key_first;
		edges[j + 1].second = key_second; // Insert the original information
	}
}

bool Kruskal(Graph* graph, ofstream& fout)
{
	int size = graph->getSize(), numEdge = 0, weightSum = 0;
	int* mst = new int[size]; // MST array: index is the child node, value is the parent node
	for (int i = 0; i < size; i++)
		mst[i] = i; // Initially, the parent node is itself
	map<int, int>* edge = new map<int, int>[size]; // Row: start node, first: end node, second: weight
	vector<pair<int, pair<int, int>>> edges; // Vector to store edges as (weight, (start, end))
	vector<pair<int, pair<int, int>>> mst_edge; // Vector to store edges in the MST

	map<int, int>::iterator iter; // Iterator to traverse edges
	for (int i = 0; i < size; i++) {
		graph->getAdjacentEdges(i, &edge[i]);
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++) {
			int dest = iter->first; // End node
			int weight = iter->second; // Weight
			edges.push_back(make_pair(weight, make_pair(i, dest))); // Store in the vector
		}
	}

	kruskal_sort(edges, 0, edges.size() - 1); // Use the custom sort function to sort edges by weight
	vector<pair<int, pair<int, int>>>::iterator iter1; // Iterator to traverse sorted edges

	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++) { // Traverse edges in ascending order of weight
		int start = (iter1->second).first;
		int end = (iter1->second).second;
		int weight = iter1->first;

		if (!isCycle(mst, start, end)) { // Check if adding the edge forms a cycle
			numEdge++; // Increment the number of edges in MST
			weightSum += weight; // Add the weight of the edge
			Union(mst, start, end); // Perform the union operation
			mst_edge.push_back(make_pair(start, make_pair(weight, end))); // Store the edge in the MST vector
		}
	}
	if (numEdge != size - 1) { // If MST cannot be formed
		delete[] edge;
		delete[] mst;
		return false;
	}
	else {
		fout << "====== Kruskal ======\n";
		for (int i = 0; i < size; i++) {
			map<int, int> link_ver; // Map to store vertex and weight
			map<int, int>::iterator iter;
			fout << "[" << i << "] ";
			for (iter1 = mst_edge.begin(); iter1 != mst_edge.end(); iter1++) {
				int weight = (iter1->second).first;
				int end = (iter1->second).second;
				int start = iter1->first;
				if (i == end) // If it is the destination vertex
					link_ver.insert(make_pair(start, weight)); // Store the start vertex and weight
				if (i == start) // If it is the start vertex
					link_ver.insert(make_pair(end, weight)); // Store the end vertex and weight
			}
			for (iter = link_ver.begin(); iter != link_ver.end(); iter++)
				fout << iter->first << "(" << iter->second << ") ";
			fout << "\n";
		}
		fout << "cost: " << weightSum << "\n=====================\n\n";
		delete[] edge;
		delete[] mst;
		return true;
	}
}


int length(Graph* graph, int v, int u) { // Function to return the weight of the edge v->u
	if (v == u) // Distance to itself is 0
		return 0;
	int size = graph->getSize();
	map<int, int>* edge = new map<int, int>[size]; // Row: starting node, first: destination node, second: weight
	vector<pair<int, pair<int, int>>> edges; // Store in vector as (start, (destination, weight))

	map<int, int>::iterator iter; // Iterator to traverse `edge`
	vector<pair<int, pair<int, int>>>::iterator iter1; // Iterator to traverse `edges`
	for (int i = 0; i < size; i++) {
		graph->getAdjacentEdges(i, &edge[i]);
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++) {
			int dest = iter->first; // Destination node
			int weight = iter->second; // Weight
			edges.push_back(make_pair(i, make_pair(dest, weight))); // Store in vector
		}
	}

	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++) {
		if (iter1->first == v && (iter1->second).first == u) { // If v is the starting node and u is the destination node, return the weight
			delete[] edge;
			return (iter1->second).second;
		}
	}
	delete[] edge;
	return MAX; // If no such edge exists
}

int choose(int* dist, bool* s, int size, int vertex) {
	int min = MAX; // Minimum weight
	int u = 0;

	for (int i = 0; i < size; i++) {
		if (!s[i] && i != vertex && min > dist[i]) { // Node with minimum weight not in set `s`
			min = dist[i];
			u = i;
		}
	}
	return u;
}

bool Dijkstra(Graph* graph, int vertex, ofstream& fout)
{
	if (vertex < 0 || graph->getSize() <= vertex) // Vertex does not exist
		return false;
	int size = graph->getSize();
	bool* s = new bool[size]; // Set of nodes currently in the set
	int* dist = new int[size]; // Distance from the start vertex
	int* prev = new int[size]; // Store previous vertex

	for (int i = 0; i < size; i++) { // Initialization
		s[i] = false;
		prev[i] = -1;
		dist[i] = length(graph, vertex, i);
		if (dist[i] != MAX) { // If connected
			prev[i] = vertex; // Previous node is the vertex
		}
	}
	s[vertex] = true; // Initially, the set contains itself
	prev[vertex] = -1; // No connection with itself

	int u = vertex;
	for (int i = 0; i < size - 1; i++) {
		u = choose(dist, s, size, u);
		s[u] = true;
		for (int w = 0; w < size; w++) {
			if (!s[w]) {
				int existing_dist = dist[w]; // Existing weight
				dist[w] = min((dist[u] + length(graph, u, w)), dist[w]); // Update weights of nodes connected to the new set
				if (existing_dist != dist[w]) // If new weight changes
					prev[w] = u; // Save previous node
			}
		}
	}
	fout << "====== Dijkstra ======\nstartvertex :" << vertex << "\n";
	for (int i = 0; i < size; i++) {
		stack<int> print_vertex; // Stack for reverse output of the shortest path
		if (i == vertex) // Skip itself
			continue;
		bool flag = false; // Variable to check if there is any connection
		int node = i; // Connected node number
		fout << "[" << i << "] ";
		while (1) {
			if (prev[node] == -1) // No more connection
				break;
			flag = true;
			print_vertex.push(node);
			node = prev[node]; // Move to the previous node
		}
		if (!flag)
			fout << "x\n";
		else {
			print_vertex.push(vertex);
			while (!print_vertex.empty()) {
				int node = print_vertex.top();
				print_vertex.pop();
				fout << node;
				if (print_vertex.size() != 0)
					fout << " -> ";
			}
			fout << " (" << dist[i] << ")\n";
		}
	}
	fout << "======================\n\n";
	delete[] s;
	delete[] dist;
	delete[] prev;
	return true;
}


bool Bellmanford(Graph* graph, int s_vertex, int e_vertex, ofstream& fout)
{
	if (s_vertex < 0 || graph->getSize() <= s_vertex) // Nonexistent s_vertex
		return false;
	if (e_vertex < 0 || graph->getSize() <= e_vertex) // Nonexistent e_vertex
		return false;
	if (s_vertex == e_vertex) { // When start and end vertices are the same
		fout << "====== Bellman-Ford ======\n";
		fout << "Same vertex\ncost: 0\n";
		fout << "==========================\n\n";
		return true;
	}
	int size = graph->getSize();
	int* dist = new int[size]; // Minimum weight from s_vertex to i
	int* p = new int[size]; // Start vertex of the shortest path

	map<int, int>* edge = new map<int, int>[size]; // Row: start node, first: destination node, second: weight
	vector<pair<int, pair<int, int>>> edges; // Stores weight, start node, and destination node in order
	map<int, int>::iterator iter; // Iterator to traverse edge
	vector<pair<int, pair<int, int>>>::iterator iter1; // Iterator to traverse edges

	for (int i = 0; i < size; i++) {
		graph->getAdjacentEdges(i, &edge[i]);
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++) {
			int dest = iter->first; // Destination node
			int weight = iter->second; // Weight
			edges.push_back(make_pair(weight, make_pair(i, dest))); // Save to vector
		}
	}

	for (int i = 0; i < size; i++) { // Initialization
		dist[i] = MAX;
		p[i] = -1;
	}
	dist[s_vertex] = 0; // Distance to itself is 0

	for (int k = 1; k <= size - 1; k++) { // Paths with k edges that have the minimum weight
		for (iter1 = edges.begin(); iter1 != edges.end(); iter1++) { // Check all edges
			int start = (iter1->second).first;
			int end = (iter1->second).second;
			int weight = iter1->first;

			if (dist[start] == MAX) continue; // Vertex not yet calculated
			if (dist[end] > dist[start] + weight) {
				dist[end] = dist[start] + weight; // Update to a path with smaller weight
				p[end] = start; // Save the path
			}
		}
	}

	for (iter1 = edges.begin(); iter1 != edges.end(); iter1++) { // Perform once more to check for negative cycles
		int start = (iter1->second).first;
		int end = (iter1->second).second;
		int weight = iter1->first;

		if (dist[start] == MAX) continue; // Vertex not yet calculated
		if (dist[end] > dist[start] + weight) { // If a value changes in an already shortest path, a negative cycle exists
			delete[] edge;
			delete[] dist;
			delete[] p;
			return false;
		}
	}
	if (dist[e_vertex] == MAX) { // If there is no path to reach
		fout << "====== Bellman-Ford ======\n";
		fout << "x\n";
		fout << "==========================\n\n";
		delete[] edge;
		delete[] dist;
		delete[] p;
		return true;
	}
	stack<int> stk; // Stack to output nodes of the shortest path
	stk.push(e_vertex); // Push destination vertex
	int i = e_vertex;
	while (1) {
		if (p[i] == -1) // Start vertex
			break;
		i = p[i];
		stk.push(i); // Save shortest path from destination to start in reverse order
	}

	fout << "====== Bellman-Ford ======\n";
	while (!stk.empty()) { // Output the shortest path
		int vertex = stk.top();
		stk.pop();
		fout << vertex;
		if (stk.size() != 0)
			fout << " -> ";
	}
	fout << "\ncost: " << dist[e_vertex] << "\n==========================\n\n";
	delete[] edge;
	delete[] dist;
	delete[] p;
	return true;
}

bool FLOYD(Graph* graph, ofstream& fout)
{
	int size = graph->getSize();
	int** arr = new int* [size]; // 2D array representing shortest paths between all vertices

	map<int, int>* edge = new map<int, int>[size]; // Row: start node, first: destination node, second: weight
	vector<pair<int, pair<int, int>>> edges; // Stores weight, start node, and destination node in order
	map<int, int>::iterator iter; // Iterator to traverse edge
	vector<pair<int, pair<int, int>>>::iterator iter1; // Iterator to traverse edges

	for (int i = 0; i < size; i++) {
		graph->getAdjacentEdges(i, &edge[i]);
		for (iter = edge[i].begin(); iter != edge[i].end(); iter++) {
			int dest = iter->first; // Destination node
			int weight = iter->second; // Weight
			edges.push_back(make_pair(weight, make_pair(i, dest))); // Save to vector
		}
	}

	for (int i = 0; i < size; i++)
		arr[i] = new int[size];

	for (int i = 0; i < size; i++) { // Array initialization
		for (int j = 0; j < size; j++) {
			bool flag = false; // Whether an edge exists
			if (i == j) { // Distance to itself is 0
				arr[i][j] = 0;
				continue;
			}
			for (iter1 = edges.begin(); iter1 != edges.end(); iter1++) {
				if (flag)
					break;
				if (i == (iter1->second).first && j == (iter1->second).second) { // i is start vertex, j is destination vertex
					arr[i][j] = iter1->first; // Save weight of the edge
					flag = true; // Edge exists
				}
			}
			if (!flag) // If no edge exists
				arr[i][j] = MAX; // Weight is MAX
		}
	}

	for (int k = 0; k < size; k++) { // Path passing through vertex k
		for (int i = 0; i < size; i++) { // Start vertex
			for (int j = 0; j < size; j++) { // End vertex
				if (arr[i][k] + arr[k][j] < arr[i][j]) { // If weight of i->k->j is less than weight of i->j
					if (arr[i][k] != MAX && arr[k][j] != MAX) // Both must be reachable
						arr[i][j] = arr[i][k] + arr[k][j]; // Update to this weight
				}
			}
		}
	}

	for (int i = 0; i < size; i++) {
		if (arr[i][i] != 0) { // If a negative cycle occurs, distance to itself is not 0 (becomes negative through the cycle)
			delete[] edge;
			for (int i = 0; i < size; i++) {
				delete[] arr[i];
			}
			delete[] arr; // Memory cleanup
			return false; // Return false
		}
	}

	fout << "======== FLOYD ========\n" << '\t'; // Output all paths
	for (int i = 0; i < size; i++) {
		fout << "[" << i << "]" << '\t';
	}
	fout << endl;
	for (int i = 0; i < size; i++) {
		fout << "[" << i << "]";
		for (int j = 0; j < size && fout << '\t'; j++) {
			if (arr[i][j] == MAX) // If path is unreachable
				fout << "x";
			else
				fout << arr[i][j];
		}
		fout << endl;
	}
	fout << "=======================\n\n";
	delete[] edge;
	for (int i = 0; i < size; i++)
		delete[] arr[i];
	delete[] arr;
	return true;
}
