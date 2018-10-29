#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include "Node.hpp"

/*You are free to add private/public members inside this template..*/
template <class T>
class MyStack{
    private:
    Node<T> *top;                
    public:
    /*Default constructor*/
    MyStack();                     					
    /*copy constructor*/
    MyStack(const MyStack<T>& rhs);  				
    /*destructor*/
    ~MyStack(); 
    /*overloaded = operator*/
    MyStack<T>& operator=(const MyStack<T>& rhs);  	
    /*returns true if stack is empty*/
    bool isEmpty() const;
    /*push newItem to stack*/
    void push(const T& newItem);
    /*pop item from stack*/
    void pop();
    /*return top item of the stack*/
    Node<T>* Top() const;
	/*Prints the stack entries. This method was already implemented. Do not modify.*/
    void print() const;
};

template <class T>
void MyStack<T>::print() const{
    const Node<T>* node = top;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}
template <class T>
MyStack<T>::MyStack(){
    top = NULL;
}
template <class T>
MyStack<T>::MyStack(const MyStack<T>& rhs){
    Node<T> *p(rhs.top);
    if(p == NULL)return;
    MyStack<T> *temp = new MyStack<T>();
    while(p!=NULL){
        temp->push(p->getData());
        p=p->getNext();
    }
    p = temp->top;
    while(p!=NULL){
        this->push(p->getData());
        p=p->getNext();
    }
    delete temp;
}
/* TO-DO: method implementations below */
template <class T>
MyStack<T>& MyStack<T>::operator=(const MyStack<T>& rhs){
    Node<T> *p(rhs.top);
    if(p==NULL) return *this;
    MyStack<T> *temp = new MyStack<T>();

    while(p!=NULL){
        temp->push(p->getData());
        p=p->getNext();
    }
    p = temp->top;

    while(p!=NULL){
        this->push(p->getData());
        p=p->getNext();
    }
    delete temp;
    return *this;
}

template<class T>
MyStack<T>::~MyStack(){
    while(this->top != NULL){this->pop();}
}

template<class T>
bool MyStack<T>::isEmpty() const{
    return this->top;
}

template<class T>
void MyStack<T>::push(const T& newItem){
    Node<T>* newNode = new Node<T>(newItem);
    newNode->setNext(this->top);
    this->top = newNode;
}

template<class T>
void MyStack<T>::pop(){
    if(this->Top() == NULL) return;
    Node<T> *p(this->top->getNext());
    delete this->top;
    this->top = p;
}
template<class T>
Node<T>* MyStack<T>::Top() const{
    return this->top;
}

#endif /* MYSTACK_HPP */

