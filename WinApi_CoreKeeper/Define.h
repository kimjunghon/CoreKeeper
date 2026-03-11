#pragma once

#define			VK_MAX			0xff

#define			WINCX			800
#define			WINCY			600

#define			PURE			= 0 

#define			OBJ_NOEVENT		0
#define			OBJ_DEAD		1

#define			TILECX			40
#define			TILECY			40
#define			TILEX			60
#define			TILEY			45

#define			DEFAULT_QUICK	10
#define			DEFAULT_INVEN	20
#define			SLOTCX			40
#define			SLOTCY			40

#define			PI				3.141592f

#define			PLAYER_ATKPOWER 1000.f
#define			PLAYER_DEFPOWER 5.f
#define			PLAYER_MININGPOWER 10.f
#define			PLAYER_MAXHP	100
#define			PLAYER_SPEED	3.f
#define			PLAYER_COLLSION_SPEED 1.f

#define			WOOD_SWORD		101
#define			WOOD_PICKAXE	102
#define			WOOD_BOW		103
#define			IRON_SWORD		104
#define			IRON_PICKAXE	105
#define			IRON_BOW		106

#define			IRON_HELM		110
#define			IRON_CHEST		111
#define			IRON_PANTS		112

#define			SLIME_FOOD		113
#define			LARVA_FOOD		114

#define			TURF_WALL		201
#define			DIRT_WALL		202
#define			CLAY_WALL		203
#define			IRON			204
#define			SLIME_CORE		205
#define			HIVE_CORE		206
#define			SLIME_MEAT		207
#define			LARVA_MEAT		208
#define			WOOD			209
#define			IRON_STONE		210

#define			EFFECT_HIT				1001
#define			EFFECT_BOSS_SLIME_DEATH 1002
#define			EFFECT_BOSS_HIVE_DEATH	1003
#define			EFFECT_ACID_IMPACT		1004
#define			EFFECT_EGG_HATCHING		1005
#define			EFFECT_WARNING			1006
#define			EFFECT_DEATH			1007
#define			EFFECT_LARVA_DEATH		1008
#define			EFFECT_POTAL			1009
#define			EFFECT_POWERUP			1010
#define			HIVE_SCENE_EFFECT		1011
#define			EFFECT_END_POTAL		1012

enum SCENE {SC_START, SC_TITLE, SC_PLAY, SC_EDIT, SC_CLEAR, SC_END};

enum DIRECTION {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_END};

enum OBJSTATE {OS_IDLE, OS_WALK, OS_ATTACK, OS_END};

enum ITEMCLASS {IC_MELEE_SWORD, IC_MELEE_PICKAXE, IC_BOW ,IC_ARMOR_HELM, IC_ARMOR_CHEST, IC_ARMOR_PANTS, IC_ORE,IC_USE, IC_END};

enum WEAPONGRADE {WG_WOOD, WG_IRON, WG_END};

enum OBJID { OBJ_PLAYER, OBJ_MONSTER, OBJ_PROJECTILE, OBJ_BUILDING, OBJ_ITEM, OBJ_CRAFTING, OBJ_EFFECT, OBJ_END };

enum RENDERID {RENDER_OBJECT, RENDER_UI, RENDER_END };

enum TILEID {TILE_TURF, TILE_DIRT, TILE_CLAY, TILE_NULL, TILE_EDIT ,TILE_END};

enum WALLID { WALL_TURF, WALL_DIRT, WALL_CLAY, WALL_CLAY_IRON,WALL_NULL, WALL_END };

enum UIID {UI_STATE, UI_INVENTORY, UI_CRAFTING, UI_EQUIP,UI_SLIE_CORE, UI_HIVE_CORE, UI_COOKING,UI_SLIME_DIALOGUE, UI_SMELTING, UI_MINIMAP, UI_MOUSE,UI_END};

enum SLOTID { SLOT_INVEN, SLOT_QUICK, SLOT_EQUIP_HELM, SLOT_EQUIP_CHEST, SLOT_EQUIP_PANTS, SLOT_CRAFTING, SLOT_SLIME_CORE, SLOT_HIVE_CORE, SLOT_FORGE,SLOT_FORGE_RESULT, SLOT_END};

enum CHANNELID { SOUND_MONSTER_EFFECT, SOUND_EFFECT, SOUND_EFFECT2,SOUND_MONSTER, SOUND_BGM, SOUND_UI, SOUND_PROJECTILE,SOUND_FOOTSTEP,MAXCHANNEL};

enum MONSTERID{MON_SLIME, MON_BOSS_SLIME, MON_LARVA, MON_BOSS_HIVE, MON_LARVA_EGG ,MON_END};

enum QUESTID{QUEST_START, QUEST_PLAY, QUEST_CLEAR, QUEST_FAIL, QUEST_END};

enum DIALOGUESTATE {DIA_MAIN, DIA_TALK, DIA_QUEST_START, DIA_QUEST_CLEAR, DIA_QUEST_FAIL, DIA_END};

enum BUTTONID{BT_START, BT_STOP, BT_ACCEPT, BT_REFUSE, BT_END};


template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
};

template<typename T>
void Null_Arr(T* Temp, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		Temp[i] = nullptr;
	}
}

template<typename T>
void Delete_Arr(T* Temp, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		if (Temp[i])
		{
			delete Temp[i];
			Temp = nullptr;
		}
	}
}



struct DeleteMap
{
	template<typename T>
	void operator()(T& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	}
};

typedef struct tagInfo
{
	float			fX;
	float			fY;
	float			fCX;	// 가로 길이
	float			fCY;	// 세로 길이

}INFO;

typedef struct tagBmpInfo
{
	float			fCX;
	float			fCY;
}BMPINFO;


typedef struct tagFrame
{
	int	iFrameStart;
	int iFrameEnd;
	int	iMotion;

	DWORD dwSpeed;
	DWORD dwTime;

}FRAME;

typedef struct tagMinimapInfo
{
	bool bPassed;
	TILEID eTileID;
}MINIMAPINFO;

struct tagStringCmp
{
public:
	tagStringCmp(const TCHAR* pString)
		: m_pString(pString) {}

public:
	template<typename T>
	bool	operator()(T& MyPair)
	{
		return !lstrcmp(m_pString, MyPair.first);
	}

public:
	const TCHAR* m_pString;
};

extern HWND g_hWnd;
extern float g_fEffectVolume;