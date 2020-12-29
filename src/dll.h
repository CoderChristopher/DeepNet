#ifndef __DLL_H__
#define __DLL_H__
#include "includes.h"
void NewDll(struct dll** entrypoint);
void AppendDll(struct dll* list,void* item);
void RemoveDll(struct dll** list,void* item);
struct dll* NextDll(struct dll* entrypoint);
#endif
