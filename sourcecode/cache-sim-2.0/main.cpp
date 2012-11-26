/* 
 * File:   main.cpp
 * Author: arink
 *
 * Created on 19 October, 2010, 2:56 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "utility.h"
#include "typedef.h"
#include "class.h"
using namespace std;


char *tocharArray(string A){
        //std::string s
char *a=new char[A.size()+1];
a[A.size()]=0;
memcpy(a,A.c_str(),A.size());
return a;
}

/* DEFAULT BASE VALUES
----------------------------------------------- */
int capacity = 2;		// capacity <kbytes>  2,4,8,16,32,64
int blocksize = 8;		// blocksize <bytes>  8,16,32,64
int associativity = 8;		// assciativity <set> 1,2,4,8



/* MAIN FUNCTION
----------------------------------------------- */
int main(int argc, char *argv[]){
/*Printing command-line*/
    printf("Command-line given : ");  
    stringstream ss;
 

    
    for(char **A = argv;*A != NULL;A++){
        printf("%s ",*A);
        ss << *A;
        ss << " ";
    }
    printf("\n");

   

/*Setting up new base values from commandline   */
   string s2;
   ss>>s2;  
   char *inputfile;
      
	while(ss>>s2){
            char *val;
            char *arg;
            arg = tocharArray(s2);
            int c = decode(arg);
            
            if(!(ss>>s2)){
                if(c==-1){
                inputfile = arg;
                break;
                }
            }else{
                    val = tocharArray(s2);
            }
            
		switch(c){                    
			case 0:
                                
                               
				capacity=atoi(val);
				break;
			case 1:
				blocksize=atoi(val);
				break;
			case 2:
				associativity= atoi(val);
				break;
                        case 3:
                                freopen (val,"w",stdout);
                                break;
                        
			default:
				printf("error: unrecognized command line option \"%s\"\n",arg);
				exit(0);
		}
	}

	
/* Declaration of new Cache L1 */
	Cache L1(capacity,blocksize,associativity);

/* Taking input */
	if(argc>=1){            
                int miss_read=0,miss_write=0,total=0,load=0,store=0;
                double rate=0,rate_read=0,rate_write=0;

                char opt,add[16],dat[16];

/*Reading from file*/
    		fstream fin;
                fin.open(inputfile);                  //opening file from given location
		address address;
                word data=0;

                				
                while(!fin.eof()){
                    fin>>opt;
                    
                    if(opt=='0'){
                        //store
                        fin>>add;
                        if(fin.eof())break;
                        address = hexInt(add);
                        if(L1.read(data,address)==false){
                            miss_read++;
                        }
                        store++;
                    }else if(opt=='1'){
                        //load
                        fin>>add>>dat;
                        if(fin.eof())break;
                        address = hexInt(add);
                        data    = hexInt(dat);
                        bool hit=L1.write(data,address);
                        if(hit==false){
                            miss_write++;
                        }
                        load++;
                    }
                    
                    total++;
                }
                fin.close();				//file close

                printf("****STATISTICS*****\n");
                
                rate = (double)(miss_read+miss_write)*100/total;
                rate_write = (double)(miss_write)*100/total;
                rate_read = (double)(miss_read)*100/total;
                int sets = capacity*1024/(blocksize*associativity);
                
                printf("cache_sets\t\t: %d\n",sets);
                printf("cache_blocksize\t\t: %d\n",blocksize);
                printf("cache_associativity\t: %d\n",associativity);
                
                printf("inst_num\t\t: %d\n",total);                
                printf("inst_load\t\t: %d\n",load);
                printf("inst_store\t\t:  %d\n",store);
                printf("miss_num\t\t: %d\n",miss_read+miss_write ); 
                printf("miss_write\t\t: %d\n",miss_write ); 
                printf("miss_read\t\t: %d\n",miss_read );                                
                printf("miss_rate\t\t: %f\n",rate);
          
              /*  printf("CACHE CONTENTS:\n");
                printf(" Set\twords:\n");
               * int sets = capacity*1024/(blocksize*associativity);
                for(int i=0;i<sets;i++){
                    printf("%5d |",i);
                    
                    for(int j=0;j<associativity;j++){
                        Block B = L1.rentblock(i,j);
                        for(int k=0;k<blocksize/4;k++){
                            if(B.valid==false)
                                printf("%8s ","null");
                            else
                                printf("%8x ",B.Word[k]);
                        }
                        printf("|");
                        
                    }
                    printf("\n");
                }*/
                
                   printf("\n");

	}else
		printf("error:no input\n");

	return 0;        
}

