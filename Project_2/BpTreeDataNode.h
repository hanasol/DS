#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"
class BpTreeDataNode : public BpTreeNode
{
private:
	map <string, FlightData*> mapData;
	BpTreeNode* pNext;
	BpTreeNode* pPrev;

public:
	BpTreeDataNode() {
		dataNode_flag = true;
		pNext = NULL;
		pPrev = NULL;
	}
	~BpTreeDataNode() { // delete FlightData object memory on a data node
		map<string, FlightData*>::iterator iter;
		for (iter = this->getDataMap()->begin(); iter != this->getDataMap()->end(); iter++) {
			FlightData* data = iter->second;
			delete data;
		}
	}
	void setNext(BpTreeNode* pN) override { pNext = pN; }
	void setPrev(BpTreeNode* pN) override { pPrev = pN; }
	BpTreeNode* getNext() { return pNext; }
	BpTreeNode* getPrev() { return pPrev; }

	void insertDataMap(string n, FlightData* pN) override {
		mapData.insert(map<string, FlightData*>::value_type(n, pN));
	}

	void deleteMap(string n) override {
		mapData.erase(n);
	}

	map<string, FlightData*>* getDataMap() { return &mapData; }
};

#endif