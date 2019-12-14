#include "Article.h"
using namespace std;
/*#############################
#               NOTE!         #
#    Please don't forget to   #
#     check PDF. Fuctions     #
#    may have more detailed   #
#     tasks which are not     #
#       mentioned here.       #
#############################*/
/*    int n; // Current number of the existing entries in hash table
    int table_size;
    int h1_param; 
    int h2_param;
    std::pair<std::string, int>* table;
    */
Article::Article( int table_size,int h1_param, int h2_param )
{    
    /*#############################
    #            TO-DO            #
    # Write a default constructor #
    #   for the Article Class     #
    #############################*/
    this->table_size = table_size;
    this->h1_param = h1_param;
    this->h2_param = h2_param;
    this->table = new pair<string,int>[table_size];
    for(int i = 0; i != table_size; ++i){
        (this->table)[i] = pair<string,int>("",-1);
    }
    this->n  = 0;
}

Article::~Article()
{
    /*#############################
    #             TO-DO           #
    #  Write a deconstructor for  #
    #     the Article Class       #
    #############################*/
    delete[] table;
}

int Article::get( std::string key, int nth, std::vector<int> &path ) const
{
    /*#############################################
    #                    TO-DO                    #
    #      Your get function should return        #
    # the original index (index in the text file) #
    #    of the nth 'key' in the hash table.      #
    #    If there is no such a key, return -1     #
    #    If there is, return the original index   #
    #     In both cases, you need to push each    #
    #          visited index while probing        #
    #           that you calculated until         #
    #      finding the key, to the path vector.   #
    #############################################*/
    int occurence = 0;
    int i = 0;
    while(i < table_size){
        int Key = hash_function(key,i);
        if(i> 0)path.push_back(Key);
        if(table[Key].second == -1)return -1;
        if(table[Key].first == key){
            ++occurence;       
            if(occurence == nth){
                return table[Key].second;
            }
        }

        i++;

}

    return -1;
}

int Article::insert( std::string key, int original_index )
{
    /*#########################################
    #                 TO-DO                   #
    #      Insert the given key, with the     #
    # original index value (at the text file) #
    #           to the hash table.            #
    #  Return the total number of probes you  #
    #      encountered while inserting.       #
    #########################################*/
    if(getLoadFactor() >= MAX_LOAD_FACTOR)expand_table();
    int i = 0;
    int hash = hash_function(key,i);
    while(table[hash].second != -1 && table[hash].second != MARKED_INDEX){
    	if(table[hash].first == key){
    		if(table[hash].second > original_index){
    			insert(table[hash].first,table[hash].second);
    			table[hash].second = original_index;
    			return i;
    		}
    	}
        ++i;
        hash = hash_function(key,i);
    }
    table[hash].first = key;
    table[hash].second = original_index;
    ++n;
    return i;
}


int Article::remove( std::string key, int nth )
{
    int i = 0;
    int occurence = 0;
    while(i < table_size){
        int Key = hash_function(key,i);
        if(table[Key].first == key){
            ++occurence;
            if(occurence == nth){
                table[Key].first = EMPTY_KEY;
                table[Key].second = MARKED_INDEX;
                --n;
                return i;
            }
        }
        else if(table[Key].first == EMPTY_KEY && table[Key].second == -1)return -1;
        ++i;
    }
    /*#########################################
    #                  TO-DO                  #
    #      Remove the nth key at the hash     #
    #                  table.                 #
    #  Return the total number of probes you  #
    #      encountered while inserting.       #
    #   If there is no such a key, return -1  #
    #     If there, put a mark to the table   #
    #########################################*/
    
    return -1;
}

double Article::getLoadFactor() const
{
    /*#########################################
    #                TO-DO                    #
    #      Return the load factor of the      #
    #                table                    #
    #########################################*/
    return double(n)/table_size;
}

void Article::getAllWordsFromFile( std::string filepath )
{
    ifstream f;
    f.open(filepath.c_str());
    if(!f.is_open()){
        return;}

    else{
        string key;
        int index = 1;
        key.clear();
        while(f >> key){
            insert(key,index);
            index++;
            key.clear();
            
        }
    }

    /*#########################################
    #                  TO-DO                  #
    #       Parse the words from the file     #
    #      word by word and insert them to    #
    #                hash table.              #
    #   For your own inputs, you can use the  #
    #  'inputify.py' script to fix them to    #
    #            the correct format.          #
    #########################################*/
}

void Article::expand_table()
{
    /*#########################################
    #                  TO-DO                  #
    #   Implement the expand table function   #
    #   in order to increase the table size   #
    #   to the very first prime number after  #
    #      the value of (2*table size).       #
    #         Re-hash all the data.           #
    #       Update h2_param accordingly.      #
    #########################################*/
    int old = n;
    pair<string,int> *tabnew = new pair<string,int>[n];
    for(int i = 0,j = 0; i < table_size; ++i){
        if(table[i].second != -1 && table[i].second != MARKED_INDEX){
            tabnew[j] = pair<string,int>(table[i].first,table[i].second);
            j++;}
    }
    table_size = nextPrimeAfter(2 * table_size);
    h2_param = firstPrimeBefore(table_size);
    delete[] table;
    n = 0;
    table = new pair<string,int>[table_size];
    for(int i = 0; i < table_size; ++i)table[i] = pair<string,int>("",-1);
    for(int i = 0; i < old; ++i){
        insert(tabnew[i].first,tabnew[i].second);
    }

}

int Article::hash_function( std::string& key, int i ) const
{
    /*#########################################
    #                TO-DO                    #
    #       Implement the main hashing        #
    #    function. Firstly, convert key to    #
    #    integer as stated in the homework    #
    #      text. Then implement the double    #
    #            hashing function.            #
    #      use convertStrToInt function to    #
    #      convert key to a integer for       #
    #         using it on h1 and h2           #
    #               reminder:                 #
    #            you should return            #
    #    ( h1(keyToInt) + i*h2(keyToınt) )    #
    #            modulo table_size            #
    #########################################*/
    int conv = convertStrToInt(key);
    return (h1(conv) + i*h2(conv))%this->table_size;
}


int Article::h1( int key ) const
{
    /*###############################
    #              TO-DO            #
    #      First Hash function      #
    # Don't forget to use h1_param. #
    ###############################*/
    string binary = "";
    while(key != 0){
        if(key%2 == 1)binary+="1";
        key /= 2;
    }
    return h1_param*binary.size();

}

int Article::h2( int key ) const
{
    /*###############################
    #              TO-DO            #
    #     Second Hash function      #
    # Don't forget to use h2_param. #
    ###############################*/
    key = key%h2_param;
    return h2_param - key;
}
