#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "FlightData.h"
#include "AVLNode.h"
/* class AVL */

class AVLTree {
private:
	AVLNode* root;
public:

	AVLTree() {
		root = NULL;
	}
	~AVLTree();
	void delete_AVL(AVLNode* node);
	bool		Insert(FlightData* pFlightData);
	FlightData* Search(string name);
	void GetVector(vector<FlightData*>& v); // Save the node in the AVL tree to print_vector
	AVLNode* GetRoot() { return root; }
};

#endif