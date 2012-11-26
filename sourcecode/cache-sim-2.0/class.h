/* 
 * File:   class.h
 * Author: arink
 *
 * Created on 19 October, 2010, 5:22 PM
 */

#ifndef CLASS_H
#define	CLASS_H

/* ADRRESS
 ---------------------------------------*/
class Address{
private:
    unsigned int tagbit,indexbit,blockoffsetbit;
public:
    unsigned int tag,index,block_offset,byte_offset;
    void decode(address address,int blocksize,int No_of_sets);
    unsigned int rentAddress(unsigned int add_tag,unsigned int add_index,unsigned int add_offset);
};


/* MEMORY
 ---------------------------------------*/
class Memory{
    word Word[1048600];
public:
    int blocksize;                         //in words
    Memory();
    Block load(address address);
    void store(Block block,address address);
};


/*Cache Structure*/

/* BLOCK
 ---------------------------------------*/
class Block{
public:
    unsigned int tag,age_offset;
    bool valid;
    word Word[150];
    Block(){
       age_offset = 0;
       tag = 0;
       valid = false;
   }
};


/* CACHE
 ---------------------------------------*/
class Cache {
private:
    Block **T;
    int Capacity,Blocksize,Associativity,No_of_sets,*Age_counter;
   
public:
    Memory Main;
    Cache();
    Cache(int cap,int blocksize,int associativity);
    virtual ~Cache();

    bool write(word Data,address address);
    bool read(word &Data,address address);

    Block rentblock(int i,int j){
        return T[i][j];
    };
};






#endif	/* CACHE_H */

