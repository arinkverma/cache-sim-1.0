/* 
 * File:   utility.h
 * Author: arink
 *
 * Created on 19 October, 2010, 2:58 AM
 */
#include <string.h>

#ifndef UTILITY_H
#define	UTILITY_H




/* Decode
-----------------------------------------------
It decode command-line option to integer. Because this (state machine)
technique is faster than linear comparison of string
	cap   ==> 0
	block ==> 1
	assoc ==> 2
	else  ==> -1
----------------------------------------------*/
int decode(char command[]){
	enum {	c,ca,cap,
		b,bl,blo,bloc,block,
		a,as,ass,asso,assoc,
		null
	}state=null;
        
        
        
        if(!strcmp(command,"-cap")){
            return 0;
            
        }else if(!strcmp(command,"-block")){
            return 1;
            
        }else if(!strcmp(command,"-assoc")){
            return 2;
            
        }else if(!strcmp(command,"-out")){
            return 3;
            
        }

	return -1;
}



/* binaryInt
-----------------------------------------------
Convert binary string to intger of 32 bit length.
----------------------------------------------*/
unsigned int binaryInt(char b[]){
	unsigned int decimal=0;
	for(int i=0;b[i];i++){
		if(b[i]=='1')
			decimal=(decimal<<1)+1;
		else if(b[i]=='0')
			decimal=decimal<<1;
		else{
			printf("error:not a binary number\n");
			exit(0);
		}
	}
	return decimal;
}


/* hexInt
-----------------------------------------------
Convert binary string to intger of 32 bit length.
----------------------------------------------*/
unsigned int hexInt(char b[]){
    unsigned int decimal=0;
    for(int i=0;b[i+2];i++){
        decimal=decimal<<4;
        if(b[i+2]>='0'&&b[i+2]<='9'){
            decimal = decimal + (int)(b[i]-'0');
        }else if(b[i+2]>='a'&& b[i+2]<='f'){
            decimal = decimal + 10 + (int)(b[i]-'a');
        }else if(b[i+2]>='A'&& b[i+2]<='F'){
            decimal = decimal + 10 + (int)(b[i]-'A');
        }else{
            printf("error:not a binary number\n");
            exit(0);
        }
    }
    return decimal;
}
	

#endif	/* UTILITY_H */

