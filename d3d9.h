#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<stddef.h>
#include<string.h>
#include <windows.h>
#include <math.h>
#include <cstdlib>
#define addr1 ((void*)0x144768E78)
#define addr2 ((void*)0x144740178)
#define addr3 ((void*)0x144743AB0)
#define addr4 ((void*)0x144766CA0)//48 8B CE 48 8B 6C 24 48 48 83 C4 20 5E
#define addr7 ((void*)0x144768F68)
#define NetBase ((void*)0x144780990)
#define ParamBase ((void*)0x144782838)
#define Control_2 ((char*)0x144768F98)
#define camrttn ((void*)0x140516BAE)
#define IMPORT(a,i) do{extern void*a##_;a##_=(void*)GetProcAddress(BaseAddressGenuine,#a);if(!a##_)a##_=(void*)GetProcAddress(BaseAddressGenuine,(char*)i);}while(0)
#define call(f,...) (((intptr_t(*)(uintptr_t,...))(f))((uintptr_t)__VA_ARGS__))
#define putstr(format, ...)\
    do {char s##__LINE__[512];DWORD c;\
        c=_snprintf_l(s##__LINE__,510,format,0,##__VA_ARGS__);s##__LINE__[c]='\r';s##__LINE__[c+1]='\n';\
        AllocConsole();WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),s##__LINE__,c+2,&c,0);\
    } while(0)
#define STEAMID_BROADCAST 601248
#define M_PI 3.14159265358979323846  /* pi */
//param
#define GetAttackAddress(id) GetParamAddress(0x18,id)
#define GetBulletAddress(id) GetParamAddress(0x48,id)
#define GetRingAddress(id) GetParamAddress(0xb8,id)
#define GetGoodAddress(id) GetParamAddress(0xc0,id)
#define GetArmorAddress(id) GetParamAddress(0xc8,id)
#define GetWeaponAddress(id) GetParamAddress(0xd0,id)
#define GetMagicAddress(id) GetParamAddress(0x1a0,id)
#define GetEffectAddress(id) GetParamAddress(0x2a8,id)
#define GetCeremonyAddress(id) GetParamAddress(0x60,id)
#define ModifyBullet(id,offset,type,...)do{static const type data[]={__VA_ARGS__};if(char*p=GetBulletAddress(id))memcpy(p+offset,data,sizeof data);}while(0)
#define ModifyEffect(id,offset,type,...)do{static const type data[]={__VA_ARGS__};if(char*p=GetEffectAddress(id))memcpy(p+offset,data,sizeof data);}while(0)
#define ModifyGood(id,offset,type,...)do{static const type data[]={__VA_ARGS__};if(char*p=GetGoodAddress(id))memcpy(p+offset,data,sizeof data);}while(0)
#define ModifyRing(id,offset,type,...)do{static const type data[]={__VA_ARGS__};if(char*p=GetRingAddress(id))memcpy(p+offset,data,sizeof data);}while(0)
#define ModifyCeremony(id,offset,type,...)do{static const type data[]={__VA_ARGS__};if(char*p=GetCeremonyAddress(id))memcpy(p+offset,data,sizeof data);}while(0)
#define ModifyWeapon(id,offset,type,...)do{static const type data[]={__VA_ARGS__};if(char*p=GetWeaponAddress(id))memcpy(p+offset,data,sizeof data);}while(0)
//param
#define KEY_FART 1
#define KEY_POOPSTORM 2
#define KEY_LOTHRICLIGHT 3


EXTERN_C double(*__imp_sin)(double); EXTERN_C double(*__imp_cos)(double);
struct bullet;
PBYTE DarkSoulsIII;
int AddBulletSpeWrite(const bullet& bl, unsigned pos);
int AddBulletSpeWrite(int pos, int handle, int bid, int quantity, uint32_t interval, int key, unsigned starttime = GetTickCount(), int target = -1, int serverside = 9, int turret = 0);
int AddBulletSpeIdle(const bullet& bl, unsigned pos);
int AddBulletSpeIdle(int pos, int handle, int bid, int quantity, uint32_t interval, int key, unsigned starttime = GetTickCount(), int target = -1, int serverside = 9, int turret = 0);
static bool lothricswitch[6] = {};