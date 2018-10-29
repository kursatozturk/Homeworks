#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Playlist.hpp"
#include "Entry.hpp"

using namespace std;


Playlist::Playlist()
{
    srand(15);
}

int Playlist::getRandomNumber(int i, int n) const
{
    int range=n-i;
    int random = rand() % range + i;
    return random;
}

void Playlist::print()
{
    cout << "[PLAYLIST SIZE=" << entries.getSize() <<"]";
    entries.print();
}

void Playlist::printHistory()
{
    cout<<"[HISTORY]";
    history.print();
}

void Playlist::load(string fileName){
	string line = string();
	ifstream l(fileName.c_str());
    while(getline(l,line)){
        string Title = string();
        string::iterator x = line.begin();
       
        while( x != line.end()){
        	if(*x == ';')break;
        	Title += *x;
        	x++;
        }
        string Genre = string();
        x++;
       
        while( x != line.end()){
        	if(*x == ';')break;
        	Genre += *x;
        	x++;
        }
        string Year = string();
        x++;
        while(x != line.end()){
        	Year += *x;
        	x++;
        }

        Entry addto(Title,Genre,Year);
        this->insertEntry(addto);
        line ="";
    } 

}

void Playlist::insertEntry(const Entry &e){
    this->entries.insertNode(this->entries.getTail(),e);
    HistoryRecord item(INSERT,e);
    this->history.push(item);

}

void Playlist::deleteEntry(const string &_title){
  Entry e(_title);
  if(this->entries.findNode(e)){
  	Entry tohist(this->entries.findNode(e)->getData());
    this->entries.deleteNode(this->entries.findPrev(e));
    HistoryRecord item(DELETE,tohist);
    this->history.push(item);
  }
  else return;
}

void Playlist::moveLeft(const string &title){
    Entry en(title);
    Node<Entry> *p(this->entries.findNode(en));
    if(!p || (p == this->entries.getHead()) )return;
    this->moveRight( (this->entries.findPrev(en))->getData().getTitle());

}

void Playlist::moveRight(const string &title){
    Entry en(title);
    Node<Entry> *p (this->entries.findNode(en));
    if(p && p->getNext()){
        Node<Entry> *q(this->entries.findPrev(en));
        if(q == NULL){
         	Node<Entry> *tmp;
        	tmp = this->entries.getHead();
        	this->entries.setHead(this->entries.getHead()->getNext());
        	tmp->setNext(this->entries.getHead()->getNext());
        	this->entries.getHead()->setNext(tmp);
        	return;
        }
		if(p->getNext() == this->entries.getTail()) this->entries.setTail(p);
        q->setNext(p->getNext());
        p->setNext(q->getNext()->getNext());
        q->getNext()->setNext(p);
    }
}

void Playlist::reverse(){
   Node<Entry> *p(this->entries.getHead());
   Node<Entry> *prev = NULL;
   Node<Entry> *tmp;

   while(p){
   	tmp = p->getNext();
   	p->setNext(prev);
   	prev = p;
   	p = tmp;
   }
   tmp = this->entries.getHead();
   this->entries.setHead(this->entries.getTail());
   this->entries.setTail(tmp);
   HistoryRecord rev(REVERSE);
   this->history.push(rev);
}

void Playlist::sort(){
    Node<Entry> *ptr1(this->entries.getHead());
    Node<Entry> *ptr1prev = NULL;
    Node<Entry> *ptr2;
    Node<Entry> *minprev;
    Node<Entry> *min;
    while(ptr1 && ptr1->getNext()){
      	ptr2 = ptr1;
      	min = ptr2;
      	minprev = this->entries.findPrev(ptr2->getData().getTitle());
      	while(ptr2->getNext()){
      		if(ptr2->getNext()->getData().getTitle() < min->getData().getTitle()){
      			min = ptr2->getNext();
      			minprev = ptr2;
      		}
      		ptr2 = ptr2->getNext();
      	}
      	Node<Entry> * tmp = ptr1->getNext();
      	swap(ptr1prev,minprev);
      	ptr1prev = this->entries.findPrev(tmp->getData());
      	ptr1 = tmp;
    }
    }

void Playlist::merge(const Playlist & pl){
    Node<Entry> *rhs(pl.entries.getHead());
while(rhs){
        Node<Entry> *lhs(this->entries.getHead());
        size_t size = this->entries.getSize();
        Entry copy(rhs->getData().getTitle(),rhs->getData().getGenre(),rhs->getData().getYear());
        while(lhs){
            if(lhs->getData().getTitle() > copy.getTitle()){
                Node<Entry> *prev = this->entries.findPrev(lhs->getData());
                this->entries.insertNode(prev,copy);
                if(prev == NULL)this->entries.setHead(this->entries.findNode(copy));
                break;
            }
            lhs = lhs->getNext();
        }
        if(size == this->entries.getSize()){
            this->entries.insertNode(this->entries.getTail(),copy);

        }
        rhs = rhs->getNext();
    }
    while(this->entries.getTail()->getNext())this->entries.setTail(this->entries.getTail()->getNext());
} 
void Playlist::undo(){
    HistoryRecord toUndo = this->history.Top()->getData();
    this->history.pop();

    switch(toUndo.getOperation()){
        case INSERT:
        this->deleteEntry(toUndo.getEntry().getTitle());
        this->history.pop();
        break;
        case DELETE:
        this->insertEntry(toUndo.getEntry());
        this->history.pop();
        break;
        case REVERSE:
        this->reverse();
        this->history.pop();
        break;
        default:
        //throw error
        break;
    }

}
void Playlist::swap(Node<Entry>* PrevNodei,Node<Entry>* PrevNodej){
	Node<Entry>* tmp1, *tmp2;
	if(PrevNodei == PrevNodej) return;
	if(PrevNodei) tmp1 = PrevNodei->getNext();
	else tmp1 = this->entries.getHead();
	tmp2 = PrevNodej->getNext();
	if(tmp1->getNext() == tmp2){
		this->moveLeft(tmp2->getData().getTitle());
		return;
	}
	while(tmp1->getNext() != tmp2)moveLeft(tmp2->getData().getTitle());
	while(PrevNodej->getNext() != tmp1){
		moveRight(tmp1->getData().getTitle());}
	if(tmp1 == this->entries.getHead())this->entries.setHead(tmp2);

}

void Playlist::shuffle(){
	size_t Size_ = this->entries.getSize();
	for(int i = 0; i < Size_-1; i++){
		int k;
		int j = getRandomNumber(i,Size_);
		Node<Entry> *previ = this->entries.getHead();
		Node<Entry> *prevj = this->entries.getHead();

		for(k = 1; k <= i; k++)previ = previ->getNext();

		for(k = 1; k <= j; k++)prevj = prevj->getNext();
		previ = this->entries.findPrev(previ->getData());
		prevj = this->entries.findPrev(prevj->getData());
		this->swap(previ,prevj);
	

	}

//while tail->next ==> tail = tail->next
} 
/* TO-DO: method implementations below */


