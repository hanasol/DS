#pragma once
#include <string>
#include <fstream>
using namespace std;
class SubtitleQueueNode
{
private:
	string sub_time; // Subtitle time
	string sub_content; // Subtitle content
	SubtitleQueueNode* link; // Next node
public:
	SubtitleQueueNode(string time, string content) {
		link = nullptr;
		sub_time = time;
		sub_content = content;
	}
	~SubtitleQueueNode() {

	}
	void Setlink(SubtitleQueueNode* link) {
		this->link = link;
	}
	SubtitleQueueNode* Getlink() const {
		return link;
	}
	string getTime() { return sub_time; }
	string getCont() { return sub_content; }
};
