/*
 * File:   class.cpp
 * Author: arink
 *
 * Created on 19 October, 2010, 5:22 PM
 */
#include "typedef.h"
#include "class.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/* ADDRESS -> DECODE ADDRESS
-----------------------------------------------/
two rightmost bit is reserved for byte offset
bit_block_offset is bits for block offset
bit_index is bits for index
-----------------------------------------------*/
void Address::decode(address address,int blocksize,int No_of_sets){
    unsigned int bit_block_offset = log2(blocksize/4);
    unsigned int bit_index = log2(No_of_sets);
    unsigned int map_filter = All_One;

    tagbit=32-(bit_index+bit_block_offset);
    indexbit=bit_index;
    blockoffsetbit=bit_block_offset;

//Extracting the last two bit for byte offset
//    map_filter = map_filter << 2;
//    byte_offset  = ~map_filter & address;

//    address = address >> 2;
//    map_filter = map_filter >> 2;

//Extracting the bit for block offset
    map_filter = map_filter << bit_block_offset;
    block_offset = ~map_filter & address;

    address = address >> bit_block_offset;
    map_filter = map_filter >> bit_block_offset;

//Extracting the bit for index
    map_filter = map_filter << bit_index;
    index = ~map_filter & address;

//Rest of bit left for tag
    address = address >> bit_index;
    tag = address;

}

/* ADDRESS -> RENT ADDRESS
-----------------------------------------------/
 Build address from tag,index and offset values
-----------------------------------------------*/
address Address::rentAddress(unsigned int add_tag,unsigned int add_index,unsigned int add_offset){

    address address;
/*putting tag*/
    address = add_tag;
/*putting index*/
    address = address<<indexbit;
    address = address + add_index;
/*putting block offset*/
    address = address<<blockoffsetbit;
    address = address + add_offset;
/*putting byte offset*/
   // address = address<<2;
   // address = address + byte_offset;
    return address;
}




/* MEMORY -> DEFAULT CONSTRUCTOR
-----------------------------------------------*/
Memory::Memory(){
    for(int i=0;i<MAXRAM;i++){
        Word[i] = i;
    }
}

/* MEMORY -> STORE
-----------------------------------------------
 store Block from Cache into Memory             */
void Memory::store(Block block, address address){
    int leftalign = address & 1048575;                  //last20bit;
    leftalign = leftalign- leftalign%blocksize;
    for(int i=0;i<blocksize;i++){
        Word[leftalign+i] = block.Word[i];
    }
}

/* MEMORY -> LOAD
-----------------------------------------------
 load and Return Block from Memory into Cache */
Block Memory::load(address address){
    Block B;
    int leftalign = address & 1048575;                  //last20bit;
    leftalign = leftalign- leftalign%blocksize;
    for(int i=0;i<blocksize;i++){
        B.Word[i] = Word[leftalign+i];
    }
    return B;
}







/* DEFAULT CACHE CONSTRUCTOR
-----------------------------------------------*/
Cache::Cache() {
    Capacity = 8*K;
    Blocksize    = 16;
    Associativity    = 4;

    No_of_sets = 128;
    T = new Block*[No_of_sets];
    for(int i=0;i<No_of_sets;i++){
        T[i] = new Block[Associativity];
    }

    Age_counter = 0;
    Main.blocksize = 4;
}


/* ARGUMENTED CACHE CONSTRUCTOR
-----------------------------------------------*/
Cache::Cache(int cap,int blocksize,int associativity) {
    if(!(cap==2 || cap==4 || cap==8 || cap==16 ||cap==32 || cap==64)){
        printf("error: capacity should be integer in <2,4,8,16,32,64>\n");
	exit(0);
    }else
	Capacity = cap*K;

    if(!( blocksize==8 || blocksize==16 || blocksize==32 || blocksize==64 )){
        printf("error: blocksize should be integer in <8,16,32,64>\n");
	exit(0);
    }else
	Blocksize = blocksize;

    if(!(associativity==1 || associativity==2 || associativity==4 || associativity==8)){
	printf("error: set-associativity should be integer in <1,2,4,8>\n");
	exit(0);
    }else
	Associativity = associativity;

    No_of_sets = Capacity/(Blocksize*Associativity);
    T = new Block*[No_of_sets];
    for(int i=0;i<No_of_sets;i++){
        T[i] = new Block[Associativity];
    }
    Age_counter= new int[No_of_sets];
    for(int i= 0;i<No_of_sets;i++){
        Age_counter[i]=0;
    }

    Main.blocksize = Blocksize/4;
}

