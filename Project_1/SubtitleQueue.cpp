#include "SubtitleQueue.h"

SubtitleQueue::SubtitleQueue() // Initialize state of the queue
{
	front = rear = nullptr;
	size = 0;
}
SubtitleQueue::~SubtitleQueue() // Destructor
{
	SubtitleQueueNode* next = nullptr;
	while (nullptr != front) {
		next = front->Getlink(); // Save the next node
		delete front; // Delete the current node
		front = next; // Move to the next node
	}
}

bool SubtitleQueue::IsEmpty() // If queue is empty -> return true, else return false
{
	return front == nullptr;
}
bool SubtitleQueue::IsFull() // If queue is full -> return true, else return false
{
	return size == 100; // The queue size is fixed to 100
}
void SubtitleQueue::Push(string time, string content, ofstream& flog)
{
	if (IsFull()) { // If queue is full -> terminate the program
		flog << "Subtitle queue is full\n";
		exit(-1);
	}
	SubtitleQueueNode* newNode = new SubtitleQueueNode(time, content); // Create a node to add
	if (IsEmpty()) { // If the queue is empty
		front = rear = newNode; // Both front and rear point to the newNode
	}
	else { // Otherwise
		rear->Setlink(newNode); // Link rear to the new node
		rear = newNode; // Move rear to the new node
	}
	size++; // Increase the size
}
SubtitleQueueNode* SubtitleQueue::Pop(ofstream& flog)
{
	if (IsEmpty()) { // If the queue is empty
		flog << "Subtitle queue is empty\n";
		exit(-1);
	}
	SubtitleQueueNode* node = front;
	front = front->Getlink(); // Move front to the next node
	return node; // Return the front node
	// Memory is freed in the Manager class
}
SubtitleQueueNode* SubtitleQueue::Front() // Return the front node of the queue
{
	return front;
}
void SubtitleQueue::print(ofstream& flog) {
	SubtitleQueueNode* node = front;
	while (node != nullptr) { // Traverse and print from front to nullptr
		flog << node->getTime() << " - " << node->getCont() << endl;
		node = node->Getlink();
	}
}
