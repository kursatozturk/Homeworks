#include "bookstore.hpp"

BookStore::BookStore( ) //implemented, do not change
{
}

void
BookStore::insert(const Book & book)
{
   primaryIndex.insert(book.getISBN(),book);
   BSTP::Iterator it = primaryIndex.find(book.getISBN());
   const Book *q = &(*(it));
   secondaryIndex.insert(SKey(*q),q);
   Book *p = &(*(it));
   ternaryIndex.insert(SKey(*q),p);

}

void
BookStore::remove(const std::string & isbn)
{
  BSTP::Iterator it = primaryIndex.find(isbn);
  if(it == primaryIndex.end()) return;
  else{
     ternaryIndex.remove(SKey(*it));
     secondaryIndex.remove(SKey(*it));
     primaryIndex.remove(isbn);

  }
}

void
BookStore::remove(const std::string & title,
                  const std::string & author)
{  
  BSTS::Iterator it = secondaryIndex.find(SKey(title,author));
  if(it == secondaryIndex.end()) return;
  else{
     ternaryIndex.remove(SKey(*(*it)));
     std::string is((*it)->getISBN());
     secondaryIndex.remove(SKey(*(*it)));
     primaryIndex.remove(is);

  }
}

void
BookStore::removeAllBooksWithTitle(const std::string & title)
{
  std::list<std::string> todeleteisbns;
  for(BSTT::Iterator it = ternaryIndex.begin(); it!= ternaryIndex.end(); ++it){
    std::string tit = (*it)->getTitle();
    for(size_t i = 0; i < tit.size(); i++)if('A' <= tit[i] && tit[i] <= 'Z') tit[i] += 'a' - 'A';
    if(tit == title)todeleteisbns.push_back((*it)->getISBN());
  }
  for(std::list<std::string>::iterator i = todeleteisbns.begin(); i != todeleteisbns.end(); ++i){
    this->remove(*i);
  }



}
void
BookStore::makeAvailable(const std::string & isbn)
{
  BSTP::Iterator itp = primaryIndex.find(isbn);
  SKey key((*itp));
  BSTT::Iterator it = ternaryIndex.find(key);
  if(it == ternaryIndex.end())return;
  else{
    (*it)->setAvailable();
  }
}

void
BookStore::makeUnavailable(const std::string & title,
                           const std::string & author)
{
  SKey key(title,author);
  BSTT::Iterator it = ternaryIndex.find(key);
  if(it == ternaryIndex.end())return;
  else{
    (*it)->setUnavailable();
  }
}

void
BookStore::updatePublisher(const std::string & author, 
                           const std::string & publisher)
{
  for(BSTT::Iterator it = ternaryIndex.begin(); it!= ternaryIndex.end(); ++it){
    std::string auth = (*it)->getAuthor();
    for(size_t i = 0; i < auth.size(); i++)if('A' <= auth[i] && auth[i] <= 'Z') auth[i] += 'a' - 'A';
    if(auth == author)(*it)->setPublisher(publisher);
  } 
}

void
BookStore::printBooksWithISBN(const std::string & isbn1,
                              const std::string & isbn2,
                              unsigned short since) const
{
  std::list<BSTP::Iterator> isbns = primaryIndex.find(isbn1,isbn2);
  for(std::list<BSTP::Iterator>::iterator it = isbns.begin(); it != isbns.end(); ++it){
    if((*(*it)).getYear() < since)continue;
    std::cout << *(*it) << std::endl;
  }
}

void
BookStore::printBooksOfAuthor(const std::string & author,
                              const std::string & first,
                              const std::string & last) const
{
  SKey key1(first,author);
  SKey key2(last,author);
  std::list<BSTS::Iterator> isbns = secondaryIndex.find(key1,key2);
  for(std::list<BSTS::Iterator>::iterator it = isbns.begin(); it != isbns.end(); ++it){
    std::cout << *(*(*it)) << std::endl;
  }
}

void //implemented, do not change
BookStore::printPrimarySorted( ) const
{
  BSTP::Iterator it;

  for (it=primaryIndex.begin(); it != primaryIndex.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
}

void //implemented, do not change
BookStore::printSecondarySorted( ) const
{
  BSTS::Iterator it;

  for (it = secondaryIndex.begin(); it != secondaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}

void //implemented, do not change
BookStore::printTernarySorted( ) const
{
  BSTT::Iterator it;

  for (it = ternaryIndex.begin(); it != ternaryIndex.end(); ++it)
  {
    std::cout << *(*it) << std::endl;
  }
}