Cache::~Cache() {
}




/* WRITING DATA INTO CACHE
-----------------------------------------------*/
bool Cache::write(word data,address address){
    Address PC;
    PC.decode(address,Blocksize,No_of_sets);
    bool Hit=false,status = false;

/*looking for tag*/
    for(int i=0;i<Associativity && status == false;i++){
            if(T[PC.index][i].tag == PC.tag){                       //looking for tag
                    if(T[PC.index][i].valid == true){                //is block is valid
                            T[PC.index][i].Word[PC.block_offset] = data;
                            Hit=true;
                    }else{      //fetching from memory
                        T[PC.index][i]= Main.load(address);
                        T[PC.index][i].Word[PC.block_offset] = data;
                    }

                    T[PC.index][i].age_offset = Age_counter[PC.index]++;
                    status = true;
            }
    }

/*looking for invalid if tag not found*/
    if(status ==  false){               //tag not found
            for(int i=0; i<Associativity && status == false; i++){
                    if(T[PC.index][i].valid == false){
                            T[PC.index][i]= Main.load(address);
                            T[PC.index][i].valid = true;
                            T[PC.index][i].tag	 = PC.tag;
                            T[PC.index][i].Word[PC.block_offset]  = data;

                            T[PC.index][i].age_offset = Age_counter[PC.index]++;
                            status = true;
                    }
            }
    }

/*Doing replacement if set is full*/
    if(status == false){            //Replacement
        unsigned int lru = T[PC.index][0].age_offset;

        int b=0;
        for(int k=1 ; k<Associativity; k++){
            if(T[PC.index][k].age_offset<lru){
                lru=T[PC.index][k].age_offset;
                b=k;
            }
        }

        unsigned int storeaddress = PC.rentAddress(T[PC.index][b].tag,PC.index,0);

        Main.store(T[PC.index][b],storeaddress);                             //store into memory

        T[PC.index][b]=Main.load(address);
        T[PC.index][b].valid = true;
        T[PC.index][b].Word[PC.block_offset]  = data;
        T[PC.index][b].age_offset = Age_counter[PC.index]++;
        T[PC.index][b].tag = PC.tag;
        status =true;
    }
    return Hit;
}

/* READING DATA FROM CACHE
-----------------------------------------------*/
bool Cache::read(word &data,address address){
    Address PC;
    PC.decode(address,Blocksize,No_of_sets);
    bool Hit=false,status=false;
/*looking for tag*/
    for(int i=0;i<Associativity;i++){
            if(T[PC.index][i].tag == PC.tag){
                    if(T[PC.index][i].valid == true){
                        Hit  = true;
                        data = T[PC.index][i].Word[PC.block_offset];
                    }else{
                        T[PC.index][i] = Main.load(address);        //fetching from memory
                        data = T[PC.index][i].Word[PC.block_offset];
                        T[PC.index][i].valid = true;
                    }
                    T[PC.index][i].age_offset = Age_counter[PC.index]++;
                    status =  true;
            }
    }

/*looking for invalid if tag not found*/
    if(status ==  false){               //tag not found
            for(int i=0; i<Associativity && status == false; i++){
                    if(T[PC.index][i].valid == false){
                            T[PC.index][i]= Main.load(address);

                            T[PC.index][i].valid = true;
                            T[PC.index][i].tag	 = PC.tag;
                            data = T[PC.index][i].Word[PC.block_offset];

                            T[PC.index][i].age_offset = Age_counter[PC.index]++;
                            status = true;
                    }
            }
    }

/*Doing replacement if set is full*/
    if(status == false){            //Replacement
        unsigned int lru = T[PC.index][0].age_offset;

        int b=0;
        for(int k=1 ; k<Associativity; k++){
            if(T[PC.index][k].age_offset<lru){
                lru=T[PC.index][k].age_offset;
                b=k;
            }
        }

        unsigned int storeaddress = PC.rentAddress(T[PC.index][b].tag,PC.index,0);

        Main.store(T[PC.index][b],storeaddress);                             //store into memory

        T[PC.index][b]=Main.load(address);
        T[PC.index][b].valid = true;
        data = T[PC.index][b].Word[PC.block_offset];
        T[PC.index][b].age_offset = Age_counter[PC.index]++;
        T[PC.index][b].tag = PC.tag;
        status =true;
    }

   return Hit;
}





