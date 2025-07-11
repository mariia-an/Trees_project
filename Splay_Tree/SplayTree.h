#include <stack>

#pragma once

class NodeInt {
public:
	NodeInt();
	NodeInt(int n);
	NodeInt(int value, NodeInt* l, NodeInt* r);

	NodeInt* root;
	NodeInt* l, * r, * parent;

	int value;

};

class TreeInt {
public:
	TreeInt();
	TreeInt(int value);
	~TreeInt();

	bool find(int value);
	void insert(int value);
	bool erase(int value);
	int getRoot();

	NodeInt* root;
	int value;

	void leftRotate(NodeInt* pivot, NodeInt* parent);
	void rightRotate(NodeInt* pivot, NodeInt* parent);
	void splay(NodeInt* rootToParent, std::stack<NodeInt*>& parents);

};

