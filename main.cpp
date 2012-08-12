/* 
 * File:   main.cpp
 * Author: arink
 *
 * Created on 19 October, 2010, 2:56 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "utility.h"
#include "typedef.h"
#include "class.h"
using namespace std;


/* DEFAULT BASE VALUES
----------------------------------------------- */
int capacity = 8;		// capacity <kbytes>  4,8,16,32,64
int blocksize = 16;		// blocksize <bytes>  4,8,16,32,64,128,256,512
int associativity = 4;		// assciativity <set> 1,2,4,8,16



/* MAIN FUNCTION
----------------------------------------------- */
int main(int argc, char *argv[]){
/*Printing command-line*/
    printf("Command-line given:\n");    
    for(char **A = argv;*A != NULL;A++){
        printf("%s ",*A);
    }
    printf("\n");


/*Setting up new base values from commandline   */
	while(--argc > 1 && (*++argv)[0] == '-'){
		switch(decode(argv[0])){
			case 0:
				capacity=atoi(*++argv);
				argc--;
				break;
			case 1:
				blocksize=atoi(*++argv);
				argc--;
				break;
			case 2:
				associativity= atoi(*++argv);
				argc--;
				break;
			default:
				printf("error: unrecognized command line option \"%s\"\n",argv[0]);
				exit(0);
		}
	}

	
/* Declaration of new Cache L1 */
	Cache L1(capacity,blocksize,associativity);

/* Taking input */
	if(argc>=1){            
                int miss_read=0,miss_write=0,total=0;
                double rate=0,rate_read=0,rate_write=0;

                char opt,add[16],dat[16];

/*Reading from file*/
    		fstream fin;
                fin.open(*++argv);                  //opening file from given location
		address address;
                word data=0;

                				
                while(!fin.eof()){
                    fin>>opt;
                    
                    if(opt=='0'){
                        fin>>add;
                        if(fin.eof())break;
                        address = hexInt(add);
                        if(L1.read(data,address)==false){
                            miss_read++;
                        }
                    }else if(opt=='1'){
                        fin>>add>>dat;
                        if(fin.eof())break;
                        address = hexInt(add);
                        data    = hexInt(dat);
                        bool hit=L1.write(data,address);
                        if(hit==false){
                            miss_write++;
                        }
                    }
                    
                    total++;
                }
                fin.close();				//file close

                printf("STATISTICS:\n");
                printf("\tMisses:\t\t%d\t\t%d\t\t%d\n",miss_read+miss_write,miss_read,miss_write );
                
                rate = (double)(miss_read+miss_write)*100/total;
                rate_write = (double)(miss_write)*100/total;
                rate_read = (double)(miss_read)*100/total;
                printf("\tMiss Rate:\t%f\t%f\t%f\n",rate,rate_read,rate_write );
                printf("\tNumber of Dirty Blocks Evicted From the Cache:\n");
                printf("CACHE CONTENTS:\n");
                printf(" Set\twords:\n");

                int sets = capacity*1024/(blocksize*associativity);
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
                }
                
                   printf("\n");

	}else
		printf("error:no input\n");

	return 0;        
}

