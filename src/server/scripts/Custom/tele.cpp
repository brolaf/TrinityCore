/*
Portal Master convert to C++ for Trinitycore
By Rochet2

Credits:
Rochet2
Eric

ScriptName: TeLe_gossip_codebox
*/

#include "ScriptPCH.h"

// Teleport data

struct Eric
{
	uint32 menu_id, next_menu_id;
	uint8 icon;
	std::string name;
	uint32 cost;
	uint8 level, faction; // 0 Both, 1 Horde, 2 Ally
	uint32 map;
	float x, y, z, o;
};
typedef struct Eric Rochet2;

//	{menu_id, next_menu_id, icon, name, cost, level, faction, map, x, y, z, o}
Rochet2 TeLe [] = // key = i & Key
{
	{1,	0,	2,	"Mall",	0,	0,	2,	0,	-8322.487305f,	2091.193115f,	125.813408f,	3.489510f}, // A
	{1,	0,	2,	"Mall",	0,	0,	1,	1,	-8564.412109f,	-1998.245483f,	100.350288f,	0.356550f}, // H
	{1,	0,	2,	"Duel Zone",	0,	0,	0,	1,	4298.943848f,	-2761.944336f,	16.662846f,	3.664825f},
	{1,	0,	2,	"Arena Que Place",	0,	0,	0,	2,	-13263.814453f,	158.768204f,	36.077797f,	1.079510f},
	{1,	0,	2,	"Wetlands",	0,	20,	2,	0,	-3517.75f,	-913.401f,	8.86625f,	2.60705f},
//	{1,	0,	2,	"Teleport",	0,	0,	0,	map,	xf,	yf,	zf,	of},
//	{1,	0,	7,	"Back..",	0,	0,	0,	0,	0,	0,	0,	0},
};

// TextIDs from npc_text
enum eEnums
{
	TEXT_MAIN_H		=	300000,			//Horde main menu text
	TEXT_MAIN_A		=	300001,			//Alliance main menu text
	TEXT_DUNGEON    =       300002,                 //Dungeon teleport menu texts
    TEXT_RAID       =       300003,                 //Raid teleport menu text
    TEXT_AREA       =       300004,                 //Area teleport location menu texts
	
	TELEPORT_COUNT	=	sizeof TeLe/sizeof(*TeLe),
};

#define ARE_YOU_SURE	"Are you sure, that you want to go to "
#define ERROR_COMBAT	"|cffff0000You are in combat|r"

bool Custom_FactCheck (uint32 Fact, unsigned char Key)
{
	bool Show = false;
	switch (TeLe[Key].faction)
	{
	case 0:
		Show = true;
		break;
	case 1:
		if (Fact == HORDE)
			Show = true;
		break;
	case 2:
		if (Fact == ALLIANCE)
			Show = true;
		break;
	}
	return (Show);
}

uint32 Custom_GetText (unsigned int menu, Player* pPlayer)
{
	uint32 TEXT = TEXT_AREA;

	switch (menu)
	{
	case 0:
		switch (pPlayer->GetTeam())
		{
		case ALLIANCE:
			TEXT = TEXT_MAIN_A;
			break;
		case HORDE:
			TEXT = TEXT_MAIN_H;
			break;
		}
	case 1:
	case 2:
	case 3:
		TEXT = TEXT_DUNGEON;
		break;
	case 4:
		TEXT = TEXT_RAID;
		break;
	}
	return (TEXT);
}

void Custom_GetMenu (Player* pPlayer, Creature* pCreature, uint32 Key)
{
	bool ENDMENU = false;
	for(uint32 i = 0; i < TELEPORT_COUNT; i++)
	{
		if (ENDMENU && TeLe[i].menu_id != Key)
			break;
		if (TeLe[i].menu_id == Key && pPlayer->getLevel() >= TeLe[i].level && Custom_FactCheck(pPlayer->GetTeam(), i))
		{
			if (TeLe[i].next_menu_id != 0)
				pPlayer->ADD_GOSSIP_ITEM_EXTENDED(TeLe[i].icon, TeLe[i].name, GOSSIP_SENDER_MAIN, i, "", TeLe[i].cost, false);
			else
				pPlayer->ADD_GOSSIP_ITEM_EXTENDED(TeLe[i].icon, TeLe[i].name, GOSSIP_SENDER_MAIN, i, ARE_YOU_SURE+TeLe[i].name, TeLe[i].cost, false);
			ENDMENU = true;
		}
	}
	pPlayer->PlayerTalkClass->SendGossipMenu(Custom_GetText(Key, pPlayer), pCreature->GetGUID());
	pPlayer->PlayerTalkClass->SendGossipMenu(555555, pCreature->GetGUID());
}

class TeLe_gossip_codebox : public CreatureScript
{
	public:
	TeLe_gossip_codebox()
	: CreatureScript("TeLe_gossip_codebox")
	{
	}

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		Custom_GetMenu(pPlayer, pCreature, 1);
		return true;
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
	{
		pPlayer->PlayerTalkClass->ClearMenus(); // clear the menu

		pPlayer->ModifyMoney(-1 * TeLe[uiAction].cost); // take cash
		uint32 Key = TeLe[uiAction].next_menu_id;
		if (Key == 0) // if no next_menu_id, then teleport to coords
		{
			if (!pPlayer->isInCombat())
			{
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->TeleportTo(TeLe[uiAction].map, TeLe[uiAction].x, TeLe[uiAction].y, TeLe[uiAction].z, TeLe[uiAction].o);
				return true;
			}
			pPlayer->GetSession()->SendAreaTriggerMessage(ERROR_COMBAT);
			Key = TeLe[uiAction].menu_id;
		}

		Custom_GetMenu(pPlayer, pCreature, Key);
		return true;
	}
};

void AddSC_TeLe_gossip_codebox()
{
    new TeLe_gossip_codebox();
}