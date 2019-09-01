// BakkesTest.cpp : Defines the exported functions for the DLL application.
//
#include "BakkesTest.h"
#include "bakkesmod\plugin\bakkesmodplugin.h"
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

BAKKESMOD_PLUGIN(BakkesTest, "Stat n stuff over ws", "0.0.2", PLUGINTYPE_THREADED)

using namespace std::placeholders;

void BakkesTest::onLoad() {
	cvarManager->log("Some POC Plugin has been loaded :o why are you running this ?");
	// Clean player stats from our OldPlayersState when game starts
	gameWrapper->HookEventPost("Function TAGame.PlayerController_TA.PostBeginPlay", bind(&BakkesTest::ClearGoalStats, this, _1));
	// If someones moves team ( or join, or you join and they're in a team ), updates stats
	gameWrapper->HookEventPost("Function TAGame.GFxData_PRI_TA.HandleTeamChanged", bind(&BakkesTest::UpdateGoalStats, this, _1));
	// That's fired everytime someone scores. Also Pri.GetGoals is updated before this is fired
	gameWrapper->HookEventPost("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored", bind(&BakkesTest::UpdateGoalStats, this, _1));
	
	ws_server.init_asio();
	ws_server.set_open_handler(bind(&BakkesTest::OnWsOpen, this, ::_1));
	ws_server.set_close_handler(bind(&BakkesTest::OnWsClose, this, ::_1));
	ws_server.listen(8323);
	ws_server.start_accept();
	ws_server.run();

}

void BakkesTest::OnWsOpen(connection_hdl hdl) {
	m_connections.insert(hdl);
}

void BakkesTest::OnWsClose(connection_hdl hdl) {
	m_connections.erase(hdl);
}

void BakkesTest::SendWsPayload(std::string payload) {
	for (auto it : m_connections) {
		ws_server.send(it, payload, websocketpp::frame::opcode::text);
	}
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
					SendWsPayload(players.Get(a).GetPlayerName().ToString() + " has just scored!\n");
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

