// dllmain.cpp : Defines the entry point for the DLL application.
#include "d3d9.h"

template<class ResultType, class PointerType>
ResultType* AccessMultilevelPointer(PointerType BaseAddress)
{
	return (intptr_t)BaseAddress < 65536 || IsBadReadPtr((void*)BaseAddress, sizeof(ResultType)) ? 0 : (ResultType*)BaseAddress;
}

template<class ResultType, class PointerType, class OffsetType, class...OffsetTypes>
ResultType* AccessMultilevelPointer(PointerType BaseAddress, OffsetType Offset, OffsetTypes... Offsets)
{
	return (intptr_t)BaseAddress < 65536 || (intptr_t)BaseAddress & (sizeof(void*) - 1) || IsBadReadPtr((void*)BaseAddress, sizeof(void*)) ? 0 : AccessMultilevelPointer<ResultType>(*(char**)BaseAddress + Offset, Offsets...);
}

//param
int cmp(const void* k, const void* e)
{
	return((int)(uintptr_t)k - *(int*)e);
}

char* GetParamAddress(int ParamOffset, int id) {
	char* p; unsigned short c;
	p = *(char**)0x144785FE0;
	p = p ? *(char**)(p + 0x10) : 0;
	p = p ? *(char**)(p + ParamOffset) : 0;
	p = p ? *(char**)(p + 0x68) : 0;
	p = p ? *(char**)(p + 0x68) : 0;
	if (p && (c = *(unsigned short*)(p + 10))) {
		int* e = (int*)bsearch((void*)(uintptr_t)id, p + 0x40, c, sizeof(int) * 6, cmp);
		return e ? p + e[2] : 0;
	}
	else return 0;
}
//param

unsigned GetLockTargetID() {
	char* p, * p2;
	unsigned pid;
	p = *(char**)addr4;
	if (p && p[0x2820]) {
		for (p = *(char**)(p + 8); p; p = *(char**)(p + 0x80))if (p[0x90] & 32) {
			if ((p2 = *(char**)(p + 0x88))) {
				pid = *(unsigned*)(p2 + 100);
				return pid;
			}
		}
	}
	return 601248;
}

char* GetLockTargetAdr()
{
	char* p2;
	p2 = (char*)call(0x1408D0E60, *(char**)addr1, GetLockTargetID());
	if (p2)
		return *(char**)p2;
	return 0;
}

unsigned GetLockTargetIndex() {
	return GetLockTargetID() - 0x10068000;
}



//player
int getplayerid(unsigned handle) {
	return handle >= 0x10068000 ? handle - 0x10068000 : -1;
}
struct playerinfo {
	int anim;
	float x = 0;
	float y = 0;
	float z = 0;
	float dir = 0;
	int handle = 0;
	//member func
	auto fangyangpi() {
		if (anim == 84701 && handle) {
			float ax = __imp_sin(dir);
			float ay = 0;
			float az = __imp_cos(dir);
			AddBulletSpeIdle(getplayerid(handle), handle, 12422000, 998, 1250, KEY_FART);
		}
		return *this;
	}
	void getinfo(unsigned pl) {
		anim = 0; x = 0; y = 0; z = 0; dir = 0; handle = 0;
		int* animp;
		char* dirp;
		if (pl <= 5) {
			animp = AccessMultilevelPointer<int>(0x144768E78, 0x40, 0x38 * pl, 0x1F90, 0x10, 0xB0);
			if (animp)
				anim = *animp;
			dirp = AccessMultilevelPointer<char>(0x144768E78, 0x40, 0x38 * pl, 0x18, 0x28, 0x74);
			if (dirp) {
				dir = *(float*)dirp;
				x = *(float*)(dirp + 0xC);
				y = *(float*)(dirp + 0x10);
				z = *(float*)(dirp + 0x14);
				handle = 0x10068000 + pl;
			}
		}
	}
	//constructor
	playerinfo() = default;
	playerinfo(unsigned pl) {
		int* animp;
		char* dirp;
		if (pl <= 5) {
			animp = AccessMultilevelPointer<int>(0x144768E78, 0x40, 0x38 * pl, 0x1F90, 0x10, 0xB0);
			if (animp)
				anim = *animp;
			dirp = AccessMultilevelPointer<char>(0x144768E78, 0x40, 0x38 * pl, 0x18, 0x28, 0x74);
			if (dirp) {
				dir = *(float*)dirp;
				x = *(float*)(dirp + 0xC);
				y = *(float*)(dirp + 0x10);
				z = *(float*)(dirp + 0x14);
				handle = 0x10068000 + pl;
			}
		}
	}
};

