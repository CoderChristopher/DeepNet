#ifndef __INTERACTABLES_H__
#define __INTERACTABLES_H__
#include "includes.h"
struct interactable* NewInteractable(struct look* look,int x,int y,struct room* rm,void* col,void* stackinit);
char* BuildWarpStruct(struct room* dest,int x,int y);
char* BuildSoftwareStruct(struct software* soft);
char* BuildMoneyStruct(int value);
char* BuildDataStruct(struct data* dest);
void SoftwareCollision(struct interactable* self,struct actor* collision);
void DataCollision(struct interactable* self,struct actor* collision);
void MoneyCollision(struct interactable* self,struct actor* collision);
void WarpCollision(struct interactable* self,struct actor* collision);
void CheckPointCollision(struct interactable* self,struct actor* collision);
void DisconnectCollision(struct interactable* self,struct actor* collision);
#endif
