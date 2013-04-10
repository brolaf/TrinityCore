/*******************************************
*Made By - SymbolixDEV                     *
*Errors fixed by - SymbolixDEV             *
*Warnings fixed by - SymbolixDEV           *
****************************************/

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Chat.h"
#include "Common.h"
#include "Language.h"

class vipcommands : public CommandScript
{
public:
    vipcommands() : CommandScript("vipcommands") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand vipCommandTable[] =

        {
            { "mall",	    SEC_MODERATOR,     true, &HandleVipMallCommand,         "", NULL },
			{ "buff",	    SEC_MODERATOR,     true, &HandleVipMallCommand,         "", NULL },
			{ "duel",	    SEC_MODERATOR,     true, &HandleVipMallCommand,         "", NULL },
			{ "arena",	    SEC_MODERATOR,     true, &HandleVipMallCommand,         "", NULL },
            { "changerace",    SEC_MODERATOR,  false, &HandleChangeRaceCommand,             "", NULL },
	    { "changefaction",	SEC_MODERATOR,  false, &HandleChangeFactionCommand,		"", NULL },
	    { "customize",	SEC_MODERATOR,  false, &HandleCustomizeCommand,		"", NULL },
	    { "tele",           SEC_MODERATOR,  false, &HandleTeleCommand,		"", NULL },
 
            { NULL,             0,                     false, NULL,                                           "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "vip",	    SEC_MODERATOR,   true, NULL,      "",  vipCommandTable},
	       { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }



static bool HandleTeleCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Player* me = handler->GetSession()->GetPlayer();

        // id, or string, or [name] Shift-click form |color|Htele:id|h[name]|h|r
        GameTele const* tele = handler->extractGameTeleFromLink((char*)args);

        if (!tele)
        {
            handler->SendSysMessage(LANG_COMMAND_TELE_NOTFOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (me->isInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        MapEntry const* map = sMapStore.LookupEntry(tele->mapId);
        if (!map || map->IsBattlegroundOrArena())
        {
            handler->SendSysMessage(LANG_CANNOT_TELE_TO_BG);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (me->isInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            me->SaveRecallPosition();

        me->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
        return true;
		}

static bool HandlevipCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

            me->Say("vip command?", LANG_UNIVERSAL);
            return true;
    }

static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("Relog to change race of your character.");
        return true;
    }

static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("Relog to change faction of your character.");
        return true;
    }


static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("Relog to customize your character.");
        return true;
    }

static bool HandleBuffCommand(ChatHandler * handler, const char * args)
    {
        Player * pl = handler->GetSession()->GetPlayer();
                if(pl->isInCombat())
                {
                        pl->GetSession()->SendNotification("You can't use this in combat!");
                        return false;
                }
                if(pl->InArena())
                {
                        pl->GetSession()->SendNotification("You can't use that item in an arena match!");
                        return false;
                }
                else
                pl->AddAura(48162, pl);
                pl->AddAura(48074, pl);
                pl->AddAura(48170, pl);
                pl->AddAura(43223, pl);
                pl->AddAura(36880, pl);
                pl->AddAura(467, pl);
                pl->AddAura(69994, pl);
                pl->AddAura(48469, pl);
                handler->PSendSysMessage("You have been buffed, enjoy!");
                return true;
 
    }	

static bool HandleVipMallCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

        if (me->isInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (me->isInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            me->SaveRecallPosition();

		me->TeleportTo(0,	23.8378f,	-1588.85f,	195.419f,	4.54306f); // MapId, X, Y, Z, O
                return true;
    }

static bool HandleVipDuelCommand(ChatHandler* handler, const char* args)
    {

        Player* me = handler->GetSession()->GetPlayer();

        if (me->isInCombat())
        {
            handler->SendSysMessage(LANG_YOU_IN_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if (me->isInFlight())
        {
            me->GetMotionMaster()->MovementExpired();
            me->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            me->SaveRecallPosition();

		me->TeleportTo(0,	23.8378f,	-1588.85f,	195.409f,	4.54306f); // MapId, X, Y, Z, O
                return true;
    }

	

};

void AddSC_vipcommands()
{
    new vipcommands();
}