// BakkesTest.cpp : Defines the exported functions for the DLL application.
//
#include "BakkesTest.h"
#include "bakkesmod\plugin\bakkesmodplugin.h"
using websocketpp::connection_hdl;


BAKKESMOD_PLUGIN(BakkesTest, "Stat n stuff over ws", "0.0.2", PLUGINTYPE_THREADED)

void BakkesTest::onLoad() {
	// Hook events :
	cvarManager->log("Some POC Plugin has been loaded :o why are you running this ?");
	// Clean player stats from our OldPlayersState when game starts
	gameWrapper->HookEventPost("Function TAGame.PlayerController_TA.PostBeginPlay", std::bind(&BakkesTest::ClearGoalStats, this, std::placeholders::_1));
	// If someones moves team ( or join, or you join and they're in a team ), updates stats
	gameWrapper->HookEventPost("Function TAGame.GFxData_PRI_TA.HandleTeamChanged", std::bind(&BakkesTest::UpdateGoalStats, this, std::placeholders::_1));
	// That's fired everytime someone scores. Also Pri.GetGoals is updated before this is fired
	gameWrapper->HookEventPost("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored", std::bind(&BakkesTest::UpdateGoalStats, this, std::placeholders::_1));
	RunWsServer();
}

void BakkesTest::OnWsOpen(connection_hdl hdl) {
	// add a connection to the set
	m_connections.insert(hdl);
}

void BakkesTest::OnWsClose(connection_hdl hdl) {
	// remove a connection from the set
	m_connections.erase(hdl);
}

void BakkesTest::SendWsPayload(std::string payload) {
	// broadcast to all connections
	for (auto it : m_connections) {
		ws_server->send(it, payload, websocketpp::frame::opcode::text);
	}
}

void BakkesTest::ClearGoalStats(std::string eventName) {
	// Mark our stash of stats as dirty
	for (unsigned int a = 0; a < sizeof(OldPlayersState); a = a + 1) {
		OldPlayersState[a].Dirty = true;
	}
}
void BakkesTest::UpdateGoalStats(std::string eventName) {
	// Make sure we're online or NULL will strike :
	if (gameWrapper->IsInOnlineGame()) {
		// Get all Pris
		ArrayWrapper<PriWrapper> players = gameWrapper->GetOnlineGame().GetPRIs();
		for (unsigned int a = 0; a < players.Count(); a = a + 1) {
			// If oldstate has a score < than current and event is scored -> fire goal_scored
			if (!OldPlayersState[a].Dirty && eventName == "Function TAGame.ArenaSoundManager_TA.HandlePlayerScored") {
				if (OldPlayersState[a].Goals < players.Get(a).GetMatchGoals()) {
					gameWrapper->LogToChatbox(players.Get(a).GetPlayerName().ToString() + " has just scored!");
					SendWsPayload(
						GetGoalScoredEvent(players.Get(a))
					);
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
string BakkesTest::GetGoalScoredEvent(PriWrapper player) {
	json::JSON json_event;
	json_event["event"] = "goal_scored";
	json_event["player"] = JsonPriWrapper(player);
	return json_event.dump();
}

json::JSON BakkesTest::JsonPriWrapper(PriWrapper player) {
	json::JSON json_player;
	json_player["PlayerName"] = player.GetPlayerName().ToString();
	json_player["MatchGoals"] = player.GetMatchGoals();
	json_player["MatchAssists"] = player.GetMatchAssists();
	json_player["TeamInfo"] = BakkesTest::JsonTeamInfoWrapper(player.GetTeam());
	return json_player;
}

json::JSON BakkesTest::JsonTeamInfoWrapper(TeamInfoWrapper team) {
	json::JSON json_team;
	json_team["TeamName"] = team.GetTeamName().ToString();
	return json_team;
}

void BakkesTest::RunWsServer() {
	// Init websocket server
	if (ws_server == NULL) {
		ws_server = new server();
	}
	else {
		ws_server->stop();
	}
	ws_server->init_asio();
	ws_server->set_open_handler(
		websocketpp::lib::bind(&BakkesTest::OnWsOpen, this, websocketpp::lib::placeholders::_1)
	);
	ws_server->set_close_handler(
		websocketpp::lib::bind(&BakkesTest::OnWsClose, this, websocketpp::lib::placeholders::_1)
	);
	ws_server->listen(8323);
	ws_server->start_accept();
	ws_server->run();
}