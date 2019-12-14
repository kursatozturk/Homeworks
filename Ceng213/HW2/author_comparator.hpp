#ifndef _author_h__
#define _author_h__

#include "book.hpp"
#include <cstring>

class AuthorComparator
{
  public:
    bool operator( ) (const Book::SecondaryKey & key1, 
                      const Book::SecondaryKey & key2) const
    {
        std::string author1(key1.getAuthor());
        std::string author2(key2.getAuthor());
        size_t size1 = author1.size();
        size_t size2 = author2.size();
        for(size_t i = 0; i < size1; i++){
            if('A' <= author1[i] && author1[i] <= 'Z')author1[i] += 'a' - 'A';    
        }
        for(size_t i = 0; i < size2; i++){
            if('A' <= author2[i] && author2[i] <= 'Z')author2[i] += 'a' - 'A';    
        }
        if(author1 == author2){
            std::string title1(key1.getTitle());
            std::string title2(key2.getTitle());
            size_t size1 = title1.size();
            size_t size2 = title2.size();
            for(size_t i = 0; i < size1; i++){
                if('A' <= title1[i] && title1[i] <= 'Z')title1[i] += 'a' - 'A';   
            }
            for(size_t i = 0; i < size2; i++){
                if('A' <= title2[i] && title2[i] <= 'Z')title2[i] += 'a' - 'A';   
            }

            return title1 < title2;
        }
        else return author1 < author2;
    }

};

#endif

