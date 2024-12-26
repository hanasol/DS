#include "BpTree.h"

BpTree::~BpTree() { // Release memory for tree nodes & FlightData objects
	delete_Bp(root);
}

void BpTree::delete_Bp(BpTreeNode* node) { // Recursively release memory for index nodes
	if (node == nullptr)
		return;
	delete_Bp(node->getMostLeftChild());
	if (!node->is_dataNode()) { // Index node
		map<string, BpTreeNode*>::iterator i;
		for (i = node->getIndexMap()->begin(); i != node->getIndexMap()->end(); i++) { // Release memory by passing lower-level nodes
			delete_Bp(i->second);
		}
	}
	delete node; // Release memory for the node
}

void BpTree::Insert(FlightData* newData) {
	if (root == nullptr) { // If the tree is empty
		BpTreeNode* node = new BpTreeDataNode;
		node->insertDataMap(newData->GetFlightNumber(), newData);
		root = node;
	}
	else {
		string data = newData->GetFlightNumber(); // Flight number of data to be stored
		if (root->is_dataNode()) { // If the root node is a data node
			root->insertDataMap(data, newData);
			if (excessDataNode(root)) { // If the root node needs to be split
				BpTreeNode* newDataNode = new BpTreeDataNode; // Create a new data node
				BpTreeNode* newIndexNode = new BpTreeIndexNode; // Create a new index node
				newDataNode->insertDataMap(root->getDataMap()->begin()->first, root->getDataMap()->begin()->second); // Initialize the new data node with the first element's first and second from root
				root->deleteMap(root->getDataMap()->begin()->first); // Delete the first element from root
				newIndexNode->insertIndexMap(root->getDataMap()->begin()->first, root); // Initialize the index node with the new first element from root
				newDataNode->setNext(root);
				root->setPrev(newDataNode); // Link newDataNode to root before changing root
				root = newIndexNode; // Change the root to the new index node
				root->setMostLeftChild(newDataNode); // Set new data node as the LeftChild of the root
				newDataNode->setParent(root); // Set the new data node's parent to root
				newDataNode->getNext()->setParent(root); // Set parent of the split data node to root
			}
		}
		else { // If the root node is an index node
			BpTreeNode* node = searchDataNode(data); // Data node to store new data
			node->insertDataMap(data, newData);
			if (excessDataNode(node)) { // If the data node needs to be split
				splitDataNode(node); // Split
			}
		}
	}
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) { // Proceed with split if true
	if (pDataNode->getDataMap()->size() > order - 1) return true; // Order is equal to the number of elements
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) { // Proceed with split if true
	if (pIndexNode->getIndexMap()->size() > order - 1) return true; // Order is equal to the number of elements
	else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) { // Split data node
	BpTreeNode* ppDataNode = pDataNode->getParent(); // Parent node (index node) of the data node to be split
	if (ppDataNode == nullptr) // Exit if there's no parent node
		return;
	BpTreeNode* newDataNode = new BpTreeDataNode; // Create a new data node
	newDataNode->insertDataMap(pDataNode->getDataMap()->begin()->first, pDataNode->getDataMap()->begin()->second); // Initialize new data node with the first and second of pDataNode
	pDataNode->deleteMap(pDataNode->getDataMap()->begin()->first); // Delete the first element of pDataNode
	newDataNode->setParent(ppDataNode); // Set parent of new data node as index node

	if (pDataNode->getPrev() == nullptr) { // If original data node has no prev & Next doesn't matter
		newDataNode->setNext(pDataNode); // Set new data node's next to pDataNode
		pDataNode->setPrev(newDataNode); // Set pDataNode's prev to new data node
	}
	else { // If original data node has a prev & Next doesn't matter
		newDataNode->setNext(pDataNode); // Set new data node's next to pDataNode
		newDataNode->setPrev(pDataNode->getPrev()); // Set new data node's prev to pDataNode's prev
		pDataNode->getPrev()->setNext(newDataNode); // Reassign the previous node's next to the new data node
		pDataNode->setPrev(newDataNode); // Reassign pDataNode's prev to new data node
	}
	// Split data node & link with doubly linked list

	// Add index node
	if (ppDataNode->getIndexMap()->begin()->first > pDataNode->getDataMap()->begin()->first) { // Added to the left of the index node
		ppDataNode->insertIndexMap(pDataNode->getDataMap()->begin()->first, pDataNode); // Insert into index node
		ppDataNode->setMostLeftChild(newDataNode); // Set new data node as new MostLeftChild of index node
	}
	else {
		map <string, BpTreeNode*>::iterator iter = ppDataNode->getIndexMap()->begin(); // First element of ppDataNode
		if (ppDataNode->getIndexMap()->size() == 2) { // Only if there are two elements
			iter++; // Move to the second element
		}
		if (pDataNode->getDataMap()->begin()->first > iter->first) { // Added to the right of the index node
			iter->second = newDataNode; // Set second (only one) of index node to new data node
		}
		else { // Added to the middle of the index node (when there are two elements)
			iter--; // Move back to the first element
			iter->second = newDataNode; // Set the first element's second of index node to new data node
		}
		ppDataNode->insertIndexMap(pDataNode->getDataMap()->begin()->first, pDataNode); // Insert into index node
	}

	BpTreeNode* Index = ppDataNode;
	while (Index != nullptr) { // Proceed with split by going up through Parent
		if (excessIndexNode(Index)) {
			splitIndexNode(Index);
			Index = Index->getParent();
		}
		else // If the index node isn't split, neither will its parent
			break;
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	BpTreeNode* newIndexNode = new BpTreeIndexNode; // Create a new index node
	newIndexNode->setMostLeftChild(pIndexNode->getMostLeftChild()); // Set newIndexNode's LeftChild as pIndexNode's LeftChild
	pIndexNode->getMostLeftChild()->setParent(newIndexNode); // Set the LeftChild's Parent to newIndexNode
	pIndexNode->getIndexMap()->begin()->second->setParent(newIndexNode); // Set the Parent of the data node (first element's second in pIndexNode) as newIndexNode
	newIndexNode->insertIndexMap(pIndexNode->getIndexMap()->begin()->first, pIndexNode->getIndexMap()->begin()->second); // Insert the first element from pIndexNode into newIndexNode
	pIndexNode->deleteMap(pIndexNode->getIndexMap()->begin()->first); // Delete the first element in pIndexNode

	if (root == pIndexNode) { // If the given index node is the root
		BpTreeNode* newRootNode = new BpTreeIndexNode; // Create a new index node as the new root
		// Set up the new root node
		root = newRootNode; // Reset the root
		newRootNode->setMostLeftChild(newIndexNode); // Set new root's LeftChild as newIndexNode
		newIndexNode->setParent(newRootNode); // Set newIndexNode's Parent as new root
		newRootNode->insertIndexMap(pIndexNode->getIndexMap()->begin()->first, pIndexNode); // Store first element's name and second as pIndexNode in new root
		pIndexNode->setMostLeftChild(pIndexNode->getIndexMap()->begin()->second); // Reset pIndexNode's LeftChild as the first element's second
		pIndexNode->setParent(newRootNode); // Set pIndexNode's Parent as new root
		pIndexNode->deleteMap(pIndexNode->getIndexMap()->begin()->first); // Delete the element that moved up to the new root
	}
	else { // If not the root
		BpTreeNode* ppIndexNode = pIndexNode->getParent();
		if (ppIndexNode->getMostLeftChild() == pIndexNode) { // Splitting from the left side
			ppIndexNode->setMostLeftChild(newIndexNode); // Set ppIndexNode's LeftChild to the new index node
		}
		else { // Middle or right side
			map <string, BpTreeNode*>::iterator iter = ppIndexNode->getIndexMap()->begin();
			if (ppIndexNode->getIndexMap()->size() == 2) // If the index node has two elements
				iter++;
			if (iter->first < pIndexNode->getIndexMap()->begin()->first) { // Adding from the right
				iter->second = newIndexNode; // Reset current element's second to new index node
			}
			else { // Adding in the middle (only if two elements)
				iter--;
				iter->second = newIndexNode; // Reset current element's second to new index node
			}
		}
		newIndexNode->setParent(ppIndexNode); // Set new index node's Parent as ppIndexNode
		ppIndexNode->insertIndexMap(pIndexNode->getIndexMap()->begin()->first, pIndexNode); // Insert the first element's name and second as pIndexNode into ppIndexNode
		pIndexNode->setMostLeftChild(pIndexNode->getIndexMap()->begin()->second); // Reset pIndexNode's LeftChild to the first element's second
		pIndexNode->deleteMap(pIndexNode->getIndexMap()->begin()->first); // Delete the element that moved up to ppIndexNode
	}
}

BpTreeNode* BpTree::searchDataNode(string name) { // Return the data node to insert name
	if (root == nullptr) // If the B+ tree is empty
		return nullptr;
	BpTreeNode* pCur = root;
	while (!pCur->is_dataNode()) { // Repeat until reaching a data node
		if (pCur->getIndexMap()->begin()->first > name) { // If smaller than the key of the first element
			pCur = pCur->getMostLeftChild();
			continue;
		}
		map <string, BpTreeNode*>::iterator iter = pCur->getIndexMap()->begin(); // Save first element in index node
		if (pCur->getIndexMap()->size() == 1) { // If map has one element
			pCur = iter->second; // Move to the next node
		}
		else { // If map has two elements
			iter++; // Move to the next (last) element
			if (iter->first <= name) { // If larger or equal to the key of the last element
				pCur = iter->second; // Move to the next node
			}
			else { // If between the first and last elements
				iter--; // Move back to the first element
				pCur = iter->second; // Move to the next node
			}
		}
	}
	return pCur;
}

bool BpTree::SearchModel(string model_name) {
	BpTreeNode* node = searchDataNode(model_name); // Store the data node where model_name might exist
	if (node == nullptr) // B+ tree is empty
		return false;
	map <string, FlightData*>::iterator iter;
	for (iter = node->getDataMap()->begin(); iter != node->getDataMap()->end(); iter++) { // Traverse the elements in node's map
		if (iter->second->GetFlightNumber() == model_name) // If there is a flight name matching model_name
			return true;
	}
	return false; // If not found
}

bool BpTree::SearchRange(string start, string end, ofstream& fout) {
	BpTreeNode* node = root;
	while (!node->is_dataNode()) {
		node = node->getMostLeftChild(); // Traverse down to the LeftChild until reaching a data node
	}
	if (node->getDataMap()->begin()->first > end) // If the first element's first in the leftmost data node is larger than end
		// No data exists in this B+ tree within the search range
		return false;
	bool flag = false;
	while (node != nullptr) {
		map <string, FlightData*>::iterator iter; // Iterator to traverse elements in node
		for (iter = node->getDataMap()->begin(); iter != node->getDataMap()->end(); iter++) {
			string first;
			first += iter->first[0]; // Store only the first letter of the current element's first
			if (start <= first && first <= end) { // If the flight name falls within start and end range
				if (!flag) // Print only when found for the first time
					fout << "========SEARCH_BP========\n";
				fout << iter->second->GetFlightNumber() << " | " << iter->second->GetAirlineName() << " | " << iter->second->GetDestination() << " | " << iter->second->GetNumberofSeats() << " | " << iter->second->GetStatus() << endl;
				// Output flight information
				flag = true;
			}
		}
		node = node->getNext(); // Move to the next node
	}
	if (flag) // If any data was found
		fout << "========================\n\n";
	return flag;
}

void BpTree::Print(ofstream& fout) {
	BpTreeNode* node = root;
	while (!node->is_dataNode()) {
		node = node->getMostLeftChild(); // Traverse down to the LeftChild until reaching a data node
	}
	while (node != nullptr) {
		map <string, FlightData*>::iterator iter; // Iterator to traverse elements in node
		for (iter = node->getDataMap()->begin(); iter != node->getDataMap()->end(); iter++) {
			fout << iter->second->GetFlightNumber() << " | " << iter->second->GetAirlineName() << " | " << iter->second->GetDestination() << " | " << iter->second->GetNumberofSeats() << " | " << iter->second->GetStatus() << endl;
			// Output flight information
		}
		node = node->getNext(); // Move to the next node
	}
}

void BpTree::add_bp(string AirlineName, string FlightNumber, string Destination, string Status, ofstream& fout, bool& flag, AVLTree* avl) {
	flag = true;
	BpTreeNode* node = searchDataNode(FlightNumber); // Data node containing the flight name matching FlightNumber
	map <string, FlightData*>::iterator iter;
	for (iter = node->getDataMap()->begin(); iter != node->getDataMap()->end(); iter++) { // Traverse elements in node's map
		if (iter->second->GetFlightNumber() == FlightNumber) { // iter is the element with the flight name passed as an argument
			if (iter->second->GetStatus() == "Boarding" && Status == "Boarding") { // Boarding->Boarding
				if (iter->second->GetNumberofSeats() >= 1) { // Available seats exist
					iter->second->SetSeatsDec();
					if (iter->second->GetNumberofSeats() == 0) // If no seats remain, insert into AVL tree
						avl->Insert(iter->second);
				}
				else { // No seats available
					flag = false;
					return;
				}
			}
			else if (iter->second->GetStatus() == "Delayed" && Status == "Delayed") { // Delayed->Delayed
				if (iter->second->GetNumberofSeats() >= 1) { // Available seats exist
					iter->second->SetSeatsDec();
					if (iter->second->GetNumberofSeats() == 0) // If no seats remain, insert into AVL tree
						avl->Insert(iter->second);
				}
				else { // No seats available
					flag = false;
					return;
				}
			}
			else if (iter->second->GetStatus() == "Cancelled" && Status == "Boarding") { // Cancelled->Boarding
				iter->second->SetStatus("Boarding"); // Change status to Boarding
				if (iter->second->GetNumberofSeats() >= 1) { // Available seats exist
					iter->second->SetSeatsDec();
					if (iter->second->GetNumberofSeats() == 0) // If no seats remain, insert into AVL tree
						avl->Insert(iter->second);
				}
				else { // No seats available
					flag = false;
					return;
				}
			}
			else if (iter->second->GetStatus() == "Boarding" && Status == "Cancelled") { // Boarding->Cancelled
				iter->second->SetStatus("Cancelled"); // Only change the status without decreasing seats
			}
			else { // Print an error for any other cases
				flag = false;
			}
			if (flag) { // Print updated information if not in an error state
				fout << "=========== ADD ===========\n";
				fout << iter->second->GetFlightNumber() << " | " << iter->second->GetAirlineName() << " | " << iter->second->GetDestination() << " | " << iter->second->GetNumberofSeats() << " | " << iter->second->GetStatus() << endl;
				fout << "===========================\n\n";
			}
		}
	}
}
