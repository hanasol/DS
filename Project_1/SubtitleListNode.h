#pragma once
#include <string>
using namespace std;
class SubtitleListNode
{
private:
	string sub_time; // Subtitle time
	string sub_cont; // Subtitle content
	SubtitleListNode* link; // Next Subtitle node

public:
	SubtitleListNode(string time, string cont) : link(nullptr){
		sub_time = time;
		sub_cont = cont;
	}
	~SubtitleListNode() {}

	SubtitleListNode* getLink() { return link; }
	void setNext(SubtitleListNode* next) { this->link = next; }
	string getTime() { return sub_time; }
	string getCont() { return sub_cont; }
};
