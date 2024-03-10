#include <linkedList.h>

LinkedListNode::LinkedListNode(int docID, float weight, LinkedListNode* next, LinkedListNode* prev){
    this->docID = docID;
    this->weight = weight;
    this->next = next;
    this->prev = prev;
}

LinkedListNode::~LinkedListNode(){
    this->next = NULL;
}

LinkedList::LinkedList(){
     this->root = NULL;
}

LinkedList::~LinkedList(){
    LinkedListNode *p = this->root;
    while (p != NULL){
        LinkedListNode *q = p->next;
        delete p;
        p = q;
    }
    this->root = NULL;
}

LinkedListNode* LinkedList::insert(int docID, float weight){
    LinkedListNode *p = this->find(docID);
    if (p != NULL)
        return p;
    p = new LinkedListNode<T>(docID, weight, NULL);
    if (this->root == NULL)
        this->root = p;
    else {
        p->next = this->root;
        this->root = p;
    }
    return p;
}

LinkedListNode* LinkedList::find(int docID){
    LinkedListNode *p = this->root;
    while (p != NULL && p->docID != docID)
        p = p->next;
    return p;
}

LinkedListNode* LinkedList::getRoot(){
    return this->root;
}

LinkedListNode* LinkedList::remove(int docID){
    LinkedListNode *p = this->root;
    LinkedListNode *q = NULL;

    while (p != NULL) {
        if (p->docID == docID) {
             if (q == NULL) 
                 this->root = p->next;
            else
                 q->next = p->next;
            delete p;
            break;
         }
        q = p;
        p = p->next;
    }

    return this->root;
}

int LinkedList::size(){
    int count = 0;
    for (LinkedListNode *p = this->root; p != NULL; p = p->next)
        ++count;
    return count;
}
