#pragma once
#include <string>
using namespace std;
class SubtitleBSTNode
{
private:
	string sub_time; // Subtitle time
	string sub_content; // Subtitme content
	SubtitleBSTNode* left; // Left child
	SubtitleBSTNode* right; // Right child
public:
	SubtitleBSTNode() : left(nullptr), right(nullptr) {
		sub_time = "";
		sub_content = "";
	}
	~SubtitleBSTNode() {}

	SubtitleBSTNode* getLeft() { return left; }
	SubtitleBSTNode* getRight() { return right; }

	void setLeft(SubtitleBSTNode* left) { this->left = left; }
	void setRight(SubtitleBSTNode* right) { this->right = right; }

	void setData(string time, string cont) { sub_time = time, sub_content = cont; }
	string getTime() { return sub_time; }
	string getCont() { return sub_content; }
};
