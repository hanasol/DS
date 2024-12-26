#include "Manager.h"

Manager::Manager()
{
	graph = nullptr;
	fout.open("log.txt");
	load = 0;
}

Manager::~Manager()
{
	if (load) {
		delete graph;
	}
	fout.close();
}

void Manager::run(const char* command_txt) {
	ifstream fin;
	fin.open(command_txt);

	if (!fin)
	{
		fout << "[ERROR] command file open error!" << endl; // Command file open error
		return;
	}
	if (fin.peek() == ifstream::traits_type::eof()) { // Exit if command.txt is empty
		return;
	}

	char* command = NULL;
	char* para1 = NULL;
	char* para2 = NULL;
	char* para3 = NULL;
	char buf[129] = { 0 };

	while (fin.getline(buf, 128))
	{
		command = strtok(buf, "\t\0");
		para1 = strtok(NULL, "\t\0");
		para2 = strtok(NULL, "\t\0");
		para3 = strtok(NULL, "\t\0");

		if (0 == strcmp(command, "LOAD")) {
			if (para2 != nullptr || para1 == nullptr) { // More than 1 argument or insufficient arguments
				printErrorCode(100);
				continue;
			}
			if (LOAD(para1)) {
				fout << "======== LOAD ========\nSuccess\n======================\n\n";
			}
			else
				printErrorCode(100);
		}
		else if (0 == strcmp(command, "PRINT")) {
			if (para1 != nullptr) { // Arguments exist
				printErrorCode(200);
				continue;
			}
			if (graph == nullptr) {
				printErrorCode(200);
				continue;
			}
			if (!PRINT())
				printErrorCode(200);
		}
		else if (0 == strcmp(command, "BFS")) {
			if (para2 != nullptr || para1 == nullptr) { // More than 1 argument or insufficient arguments
				printErrorCode(300);
				continue;
			}
			int vertex = stoi(para1);
			if (!mBFS(vertex)) {
				printErrorCode(300);
			}
		}
		else if (0 == strcmp(command, "DFS")) {
			if (para2 != nullptr || para1 == nullptr) { // More than 1 argument or insufficient arguments
				printErrorCode(400);
				continue;
			}
			int vertex = stoi(para1);
			if (!mDFS(vertex)) {
				printErrorCode(400);
			}
		}
		else if (0 == strcmp(command, "DFS_R")) {
			if (para2 != nullptr || para1 == nullptr) { // More than 1 argument or insufficient arguments
				printErrorCode(500);
				continue;
			}
			int vertex = stoi(para1);
			bool flag = true;
			if (graph == nullptr) {
				printErrorCode(500);
				continue;
			}
			if (vertex < 0 || graph->getSize() < vertex) // Non-existent vertex
				printErrorCode(500);
			else { // Write output format outside the function
				if (graph == nullptr) { // Check if the object exists
					printErrorCode(500);
					continue;
				}
				fout << "======== DFS_R ========\n";
				fout << "startvertex: " << vertex << "\n";
				mDFS_R(vertex);
				fout << "\n=======================\n\n";
			}
		}
		else if (0 == strcmp(command, "KRUSKAL")) {
			if (para1 != nullptr) { // Arguments exist
				printErrorCode(600);
				continue;
			}
			if (!mKRUSKAL())
				printErrorCode(600);
		}
		else if (0 == strcmp(command, "DIJKSTRA")) {
			if (para2 != nullptr || para1 == nullptr) { // More than 1 argument or insufficient arguments
				printErrorCode(700);
				continue;
			}
			int vertex = stoi(para1);
			if (!mDIJKSTRA(vertex)) {
				printErrorCode(700);
			}
		}
		else if (0 == strcmp(command, "BELLMANFORD")) {
			if (para3 != nullptr || para2 == nullptr || para1 == nullptr) { // More than 2 arguments or insufficient arguments
				printErrorCode(800);
				continue;
			}
			int start = stoi(para1);
			int end = stoi(para2);
			if (!mBELLMANFORD(start, end))
				printErrorCode(800);
		}
		else if (0 == strcmp(command, "FLOYD")) {
			if (para1 != nullptr) {
				printErrorCode(900);
				continue;
			}
			if (!mFLOYD()) {
				printErrorCode(900);
			}
		}
		else if (0 == strcmp(command, "EXIT")) {
			fout << "======== EXIT ========\nSuccess\n======================\n\n";
			fin.close();
			load = 1;
			return;
		}
		else {
			printErrorCode(1000);
		}
	}
}

bool Manager::LOAD(char* filename)
{
	if (graph != nullptr) { // If graph information already exists
		delete graph;
	}
	ifstream fin;
	string temp;
	fin.open(filename);
	if (fin.peek() == ifstream::traits_type::eof()) { // Error if file is empty
		return false;
	}
	if (0 == strcmp(filename, "graph_L.txt")) { // List format
		int v, weight, i = -1;
		int size = 0;
		getline(fin, temp); // Check list format
		if (temp != "L")
			return false;
		getline(fin, temp); // Save size
		if (!temp.empty())
			size = stoi(temp);
		graph = new ListGraph(false, size); // Create object in List format
		while (getline(fin, temp)) { // Save connection information
			if (i >= size) { // If a node larger than the size exists
				break; // Exit without performing insertion
			}
			if (temp.find(" ") == string::npos) { // Start vertex
				if (!temp.empty()) {
					int node_num = stoi(temp); // Current vertex
					if (i + 1 != node_num) { // Node does not exist in txt file
						graph = nullptr;
						delete graph; // Delete graph information
						return false; // Output error code
					}
				}
				i++;
			}
			else { // Destination vertex & weight
				istringstream iss(temp);
				iss >> v >> weight;
				graph->insertEdge(i, v, weight);
			}
		}
	}
	else if (0 == strcmp(filename, "graph_M.txt")) { // Matrix format
		int v, weight;
		int size = 0;
		getline(fin, temp); // Check matrix format
		if (temp != "M")
			return false;
		getline(fin, temp); // Save size
		if (!temp.empty())
			size = stoi(temp);
		graph = new MatrixGraph(true, size); // Create object in Matrix format
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				fin >> weight;
				graph->insertEdge(i, j, weight);
			}
		}
	}
	else // Error if file does not exist
		return false;
	return true;
}

bool Manager::PRINT()
{
	if (graph == nullptr)
		return false;
	return graph->printGraph(fout);
}

bool Manager::mBFS(int vertex)
{
	if (graph == nullptr)
		return false;
	return BFS(graph, vertex, fout);
}

bool Manager::mDFS(int vertex)
{
	if (graph == nullptr)
		return false;
	return DFS(graph, vertex, fout);
}

void Manager::mDFS_R(int vertex)
{
	vector<bool> visit(MAX + 1, false);
	DFS_R(graph, visit, vertex, fout);
}

bool Manager::mKRUSKAL() {
	if (graph == nullptr)
		return false;
	return Kruskal(graph, fout);
}

bool Manager::mDIJKSTRA(int vertex)
{
	if (graph == nullptr)
		return false;
	return Dijkstra(graph, vertex, fout);
}

bool Manager::mBELLMANFORD(int s_vertex, int e_vertex)
{
	if (graph == nullptr)
		return false;
	return Bellmanford(graph, s_vertex, e_vertex, fout);
}

bool Manager::mFLOYD()
{
	if (graph == nullptr)
		return false;
	return FLOYD(graph, fout);
}

void Manager::printErrorCode(int n)
{
	fout << "======== ERROR ========" << endl;
	fout << n << endl;
	fout << "=======================" << endl << endl;
}
