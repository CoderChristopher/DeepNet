#include "look.h"
void NewLook(char character,int attribute,const char* name,const char* description,const char* flavor){
	if(!name||!description)
		return;
	SetEntryPoint(EP_NEWLOOK);

	if(!looklist)
		NewDll(&looklist);
	struct look* look=malloc(sizeof(struct look));
	AppendDll(looklist,look);
	look->character=character;
	look->nattribute=attribute;
	look->name=malloc(DEFAULT_STRING_SIZE);
	look->description=malloc(DEFAULT_STRING_SIZE);
	look->flavor=malloc(DEFAULT_STRING_SIZE);
	strncpy(look->name,name,DEFAULT_STRING_SIZE);
	strncpy(look->description,description,DEFAULT_STRING_SIZE);
	strncpy(look->flavor,flavor,DEFAULT_STRING_SIZE);

	ResetEntryPoint();
	return;
}
struct look* FindLook(const char* name){
	struct dll* iter=looklist;	
	do{
		if(!iter)
			break;
		if(!iter->current)
			break;
		if(strcmp(((struct look*)iter->current)->name,name))
			continue;
		return iter->current;
	}while(iter=NextDll(iter));
	return NULL;
}
