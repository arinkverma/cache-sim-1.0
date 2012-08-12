/* 
 * File:   utility.h
 * Author: arink
 *
 * Created on 19 October, 2010, 2:58 AM
 */

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
	for(int i=0;command[i];i++){
		if(command[i]>='A'&&command[i]<='Z'){
			command[i]+=32;
		}
		switch(command[i]){
			case 'a':
				if(state==c)
					state=ca;
				else state=a;
				break;
			case 'b':
				state=b;
				break;
			case 'c':
				if(state==blo)
					state=bloc;
				else if(state==asso)
					state=assoc;
				else
					state=c;
				break;
			case 'k':
				if(state==bloc)
					state=block;
				else state=null;
				break;
			case 'l':
				if(state==b)
					state=bl;
				else state=null;
				break;
			case 'o':
				if(state==bl)
					state=blo;
				else if(state==ass)
					state=asso;
				else state=null;
				break;
			case 'p':
				if(state==ca)
					state=cap;
				else state=null;
				break;
			case 's':
				if(state==a)
					state=as;
				else if(state==as)
					state=ass;
				else state=null;
				break;
			default:
				state=null;
				break;
		}
	}
	if(state==cap) return 0;
	else if(state==block) return 1;
	else if(state==assoc) return 2;
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
    for(int i=0;b[i];i++){
        decimal=decimal<<4;
        if(b[i]>='0'&&b[i]<='9'){
            decimal = decimal + (int)(b[i]-'0');
        }else if(b[i]>='a'&& b[i]<='f'){
            decimal = decimal + 10 + (int)(b[i]-'a');
        }else if(b[i]>='A'&& b[i]<='F'){
            decimal = decimal + 10 + (int)(b[i]-'A');
        }else{
            printf("error:not a binary number\n");
            exit(0);
        }
    }
    return decimal;
}
	

#endif	/* UTILITY_H */