//player
//packet
uint64_t GetSteamID(const unsigned playerindex) {
	if (playerindex > 0 && playerindex < 6) {
		char* padr = AccessMultilevelPointer<char>(addr1, 0x40, 0x38 * playerindex);
		char* netbase = *(char**)NetBase, * p; uint64_t*** conn;
		if (padr && (padr = *(char**)padr) && netbase && (conn = (uint64_t * **)(*(char**)(netbase + 0x18) + 0x68))) if (conn[0]) {
			for (unsigned i = 0; conn[0] + i < conn[1]; ++i) {
				if (conn[0][i] && conn[0][i][13] == *(uint64_t*)(8 + *(char**)(padr + 0x1fd0))) {
					return conn[0][i][25];
				}
			}
		}
	}
	return STEAMID_BROADCAST;
}

void RealSendPockage(const void* plnetbase, const void* pdata, const unsigned outterlength) {
	char junk[256];
	call(DarkSoulsIII + 0x1954CE0, junk);
	call(DarkSoulsIII + 0x196F690, *(char**)(*(char**)0x144780990 + 0x18), 0, junk);
	call(0x14198e670, plnetbase, junk, pdata, outterlength);
}

void SendPockageOuter(const void* pdata, const unsigned length, const uint64_t steamid = STEAMID_BROADCAST) {
	char* netbase = *(char**)NetBase, * p; uint64_t*** conn;
	if (netbase && (conn = (uint64_t * **)(*(char**)(netbase + 0x18) + 0x68))) if (conn[0]) {
		if (steamid < conn[1] - conn[0]) {
			RealSendPockage(conn[0][steamid], pdata, length);
		}
		else
			for (unsigned i = 0; conn[0] + i < conn[1]; ++i)
				if (conn[0][i] && (steamid == STEAMID_BROADCAST || steamid == conn[0][i][25])) {
					putstr("Send to: %p", conn[0][i][25]);
					RealSendPockage(conn[0][i], pdata, length);
				}
	}
}

void SendPockageInner(const unsigned id, const void* pdata, const unsigned length, const uint64_t steamid = STEAMID_BROADCAST) {
	char buf[2048];
	*(unsigned*)buf = id;
	*(unsigned*)(buf + 4) = length;
	memcpy(buf + 8, pdata, length);
	SendPockageOuter(buf, length + 8, steamid);
}


//packet
//b array


void shootbullet(int handle, int bid, float bx, float by, float bz, float ax, float ay, float az, int target = -1, int serverside = 9, int turret = 0) {
	char buf[512];
	memset(buf, 0, sizeof buf);
	*(int*)(buf + 0x30) = handle;
	*(int*)(buf + 0x40) = bid;
	*(int*)(buf + 0x48) = turret;
	*(int*)(buf + 0x6C) = serverside;
	*(float*)(buf + 0x7C) = bx;
	*(float*)(buf + 0x8C) = by;
	*(float*)(buf + 0x9C) = bz;
	*(float*)(buf + 0x78) = ax;
	*(float*)(buf + 0x88) = ay;
	*(float*)(buf + 0x98) = az;

	call(0x140978500, *(char**)0x144772D78, buf, buf + 0x30);
}

struct bullet {
	int bid, target, serverside, turret;
	unsigned handle;
	int quantity;
	uint32_t starttime;
	uint32_t interval;
	int key;
	//constructors
	bullet() = default;
	bullet(int handle, int bid, int quantity, uint32_t interval, int key, unsigned starttime = GetTickCount(), int target = -1, int serverside = 9, int turret = 0) : handle(handle), bid(bid), quantity(quantity), interval(interval), key(key), starttime(starttime), target(target), serverside(serverside), turret(turret) {}
};

static bullet bulletqueue[1024]; //for special slots, next time start from 2*6

int AddBullet(const bullet& bl) {
	for (int i = 0; i < _countof(bulletqueue) / 2; ++i) {//max slots
		if (bulletqueue[i].quantity == 0) {
			bulletqueue[i] = bl;
			return i;
		}
	}
	return -1;
}
int AddBullet(int handle, int bid, int quantity, uint32_t interval, int key, unsigned starttime = GetTickCount(), int target = -1, int serverside = 9, int turret = 0) {
	return AddBullet(bullet(handle, bid, quantity, interval, key, starttime, target, serverside, turret));
}

