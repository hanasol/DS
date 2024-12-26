#include "SectionList.h"

SectionList::SectionList() : head(nullptr)
{
	
}
SectionList::~SectionList()
{
	SectionListNode* head_node = head; // Header node
	SectionListNode* next_node; // Next header node
	SubtitleListNode* cont_node; // Content node
	SubtitleListNode* next_cont_node; // Next content node

	while (head_node != nullptr) {
		next_node = head_node->getNext(); // Next header node of the head
		cont_node = head_node->getLink(); // Next content node of the head
		delete head_node; // Delete the current header node
		while (cont_node != nullptr) {
			next_cont_node = cont_node->getLink(); // Save the next content node
			delete cont_node; // Delete the current content node
			cont_node = next_cont_node; // Move to the next content node
		}
		head_node = next_node; // Move to the next header node
	}
}

SectionListNode* SectionList::getHead()
{
	return head;
}

// Insert
void SectionList::insert_head(int num) {
	if (search(num) != nullptr) // If a section with the same number already exists
		return; // Exit the function
	SectionListNode* head_node = new SectionListNode(num);
	if (head == nullptr) { // If the list is empty
		head = head_node;
	}
	else {
		SectionListNode* temp = head;
		while (temp->getNext() != nullptr)
			temp = temp->getNext(); // Move to the last section's header node
		temp->setNext(head_node); // Connect to the next header node of the corresponding header node
	}
}
void SectionList::insert_content(int num, string sub_time, string sub_cont) {
	SubtitleListNode* node = new SubtitleListNode(sub_time, sub_cont); // The content node to be added
	SectionListNode* temp = search(num); // Move to the header node corresponding to the section number passed as an argument
	if (temp == nullptr)
		return;
	if (temp->getLink() == nullptr) // If only the header node exists in the section
		temp->setLink(node); // Connect it to the first content node of the section
	else { // Otherwise
		SubtitleListNode* temp_node = temp->getLink(); // The last content node of the section to be added
		while (temp_node->getLink() != nullptr)
			temp_node = temp_node->getLink(); // Move temp_node to the last content node of the section
		temp_node->setNext(node); // Connect to the next node of the last content node
	}
}
// Search
inline SectionListNode* SectionList::search(int num) { // Since it's called repeatedly during insertion and is a short code, set as inline function
	SectionListNode* temp = head;
	while (temp != nullptr) {
		if (temp->getNum() == num) // If the header node of the section matches the number passed as an argument
			return temp; // Return the corresponding header node
		else { // Otherwise
			temp = temp->getNext(); // Move to the next header
		}
	}
	return nullptr; // Return nullptr if not found
}

// Print
void SectionList::print(int num, bool& flag, ofstream& flog) {
	if (search(num) == nullptr) { // If no section matching num exists
		flag = false; // Set flag to false
		return;
	}
	SubtitleListNode* temp = search(num)->getLink(); // Store the next content node of the corresponding section's header node
	flog << "===== PRINT =====\n" << "Section " << num << endl;
	while (temp != nullptr) {
		flog << temp->getTime() << " - " << temp->getCont() << endl;
		temp = temp->getLink();
	}
	flog << "===============\n";
}
