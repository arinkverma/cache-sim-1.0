/***
	WorkLoad generator
	@author : www.arinkverma.in

-seq < length = % of ins * repeat ,:... >
-ins < total # of instruction >
-load < approx % of load instruction >
Example : ./workgen -seq 10:20*2:10:20*2 -ins 100000 -load 20
Address trace will have 10000 instructions then 20000 instructions in a loop 2 times. Again
 10000 instructions then 20000 instructions in a loop 2 times. And load instructions approx 20 %.
 */


#include <iostream>
#include <queue>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

using namespace std;

//Arguments
queue< pair <float,int> > SeqPart;
unsigned int InstNum;
int loadPercent;

//Stats
int NumLoad,NumStore;


char *tocharArray(string A){
        //std::string s
char *a=new char[A.size()+1];
a[A.size()]=0;
memcpy(a,A.c_str(),A.size());
return a;
}



void storeSeqPattern(char *value){
	if(value){
		char buffer[400];

		int len = strlen(value);
		float check = 0;

		int s=0;
		for(int i=0;i<=len;i++){
			if(value[i]==':' || i==len){
				buffer[s++] = '\0';
				float seqlen = 0;
				int repeat = 1;
				sscanf (buffer,"%f*%d",&seqlen,&repeat);
				pair <float,int> blockUnit = make_pair (seqlen,repeat);
				SeqPart.push(blockUnit);
				check += seqlen*repeat;
				s = 0;
			}else{
				buffer[s++] = value[i];
			}
		}

		if(check < 100){
			pair <float,int> blockUnit = make_pair (100-check,1);
			SeqPart.push(blockUnit);
		}else if(check > 100){
			printf("error\n");
		}

	}else{
		printf("error\n");
	}
}


vector< pair <unsigned int,unsigned int> > AddSeq;

void genAdd(unsigned int start,int length){
	AddSeq.clear();
	for(int i=start;i<start+length;i++){
		float loadProbablity = (rand() % 100) + 1;
		unsigned int data = 0;
		if(loadProbablity < loadPercent){
			data = rand()%10000+1;
		}
		AddSeq.push_back(make_pair (i,data));
	}
}


void writeTrace(FILE * pFile){
	for(int i=0;i<AddSeq.size();i++){
		pair <unsigned int,unsigned int> a = AddSeq[i];
		if(a.second>0){
			fprintf(pFile, "1 %08x %08x\n",a.first,a.second);
			NumLoad++;
		}else {
			fprintf(pFile, "0 %08x\n",a.first);
			NumStore++;
		}
	}
}



int main(int argc, char *argv[]){

	stringstream ss;

	printf("Command-line given : ");    
    for(char **A = argv;*A != NULL;A++){
    	char *cmd = *A;
        printf("%s ",cmd);
        ss << *A;
        ss << " ";
    }

    printf("\n");


    int load =0 ;

    string s2;
   	ss>>s2;  
   	char *SeqPattern;
   	while(ss>>s2){
            char *value;
            char *cmd;
            cmd = tocharArray(s2);
            if(!strcmp(cmd,"-seq")){
            	ss>>s2;
            	SeqPattern = tocharArray(s2);
        		storeSeqPattern(SeqPattern);        		
        	}else if(!strcmp(cmd,"-ins")){
            	ss>>s2;
            	value = tocharArray(s2);
        		InstNum = atoi(value);        		
        	}else if(!strcmp(cmd,"-load")){
            	ss>>s2;
            	value = tocharArray(s2);
        		loadPercent = atoi(value);        		
        	}           
				
	}

    //loadPercent = InstNum/load;


    unsigned int Add = 0;

    FILE * pFile;
    pFile = fopen (SeqPattern,"w");

    NumLoad = 0;
    NumStore = 0;
    
    while (!SeqPart.empty()){
		pair <float,int> v = SeqPart.front();
		float seqPart = v.first;
		int repeat = v.second;
		int length = (seqPart*InstNum)/100;
		genAdd(Add,length);
		for(int i=0;i<repeat;i++){
			writeTrace(pFile);
		}

		Add += length ;
		SeqPart.pop();
	}

	printf("Num of Loads: %d\nNum of Store: %d\n",NumLoad,NumStore);

	return 0;
}