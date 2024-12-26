#pragma once
#include "SubtitleQueueNode.h"

class SubtitleQueue
{
private:
	SubtitleQueueNode* front;
	SubtitleQueueNode* rear;
	int size;

public:
	SubtitleQueue();
	~SubtitleQueue();

	bool IsEmpty();
	bool IsFull();
	// Push
	void Push(string time, string content, ofstream& flog);
	// Pop
	SubtitleQueueNode* Pop(ofstream& flog);
	// Retrn front
	SubtitleQueueNode* Front();
	// Print
	void print(ofstream& flog);
};
