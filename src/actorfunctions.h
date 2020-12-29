#ifndef __ACTORFUNCTIONS_H__
#define __ACTORFUNCTIONS_H__
#include "includes.h"
void CalculateDamages(struct actor* a1,struct actor* a2);
void DefaultCollisionBehavior(struct actor* self,struct actor* opponent);
void DefaultActBehavior(struct actor* self);
struct actor* NewActor(struct look* look,int x,int y,struct room* rm,void* adf,void* cr,void* action);
void RemoveActor(struct actor* remove);
void GiveAttack(struct actor* act,const char name[8],unsigned int value);
void GiveDefense(struct actor* act,const char name[8],int value);
void KillerInput(struct actor* self);
void KillerAct(struct actor* self);
void KillerCollision(struct actor* self,struct actor* opponent);
void SysAdminCollision(struct actor* self,struct actor* opponent);
void SysAdminInput(struct actor* sysad);
void SysAdminAct(struct actor* self);
float PlayerGetTrace();
void PlayerIncrementTrace();
void PlayerSetDeltaTrace(float dt);
float PlayerGetDeltaTrace();
void PlayerSetMoney(int money);
int PlayerGetMoney();
void PlayerInput(struct actor* player);
void PlayerAct(struct actor* self);
void PlayerCollision(struct actor* self,struct actor* opponent);
#endif
