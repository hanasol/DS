#pragma once
#include "SubtitleListNode.h"

class SectionListNode
{
private:
	int Section_num; // Section number
	SectionListNode* next; // Pointer to the next header node
	SubtitleListNode* link; // Pointer to the next content node
public:
	SectionListNode(int num) : next(nullptr), link(nullptr), Section_num(num) {}
	~SectionListNode() {}

	int getNum() { return Section_num; }
	SectionListNode* getNext() { return next; } // Returns the next header node
	SubtitleListNode* getLink() { return link; } // Returns the next content node
	void setNext(SectionListNode* next) { this->next = next; }
	void setLink(SubtitleListNode* link) { this->link = link; }
};

