// BakkesTest.cpp : Defines the exported functions for the DLL application.
//
#include "BakkesTest.h"
#include "bakkesmod\plugin\bakkesmodplugin.h"

BAKKESMOD_PLUGIN(BakkesTest, "Stat n stuff", "0.0.1", PLUGINTYPE_THREADED)

using namespace std::placeholders;

void BakkesTest::onLoad() {
	cvarManager->log("Some POC Plugin has been loaded :o why are you running this ?");
	// Clean player stats from our OldPlayersState when game starts
	gameWrapper->HookEventPost("Function TAGame.PlayerController_TA.PostBeginPlay", bind(&BakkesTest::ClearGoalStats, this, _1));
	// If someones moves team ( or join, or you join and they're in a team ), updates stats
	gameWrapper->HookEventPost("Function TAGame.GFxData_PRI_TA.HandleTeamChanged", bind(&BakkesTest::UpdateGoalStats, this, _1));
	// That's fired everytime someone scores. Also Pri.GetGoals is updated before this is fired
	gameWrapper->HookEventPost("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored", bind(&BakkesTest::UpdateGoalStats, this, _1)); 
}
void BakkesTest::ClearGoalStats(std::string eventName) {
	// Mark our stash of stats as dirty
	for (unsigned int a = 0; a < sizeof(OldPlayersState); a = a + 1) {
		OldPlayersState[a].Dirty = true;
	}
}
void BakkesTest::UpdateGoalStats(std::string eventName) {
	if (gameWrapper->IsInOnlineGame()) {
		ArrayWrapper<PriWrapper> players = gameWrapper->GetOnlineGame().GetPRIs();
		for (unsigned int a = 0; a < players.Count(); a = a + 1) {
			if (!OldPlayersState[a].Dirty && eventName == "Function TAGame.ArenaSoundManager_TA.HandlePlayerScored") {
				if (OldPlayersState[a].Goals < players.Get(a).GetMatchGoals()) {
					gameWrapper->LogToChatbox(players.Get(a).GetPlayerName().ToString() + " has just scored!");
				}
			}
			OldPlayersState[a] = {
				players.Get(a).GetMatchGoals(),
				players.Get(a).GetPlayerName().ToString(),
				false
			};
		}
	}
}
