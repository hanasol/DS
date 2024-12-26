#pragma once
#include "SubstitleBSTNode.h"
#include "SectionList.h"
#include <stack>
class SubtitleBST
{
private:
	SubtitleBSTNode* root;
public:
	SubtitleBST();
	~SubtitleBST();

	SubtitleBSTNode* getRoot();

	void delete_bst(SubtitleBSTNode* node);
	// Insert
	void insert(string time, string cont);
	// Print
	void print(SubtitleBSTNode* node, ofstream& flog);
	// Search
	void search(SubtitleBSTNode* node, int num, string first_time, string second_time, SectionList& list, bool& flag);
	// Delete
	void delete_equal(string time, bool& flag);
	void delete_under(SubtitleBSTNode* node, string time, bool& flag, stack<SubtitleBSTNode*>& stk);
};
