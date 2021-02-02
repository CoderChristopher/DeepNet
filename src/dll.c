#include "dll.h"
void NewDll(struct dll** list){
	if(!entrypoint)
		return; 
	SetEntryPoint(EP_NEWDLL);
	*list=malloc(sizeof(struct dll));
	(*list)->next=NULL;
	(*list)->current=NULL;
	(*list)->prev=NULL;
	ResetEntryPoint();
	return;
}
void AppendDll(struct dll* list,void* item){
	SetEntryPoint(EP_APPENDDLL);
	if(!list||!item){
		ResetEntryPoint();
		return;
	}
	void* prev=NULL;
	while(list->next){
		prev=list;
		list=list->next;
	}
	list->next=malloc(sizeof(struct dll));
	list->next->next=NULL;
	list->next->current=NULL;
	list->next->prev=list;
	list->current=item;
	list->prev=prev;
	entitycount++;
	ResetEntryPoint();
	return;
}
void RemoveDll(struct dll** list,void* item){
	if(!list||!item||!*list){
		return;
	}
	SetEntryPoint(EP_REMOVEDLL);
	struct dll* prev=NULL;
	struct dll* iter=*list;
	while(iter->current!=item){
		prev=iter;
		iter=iter->next;
		if(iter==NULL){
			ResetEntryPoint();
			return;
		}
	}
	if(prev==NULL){
		*list=iter->next;
	}else{
		prev->next=iter->next;
	}
	if(iter->next)
		iter->next->prev=prev;
	if(iter)
		free(iter);
	ResetEntryPoint();
	return;
}
struct dll* NextDll(struct dll* entrypoint){
	if(!entrypoint)
		return NULL;
	if(!entrypoint->next)
		return NULL;
	SetEntryPoint(EP_NEXTDLL);
	//I guess in case I add more code this area later
	ResetEntryPoint();
	return entrypoint->next;
}
void FreeDll(struct dll** list){
	if(!list)
		return;
	if(!*list)
		return;
	SetEntryPoint(EP_FREEDLL);
	struct dll* iter=*list;
	while(iter->next){
		iter=iter->next;
	}
	do{
		if(!iter)
			break;
		if(iter->current)
			free(iter->current);
		if(!iter->prev)
			break;
		iter=iter->prev;
		free(iter->next);
	}while(iter->prev);

	ResetEntryPoint();
	return;
}
