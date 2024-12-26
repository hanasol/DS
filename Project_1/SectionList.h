#pragma once
#include "SectionListNode.h"
#include <fstream>
class SectionList
{
private:
	SectionListNode* head;
public:
	SectionList();
	~SectionList();

	SectionListNode* getHead();

	// Insert
	void insert_content(int num, string sub_time, string sub_cont);
	void insert_head(int num);
	// Search
	SectionListNode* search(int num);
	// Print
	void print(int num, bool& flag, ofstream& flog);
};
