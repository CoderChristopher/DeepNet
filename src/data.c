#include "data.h"
//Create a new global data item in the data dll
//@param name- name of the data item, type- unique identifies data as belonging to 
//a particular category, value- the worth in money of the data
//@return returns a pointer to the data
struct data* NewData(const char* name,char type,int value){
	SetEntryPoint(EP_NEWDATA);
	if(!datalist){
		NewDll(&datalist);
	}

	struct data* datum=malloc(sizeof(struct data));
	memcpy(datum->name,name,64);
	datum->type=type;
	datum->value=value;

	AppendDll(datalist,datum);

	ResetEntryPoint();
	return datum;
}
