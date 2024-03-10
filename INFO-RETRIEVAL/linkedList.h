#ifndef LINKEDLIST_H
#define LINKEDLIST_H

using namespace std;

#include <iostream>

struct LinkedListNode{
    int docID; float weight;
    LinkedListNode<T> *next; LinkedListNode<T> *prev;
    LinkedListNode(int docID = 0, float weight = 0.0, LinkedListNode* next = NULL, LinkedListNode* prev = NULL);
    ~LinkedListNode();
};

class LinkedList{
    private:
         LinkedListNode* root;
    public:
         LinkedList();
         ~LinkedList();

         LinkedListNode* insert(int value, float weight);
         LinkedListNode* find(int value);
         LinkedListNode* remove(int value);
         int size();

         LinkedListNode* getRoot();
};

#endif
