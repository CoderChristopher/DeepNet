#include "utility.h"
void SetEntryPoint(int ep){
	preventrypoint=entrypoint;
	entrypoint=ep;
}
void ResetEntryPoint(){
	entrypoint=preventrypoint;
	preventrypoint=EP_NIL;
}
int Distance(int x,int y){
	return sqrt(pow(x,2)+pow(y,2));
}
int RandomSign(){
	int output=rand()%2;
	if(output)
		return 1;
	else
		return -1;
}
char WeightedCoinFlip(int weight){
	int result= rand()%(2+weight);
	if(result)
		return 1;
	return 0;
}
char CoinFlip(){
	return rand()%2;
}
void PrintNoSpace(int y,int x,const char* message){
	int len=strlen(message);
	for(int i=0;i<len;i++){
		if(message[i]==' ')
			continue;
		mvprintw(y,x+i,"%c",message[i]);
	}
}