int AddBulletSpeIdle(const bullet& bl, unsigned pos) {
	pos = _countof(bulletqueue) / 2 + pos;
	if (bulletqueue[pos].quantity == 0) {
		bulletqueue[pos] = bl;
		return pos;
	}
	return -1;
}
int AddBulletSpeIdle(int pos, int handle, int bid, int quantity, uint32_t interval, int key, unsigned starttime, int target, int serverside, int turret) {
	pos = _countof(bulletqueue) / 2 + pos;
	if (bulletqueue[pos].quantity == 0) {
		bulletqueue[pos] = bullet(handle, bid, quantity, interval, key, starttime, target, serverside, turret);
		return pos;
	}
	return -1;
}

int AddBulletSpeWrite(const bullet& bl, unsigned pos) {
	pos = _countof(bulletqueue) / 2 + pos;
	bulletqueue[pos] = bl;
	return pos;
}
int AddBulletSpeWrite(int pos, int handle, int bid, int quantity, uint32_t interval, int key, unsigned starttime, int target, int serverside, int turret) {
	pos = _countof(bulletqueue) / 2 + pos;
	bulletqueue[pos] = bullet(handle, bid, quantity, interval, key, starttime, target, serverside, turret);
	return pos;
}

void queueshoot(bullet& bl) {
	int i;
	playerinfo pl;
	static float poopradius = 2.5, height = 0;
	static bool poopradiusflag = 0;
	switch (bl.key) {
	case KEY_FART: //ass
		i = getplayerid(bl.handle);
		if (i != -1) {
			pl.getinfo(i);
			if (pl.anim == 84701) {//patches squat
				//shoot
				float ax = __imp_sin(pl.dir);
				float ay = 0;
				float az = __imp_cos(pl.dir);
				shootbullet(pl.handle, bl.bid, pl.x, pl.y + 0.25, pl.z, ax, ay, az);
				bl.starttime += bl.interval;
				//--bl.quantity;
				break;
			}
		}
		bl.quantity = 0;
		break;
	case KEY_POOPSTORM:
		i = getplayerid(bl.handle);

		if (i != -1) {
			pl.getinfo(i);
			if (pl.handle == 0) {
				bulletqueue[_countof(bulletqueue) / 2 + 6 + i].quantity = 0;
				break;
			}
			if (height >= 0 && height <= 10) {
				for (float rad = 0; rad <= 2 * M_PI && rad >= 0; rad += 2 * M_PI * poopradius / 5) {
					shootbullet(pl.handle, bl.bid, pl.x + __imp_sin(rad) * poopradius, pl.y + height, pl.z + __imp_cos(rad) * poopradius, 0, -1, 0);
					if (poopradius < 0)
						poopradiusflag = 1;
					else if (poopradius > 5)
						poopradiusflag = 0;
				}
				height += 1;
				poopradiusflag ? poopradius += static_cast<float>(rand()) / static_cast <float> (RAND_MAX) : poopradius -= static_cast<float>(rand()) / static_cast <float> (RAND_MAX);
			}
			else
				height = 0;
			bl.starttime += bl.interval;
			//--bl.quantity;
		}
		break;
	case KEY_LOTHRICLIGHT:
		i = getplayerid(bl.handle);
		if (i != -1) {
			pl.getinfo(i);
			if (pl.anim == 80500) {
				float x, y = pl.y, z;
				float ax = -__imp_sin(pl.dir) * 100;
				float ay = 0;
				float az = -__imp_cos(pl.dir) * 100;
				for (float i = 1.5; i <= 31.5; i += 3) {
					x = -__imp_sin(pl.dir) * i + pl.x;
					z = -__imp_cos(pl.dir) * i + pl.z;
					shootbullet(bl.handle, bl.bid, x, y + 1.5, z, ax, ay, az);
				}
				bl.starttime += bl.interval;
				bl.quantity = 0;
			}
		}
		break;
	}

}

void queueloop() {
	for (int i = 0; i < _countof(bulletqueue); ++i)
		if (bulletqueue[i].quantity > 0 && GetTickCount() >= bulletqueue[i].starttime) {
			queueshoot(bulletqueue[i]);
		}
}
//b array


