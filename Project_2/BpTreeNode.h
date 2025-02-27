#pragma once
#include "FlightData.h"
class BpTreeNode
{
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;
protected:
	bool dataNode_flag; // Variables that distinguish whether the node is a data node or an index node
public:
	BpTreeNode() {
		dataNode_flag = false;
		pParent = NULL;
		pMostLeftChild = NULL;
	}
	virtual ~BpTreeNode() {

	}

	void setMostLeftChild(BpTreeNode* pN) { pMostLeftChild = pN; }
	void setParent(BpTreeNode* pN) { pParent = pN; }

	BpTreeNode* getParent() { return pParent; }
	BpTreeNode* getMostLeftChild() { return pMostLeftChild; }

	virtual void setNext(BpTreeNode* pN) {}
	virtual void setPrev(BpTreeNode* pN) {}
	virtual BpTreeNode* getNext() { return NULL; }
	virtual BpTreeNode* getPrev() { return NULL; }

	virtual void insertDataMap(string n, FlightData* pN) {}
	virtual void insertIndexMap(string n, BpTreeNode* pN) {}
	virtual void deleteMap(string n) {}

	virtual map<string, BpTreeNode*>* getIndexMap() { map<string, BpTreeNode*> m; return &m; }
	virtual map<string, FlightData*>* getDataMap() { map<string, FlightData*> m; return &m; }

	virtual bool is_dataNode() { return dataNode_flag; }
};