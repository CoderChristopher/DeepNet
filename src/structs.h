#ifndef __STRUCT__H__
#define __STRUCT__H__
#include "includes.h"
struct room;
struct dll{
	struct dll* prev;
	struct dll* next;
	void* current;
};
struct look{
	char character;
	int nattribute;
	char* name;
	char* description;
	char* flavor;
};
struct software{
	char name[8];
	unsigned int value;
	char type;
};
/*struct action{
	char type;
	char stack[16];
};*/
struct tile{
	struct look* apperance;
	BOOLEAN passable;
	BOOLEAN seen;
};
struct data{
	char type;
	char name[64];
	int value;
};
struct actor{
	struct look*      apperance;
	struct software* attack;
	struct software* defense;
	struct room*     currentroom;
	struct dll*      data; 

	int x;
	int y;

	BOOLEAN seen;

	char health;
	char shield;

	char reaction;
	char stealth;
	char speed;

	char speedcounter;

	void (*actiondecisionfunction)(struct actor*);
	void (*collisionresolver)(struct actor*,struct actor*);
	void (*undo)(struct actor*);
	void (*act)(struct actor*);

	char action;
	char stack[12];
};
struct interactable{
	struct look* apperance;
	int x;
	int y;
	char type;
	void (*collisionresolver)(struct interactable*,struct actor*);
	char stack[INTERACTABLE_STACKSIZE];
};
struct room{
	struct tile* tiles;
	struct dll* actors;
	struct dll* interactables;
	char sizex;
	char sizey;
};
struct map{
	struct dll* rooms;
	BOOLEAN fogofwar;
	BOOLEAN missionsuccess;
};
#endif
