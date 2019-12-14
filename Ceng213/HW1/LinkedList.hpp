#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T> 
class LinkedList {
    private:
        Node<T>* head;
        Node<T>* tail;
	size_t  size; 
    public:

        LinkedList();
        LinkedList(const LinkedList<T>& ll); 
        LinkedList<T>& operator=(const LinkedList<T>& ll);
        ~LinkedList();

        /* Return head of the linked-list*/
        Node<T>* getHead() const;
        /* Set head of the linked-list*/
        void setHead(Node<T>* n);
        /* Return tail of the linked-list*/
        Node<T>* getTail() const;
        /* Set tail of the linked-list*/
        void setTail(Node<T>* n);
        /* Get the previous node of the node that contains the data item. 
         * If the head node contains the data item, this method returns NULL.*/
        Node<T>* findPrev(const T& data) const;
        /* Get the node that stores the data item. 
         * If data is not found in the list, this function returns NULL.*/
        Node<T>* findNode(const T& data) const;
        /* Insert a new node to store the data item. 
         * The new node should be placed after the “prev” node. 
         * If prev is NULL then insert new node to the head.*/
        void insertNode(Node<T>* prev, const T& data); 
        /* This method is used to delete the node that is next to “prevNode”. 
         * PS:prevNode is not the node to be deleted. */
        void deleteNode(Node<T>* prevNode);  
        /* This method is used to clear the contents of the list.*/
        void clear();
        /* This method returns true if the list empty, otherwise returns false.*/
        bool isEmpty() const;
        /* This method returns the current size of the list. */
        size_t getSize() const;
        /*Prints the list. This method was already implemented. Do not modify.*/
        void print() const;//done
};

template <class T>
void LinkedList<T>::print() const{
    const Node<T>* node = head;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/*....DO NOT EDIT ABOVE....*/
template <class T>
LinkedList<T>::LinkedList(){
     head = NULL;
     tail = NULL;
     size = 0;
}
template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &ll){
    Node<T> *p = ll.head;
    if(p == NULL)return;
    this->head = new Node<T>(ll.head->getData());
    p= p->getNext();
    Node<T> *q = this->head;
    while(p != NULL){
        Node<T> *node = new Node<T>(p->getData());
        q->setNext(node);
        q=q->getNext();
        p=p->getNext();
    }
    this->size = ll.size;
    this->tail = q;
}
/* TO-DO: method implementations below */
template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll){
    this->clear();
    Node<T> *p = ll.head;
    if(p == NULL)return *this;
    this->head = new Node<T>(ll.head->getData());
    p= p->getNext();
    Node<T> *q = this->head;
    while(p != NULL){
        Node<T> *node = new Node<T>(p->getData());
        q->setNext(node);
        q=q->getNext();
        p=p->getNext();
    }
    this->size = ll.size;
    this->tail = q;

    return *this;
};
template <class T>
LinkedList<T>::~LinkedList(){
    this->clear();
};
template <class T>
Node<T>* LinkedList<T>::getHead() const {
    return this->head;
}
template <class T>
void LinkedList<T>::setHead(Node<T>* n){
    this->head = n;
}
template <class T>
Node<T>* LinkedList<T>::getTail() const {
    return tail;
}
template <class T>
void LinkedList<T>::setTail(Node<T>* n){

    this->tail = n;
}
template <class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const {
    if(this->head->getData() == data) return NULL;
    Node<T> *p(this->head->getNext());
    Node<T> *prev(this->head);
    while(p != NULL){
        if(p->getData() == data) return prev;
        p = p->getNext();
        prev = prev->getNext();
    }
    return this->tail;
}
template <class T>
Node<T>* LinkedList<T>::findNode(const T& data) const {
    Node<T>* p(this->head);
    while(p != NULL){
        if(p->getData() == data)return p;
        p = p->getNext();
    }
    return NULL;
}
template <class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data){
    if(prev == NULL){

        if(this->head == NULL){
            Node<T> *newNode = new Node<T>(data);
            this->head = newNode; 
            this->tail = newNode;}
        else{
            Node<T> *newNode = new Node<T>(data);
            newNode->setNext(this->head);
            this->head = newNode;
        }
    }
    else{
        Node<T> *temp = prev->getNext();
        Node<T> *newnode = new Node<T>(data);
        newnode->setNext(temp);
        prev->setNext(newnode);
        if(this->tail->getNext()) this->tail = this->tail->getNext();
    }
    this->size += 1;
} 
template <class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode){
    if(prevNode == NULL){
        Node<T> *p(this->head);
        this->head = this->head->getNext();
        delete p;
        (this->size) -= 1;
        return;

    }
    if(prevNode->getNext() == NULL) return;     

    Node<T>* temp = prevNode->getNext()->getNext();
    if(temp == NULL) this->tail = prevNode;
    delete prevNode->getNext();
    prevNode->setNext(temp);
    (this->size)-=1;
}  
template <class T>
void LinkedList<T>::clear(){
    if(this->head == NULL) return;
    if(this->head!= this->tail)
    while( this->head->getNext() != this->tail){deleteNode(this->head);}
    deleteNode(this->head);
    delete this->head;
    this->head = NULL;
    this->tail = NULL;
}
template <class T>
bool LinkedList<T>::isEmpty() const {
    return *(this->head);
}
template <class T>
size_t LinkedList<T>::getSize() const {
    return this->size;
}
/* end of your implementations*/

#endif