DWORD CALLBACK ThreadProc(void* param)
{
	while (1) {
		queueloop();
		for (int i = 0; i < 6; ++i) {
			playerinfo pl(i);
			pl.fangyangpi();
			if (pl.anim == 80500 && lothricswitch[i] == 0) {
				ModifyBullet(100062700, 0, int, 13531000);
				lothricswitch[i] = 1;
				AddBullet(pl.handle, 100062700, 1, 100, KEY_LOTHRICLIGHT, GetTickCount() + 1500);
			}
			else if (pl.anim != 80500)
				lothricswitch[i] = 0;
		}
		Sleep(50);
	}
	return 0;
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code == HC_ACTION) {
		switch (wParam) {
		case 'J':
			if (!(lParam & 1 << 31) && GetKeyState(VK_CONTROL) < 0) {
				static int a[] = { 20 };
				SendPockageInner(15, a, sizeof a, GetSteamID(GetLockTargetIndex()));
			}
			break;
		case 'K':
			if (!(lParam & 1 << 31) && GetKeyState(VK_CONTROL) < 0) {
				static int a[] = { 30 };
				SendPockageInner(15, a, sizeof a, GetSteamID(GetLockTargetIndex()));
			}
			break;
		case 'L':
			if (!(lParam & 1 << 31) && GetKeyState(VK_CONTROL) < 0) {
				putstr("%#X", GetLockTargetID());
			}
			break;
		case 'P':
			if (!(lParam & 1 << 31) && GetKeyState(VK_CONTROL) < 0 && GetLockTargetIndex() > 0 && GetLockTargetIndex() < 6) {
				if (bulletqueue[_countof(bulletqueue) / 2 + 6 + GetLockTargetIndex()].quantity == 0)
					AddBulletSpeIdle(GetLockTargetIndex() + 6, GetLockTargetID(), 341, 1, 10, KEY_POOPSTORM);
				else
					bulletqueue[_countof(bulletqueue) / 2 + 6 + GetLockTargetIndex()].quantity = 0;
			}
			break;
		}
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

HINSTANCE BaseAddress, BaseAddressGenuine; wchar_t* BaseFileName, FullFilePath[512];
EXTERN_C BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		BaseAddress = hinstDLL;
		BaseFileName = FullFilePath + GetModuleFileNameW(hinstDLL, FullFilePath, _countof(FullFilePath));
		while (BaseFileName-- > FullFilePath + 1 && *BaseFileName != L'\\');
		DarkSoulsIII = (PBYTE)GetModuleHandleW(0);
		srand(GetTickCount());
		CloseHandle(CreateThread(0, 0, ThreadProc, 0, 0, 0));
	case DLL_THREAD_ATTACH:
		SetWindowsHookExW(WH_KEYBOARD, KeyboardProc, 0, GetCurrentThreadId());
		break;
	}
	return 1;
}

EXTERN_C void LoadGenuineDll()
{
	if (!BaseAddressGenuine) {
		static wchar_t filename[512];
		wcscpy(filename + GetSystemDirectoryW(filename, _countof(filename)), BaseFileName);
		BaseAddressGenuine = LoadLibraryW(filename);
		IMPORT(Ordinal16, 16);
		IMPORT(Ordinal17, 17);
		IMPORT(Ordinal18, 18);
		IMPORT(Ordinal19, 19);
		IMPORT(Ordinal20, 20);
		IMPORT(Ordinal21, 21);
		IMPORT(Ordinal22, 22);
		IMPORT(Ordinal23, 23);
		IMPORT(Direct3DShaderValidatorCreate9, 24);
		IMPORT(PSGPError, 25);
		IMPORT(PSGPSampleTexture, 26);
		IMPORT(D3DPERF_BeginEvent, 27);
		IMPORT(D3DPERF_EndEvent, 28);
		IMPORT(D3DPERF_GetStatus, 29);
		IMPORT(D3DPERF_QueryRepeatFrame, 30);
		IMPORT(D3DPERF_SetMarker, 31);
		IMPORT(D3DPERF_SetOptions, 32);
		IMPORT(D3DPERF_SetRegion, 33);
		IMPORT(DebugSetLevel, 34);
		IMPORT(DebugSetMute, 35);
		IMPORT(Direct3D9EnableMaximizedWindowedModeShim, 36);
		IMPORT(Direct3DCreate9, 37);
		IMPORT(Direct3DCreate9Ex, 38);
	}
}

