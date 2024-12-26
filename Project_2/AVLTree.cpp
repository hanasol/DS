#include "AVLTree.h"

AVLTree::~AVLTree() {
	delete_AVL(root);
}
void AVLTree::delete_AVL(AVLNode* node) { // Post-order traversal to free memory of AVL tree
	if (node == nullptr)
		return;
	delete_AVL(node->getLeft()); // Recursively delete left subtree
	delete_AVL(node->getRight()); // Recursively delete right subtree
	delete node; // Free current node
}

bool AVLTree::Insert(FlightData* pFlightData) {
	AVLNode* node = new AVLNode;
	node->setFlightData(pFlightData);
	if (root == nullptr) { // If the tree is empty
		root = node;
		return true;
	}
	AVLNode* a = root, // The most recent node where balance factor is +1 or -1
		* pa = nullptr, // Parent of 'a'
		* p = root, // Node for tree traversal
		* pp = nullptr, // Parent of 'p'
		* root_sub = nullptr; // Root of the new subtree
	while (p != nullptr) { // Traverse the tree
		if (p->getBF() != 0) { // If the balance factor is +1 or -1, candidate for imbalance
			a = p; pa = pp;
		}
		if (pFlightData->GetFlightNumber() < p->getFlightData()->GetFlightNumber()) { // If current flight is lexicographically smaller than p's flight
			pp = p;
			p = p->getLeft();
		}
		else { // If current flight is lexicographically greater than p's flight
			// Flight numbers are unique, so no duplicates
			pp = p;
			p = p->getRight();
		}
	}
	// Set as a child of pp
	if (pFlightData->GetFlightNumber() < pp->getFlightData()->GetFlightNumber())
		pp->setLeft(node);
	else
		pp->setRight(node);
	int def; // Variable indicating direction of imbalance: 1 for left, -1 for right
	AVLNode* b, // Child of a
		* c; // Child of b
	if (pFlightData->GetFlightNumber() > a->getFlightData()->GetFlightNumber()) { // Right-leaning imbalance
		p = a->getRight(); // p is the right child of a
		b = p; // Initialize b as p
		def = -1;
	}
	else { // Left-leaning imbalance
		p = a->getLeft(); // p is the left child of a
		b = p;
		def = 1;
	}
	while (p != node) {
		if (pFlightData->GetFlightNumber() > p->getFlightData()->GetFlightNumber()) { // Increase height on the right side
			p->setBF(-1);
			p = p->getRight();
		}
		else { // Increase height on the left side
			p->setBF(1);
			p = p->getLeft();
		}
	}
	if (a->getBF() == 0 || a->getBF() + def == 0) { // If the tree is still balanced
		a->setBF(a->getBF() + def);
		return true;
	}

	if (def == 1) { // Left imbalance
		if (b->getBF() == 1) { // LL Rotation
			a->setLeft(b->getRight()); // Set a's left child as b's right child
			b->setRight(a); // Set b's right child as a
			a->setBF(0);
			b->setBF(0); // Both a and b have balance factor 0
			root_sub = b; // b becomes the root of the subtree
		}
		else { // LR Rotation
			c = b->getRight();
			b->setRight(c->getLeft());
			a->setLeft(c->getRight());
			c->setLeft(b);
			c->setRight(a);
			if (c->getBF() == 0) { // If a and b have equal heights
				a->setBF(0);
				b->setBF(0);
			}
			else if (c->getBF() == 1) { // If b's height is 1 greater
				a->setBF(-1);
				b->setBF(0);
			}
			else if (c->getBF() == -1) { // If a's height is 1 greater
				a->setBF(0);
				b->setBF(1);
			}
			c->setBF(0);
			root_sub = c; // c becomes the root of the subtree, unlike LL
		}
	}
	else { // Right imbalance
		if (b->getBF() == -1) { // RR Rotation
			a->setRight(b->getLeft()); // Set a's right child as b's left child
			b->setLeft(a); // Set b's left child as a
			a->setBF(0);
			b->setBF(0); // Both a and b have balance factor 0
			root_sub = b; // b becomes the root of the subtree
		}
		else { // RL Rotation
			c = b->getLeft();
			b->setLeft(c->getRight());
			a->setRight(c->getLeft());
			c->setRight(b);
			c->setLeft(a);
			if (c->getBF() == 0) { // If a and b have equal heights
				a->setBF(0);
				b->setBF(0);
			}
			else if (c->getBF() == 1) { // If b's height is 1 greater
				a->setBF(0);
				b->setBF(-1);
			}
			else if (c->getBF() == -1) { // If a's height is 1 greater
				a->setBF(1);
				b->setBF(0);
			}
			c->setBF(0);
			root_sub = c; // c becomes the root of the subtree, unlike RR
		}
	}
	if (pa == nullptr)
		root = root_sub; // If a has no parent, set the root as root_sub
	else if (pa->getLeft() == a)
		pa->setLeft(root_sub); // If a is the left child of pa, update pa's left child
	else
		pa->setRight(root_sub); // If a is the right child of pa, update pa's right child
	// The root of the subtree has changed, so rearrange the structure
	return true;
}

void AVLTree::GetVector(vector<FlightData*>& v) { // In-order traversal to get flight data in a vector
	v.clear(); // Clear Print_vector
	stack<AVLNode*> stk;
	AVLNode* node = root;
	while (node != nullptr || !stk.empty()) { // While there is a node or the stack is not empty
		while (node != nullptr) {
			stk.push(node); // Push the current node to the stack
			node = node->getLeft(); // Move to the left child
		}
		node = stk.top(); // Get the top node from the stack
		stk.pop(); // Pop the top node
		v.push_back(node->getFlightData()); // Add the flight data to the vector
		node = node->getRight(); // Move to the right child
	}
}

FlightData* AVLTree::Search(string name) {
	AVLNode* node = root;
	while (node != nullptr) {
		if (node->getFlightData()->GetFlightNumber() > name) // If the flight number is greater than the search key
			node = node->getLeft(); // Move to the left child
		else if (node->getFlightData()->GetFlightNumber() < name) // If the flight number is smaller than the search key
			node = node->getRight(); // Move to the right child
		else // If a match is found
			return node->getFlightData();
	}
	return nullptr; // Return nullptr if the flight is not found in the AVL tree
}

