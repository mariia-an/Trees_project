#include <iostream>
#include "SplayTree.h"
#include <limits>
#include <algorithm>

//Node

NodeInt::NodeInt() {
    value = 0;
    l = r = nullptr;
}

NodeInt::NodeInt(int n) {
    value = n;
    l = r = nullptr;
}

NodeInt::NodeInt(int value, NodeInt* l, NodeInt* r) {
    this->value = value;
    this->l = l;
    this->r = r;
}

//Tree

TreeInt::TreeInt() {
    root = nullptr;
}

TreeInt::TreeInt(int value) {
    root = new NodeInt(value);
}

TreeInt::~TreeInt() {
    delete root;
}

void TreeInt::leftRotate(NodeInt* pivot, NodeInt* parent) {
    NodeInt* current = pivot->r;
    pivot->r = current->l;
    current->l = pivot;
    if (parent != nullptr) {
        if (parent->value > current->value) {
            parent->l = current;
        }
        else if (parent->value < current->value) {
            parent->r = current;
        }
    }
    else {
        return;
    }
}

void TreeInt::rightRotate(NodeInt* pivot, NodeInt* parent) {
    NodeInt* current = pivot->l;
    pivot->l = current->r;
    current->r = pivot;
    if (parent != nullptr) {
        if (parent->value > current->value) {
            parent->l = current;
        }
        else if (parent->value < current->value) {
            parent->r = current;
        }
    }
    else {
        return;
    }
}

void TreeInt::insert(int value) {
    std::stack<NodeInt*> parents;
    NodeInt* current_node = root;
    
    bool is_inserted = false;

    if (root == nullptr) {
        root = new NodeInt(value);
        return;
    }

    while (!is_inserted) {
        if (current_node->value > value) {
            if (current_node->l == nullptr) {
                current_node->l = new NodeInt(value);
                is_inserted = true;
            }
            parents.push(current_node);
            current_node = current_node->l;
        }
        else if (current_node->value < value) {
            if (current_node->r == nullptr) {
                current_node->r = new NodeInt(value);
                is_inserted = true;
            }
            parents.push(current_node);
            current_node = current_node->r;
        }
        else {
            return;
        }
    }
    splay(current_node, parents);
}

void TreeInt::splay(NodeInt* rootToParent, std::stack<NodeInt*>& parents) {

    if (root == NULL) {
        return;
    }

    NodeInt* leftTreeMax = NULL;
    NodeInt* rightTreeMin = NULL;
    NodeInt* parent;
    NodeInt* ded;

    int flag = 0;

    while (!parents.empty()) {
        parent = parents.top();
        parents.pop();
        if (parents.size() >= 1) {
            ded = parents.top();
            parents.pop();
        }
        else {
            flag = 1;
            ded = parent;
        }

        if (flag == 0 && ded->l == parent && parent->l == rootToParent) {
            rightRotate(parent, ded);
            rightRotate(ded, (parents.empty()) ? nullptr : parents.top());
        }
        else if (flag == 0 && ded->r == parent && parent->r == rootToParent) {
            leftRotate(parent, ded);
            leftRotate(ded, (parents.empty()) ? nullptr : parents.top());
        }
        else if (flag == 0 && ded->r == parent && parent->l == rootToParent) {
            rightRotate(parent, ded);
            leftRotate(ded, (parents.empty()) ? nullptr : parents.top());
        }
        else if (flag == 0 && ded->l == parent && parent->r == rootToParent) {
            leftRotate(parent, ded);
            rightRotate(ded, (parents.empty()) ? nullptr : parents.top());
        }
        else if (flag == 1 && parent->l == rootToParent) {
            rightRotate(parent, nullptr);
        }
        else if (flag == 1 && parent->r == rootToParent) {
            leftRotate(parent, nullptr);
        }
    }
    if (root != rootToParent) {
        root = rootToParent;
    }
}

bool TreeInt::find(int value) {

    std::stack<NodeInt*> parents;
    NodeInt* current_node = root;

    while (current_node != nullptr) {
        if (current_node->value > value) {
            parents.push(current_node);
            current_node = current_node->l;
        }
        else if (current_node->value < value) {
            parents.push(current_node);
            current_node = current_node->r;
        }
        else {
            splay(current_node, parents);
            return true;
        }
    }

    return false;

}

bool TreeInt::erase(int value) {

    int flag = 0;

    if (root == nullptr) {
        return false;
    }

    NodeInt* current_node = root;
    NodeInt* parent = nullptr;

    while (current_node->value != value && current_node != nullptr) {
        if (current_node->value > value) {
            parent = current_node;
            current_node = current_node->l;
        }
        else if (current_node->value < value) {
            parent = current_node;
            current_node = current_node->r;
        }
        else {
            break;
        }
    }

    if (current_node == nullptr) {
        return false;
    }

    NodeInt* add = current_node;

    while (current_node->l != nullptr || current_node->r != nullptr) {
        if (current_node->l != nullptr) {
            parent = current_node;
            current_node = current_node->l;
            parent->value = current_node->value;
        }
        else if (current_node->r != nullptr) {
            parent = current_node;
            current_node = current_node->r;
            parent->value = current_node->value;
        }
    }

    if (parent == nullptr) {
        delete root;
        root = nullptr;
        return true;
    }

    if (parent->l == current_node) {
        parent->l = nullptr;
    }
    else {
        parent->r = nullptr;
    }
    delete current_node;
    return true;
}

int TreeInt::getRoot() {
    return root->value;
}