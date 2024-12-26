#include "SubtitleBST.h"
SubtitleBST::SubtitleBST() : root(nullptr)
{
}
SubtitleBST::~SubtitleBST()
{
	delete_bst(root);
}

SubtitleBSTNode* SubtitleBST::getRoot()
{
	return root;
}

void SubtitleBST::delete_bst(SubtitleBSTNode* node) { // Delete leaf nodes first using post-order traversal
	if (node == nullptr) return;
	delete_bst(node->getLeft());
	delete_bst(node->getRight());
	delete node;
}
// Insert
void SubtitleBST::insert(string time, string cont) {
	SubtitleBSTNode* ptr = new SubtitleBSTNode(); // Save node
	SubtitleBSTNode* node = nullptr; // Parent of the saved node
	ptr->setData(time, cont);
	if (root == nullptr) { // Tree is empty
		root = ptr;
		root->setLeft(nullptr);
		root->setRight(nullptr);
	}
	else { // Tree is not empty
		SubtitleBSTNode* temp = root; // Node to traverse the tree
		while (temp != nullptr) {
			node = temp;
			if (temp->getTime() > time) // Current subtitle time is earlier than the node's subtitle time
				temp = temp->getLeft();
			else
				temp = temp->getRight(); // Current subtitle time is later than the node's subtitle time
		}
		if (ptr->getTime() < node->getTime()) // If the time of the node to be saved is earlier than the parent node's time
			node->setLeft(ptr); // Save as the left child
		else
			node->setRight(ptr); // Save as the right child
	}
}
// Print
void SubtitleBST::print(SubtitleBSTNode* node, ofstream& flog) { // Print BST using in-order traversal
	if (node == nullptr)
		return;
	print(node->getLeft(), flog);
	flog << node->getTime() << " - " << node->getCont() << endl;
	print(node->getRight(), flog);
}
// Search
void SubtitleBST::search(SubtitleBSTNode* node, int num, string first_time, string second_time, SectionList& list, bool& flag) { // In-order traversal
	if (node == nullptr)
		return;
	search(node->getLeft(), num, first_time, second_time, list, flag);
	if (first_time <= node->getTime() && node->getTime() <= second_time) { // If the node's time satisfies the specific range
		list.insert_head(num);
		list.insert_content(num, node->getTime(), node->getCont()); // Insert as content node into the list
		flag = true;
	}
	search(node->getRight(), num, first_time, second_time, list, flag);
}
// Delete
void SubtitleBST::delete_equal(string time, bool& flag) {
	SubtitleBSTNode* p = root; // Node to be deleted (current node)
	SubtitleBSTNode* q = nullptr; // q is the parent of p
	flag = false;
	while (p != nullptr && p->getTime() != time) { // Repeat while p is not NULL and not the target time
		q = p;
		if (time < p->getTime()) p = p->getLeft(); // If the target time is earlier than the current node's time, move to the left child
		else p = p->getRight(); // Otherwise, move to the right child
	}
	if (p == nullptr) { // Not found
		flag = true;
		return;
	}
	if (p->getLeft() == nullptr && p->getRight() == nullptr) { // p is a leaf node
		if (q == nullptr) root = nullptr; // p is root
		else if (q->getLeft() == p) // Left child of q is p
			q->setLeft(nullptr);
		else // Right child of q is p
			q->setRight(nullptr);
		delete p;
	}

	else if (p->getLeft() == nullptr) { // p only has right child
		if (q == nullptr) root = p->getRight(); // p is root
		else if (q->getLeft() == p) q->setLeft(p->getRight()); // Left child of q is p
		else q->setRight(p->getRight()); // Right child of q is p
		delete p;
	}

	else if (p->getRight() == nullptr) { // p only has left child
		if (q == nullptr) root = p->getLeft(); // p is root
		else if (q->getLeft() == p) q->setLeft(p->getLeft()); // Left child of q is p
		else q->setRight(p->getLeft()); // Right child of q is p
		delete p;
	}

	else { // p has two children
		SubtitleBSTNode* prevprev = p; // prevprev is the parent of prev
		SubtitleBSTNode* prev = p->getRight();
		SubtitleBSTNode* curr = p->getRight()->getLeft();

		while (curr) { // Find the leftmost node in the right subtree and store it in prev
			prevprev = prev;
			prev = curr;
			curr = curr->getLeft();
		} // If curr is nullptr, prev is the node that should replace p
		p->setData(prev->getTime(), prev->getCont()); // Replace p's data with prev's data
		if (prevprev == p) prevprev->setRight(prev->getRight()); // prev is the right child of p
		// p's right child pointer is updated to point to prev's right child pointer
		else prevprev->setLeft(prev->getRight()); // prev is the minimum value in p's right subtree, prevprev's left child is prev
		// prevprev's left child pointer is updated to point to prev's right child pointer
		delete prev; // Delete prev as it has swapped places
	}
}

void SubtitleBST::delete_under(SubtitleBSTNode* node, string time, bool& flag, stack<SubtitleBSTNode*>& stk) { // Traverse using in-order traversal
	if (node == nullptr)
		return;
	delete_under(node->getLeft(), time, flag, stk); // Search left subtree
	if (node->getTime() < time) { // If the node's subtitle time is earlier than the specific time
		stk.push(node); // Push the node into the stack
		flag = false; // Nodes to delete exist
	}
	delete_under(node->getRight(), time, flag, stk); // Search right subtree
}
