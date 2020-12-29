#ifndef _LOOK_H__
#define _LOOK_H__
#include "includes.h"
void NewLook(char character,int attr,const char* name,const char* description,const char* flavor);
struct look* FindLook(const char* name);
#endif
