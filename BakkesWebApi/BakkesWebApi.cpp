// BakkesWebApi.cpp : Defines the exported functions for the DLL application.
//
#include "BakkesWebApi.h"
using websocketpp::connection_hdl;
using std::placeholders::_1;
BAKKESMOD_PLUGIN(BakkesWebApi, "BakkesWebApi", "0.1.0", PLUGINTYPE_THREADED)

void BakkesWebApi::onLoad() {
	// Clean player stats from our OldPlayersState when game starts
	this->gameWrapper->HookEventPost("Function TAGame.PlayerController_TA.PostBeginPlay", std::bind(&BakkesWebApi::HookMatchStarted, this, _1));
	// If someones moves team ( or join, or you join and they're in a team ), or touches the ball, updates stats
	this->gameWrapper->HookEventPost("Function TAGame.Ball_TA.RecordCarHit", std::bind(&BakkesWebApi::HookPlayerChanged, this, _1));
	this->gameWrapper->HookEventPost("Function TAGame.GFxData_PRI_TA.HandleTeamChanged", std::bind(&BakkesWebApi::HookPlayerChanged, this, _1));
	// That's fired everytime someone scores. Also Pri.GetGoals is updated before this is fired
	this->gameWrapper->HookEventPost("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored", std::bind(&BakkesWebApi::HookGoalScored, this, _1));
	this->ws_connections = new ConnectionSet();
	RunWsServer();
}

void BakkesWebApi::HookMatchStarted(std::string eventName) {
	this->ClearPlayersState();
	this->ClearTeamsState();
}

void BakkesWebApi::HookPlayerChanged(std::string eventName) {
	this->UpdatePlayersState();
	this->UpdateTeamsState();
}

void BakkesWebApi::HookGoalScored(std::string eventName) {
	if (this->gameWrapper->IsInOnlineGame()) {
		this->UpdateTeamsState();
		// Get all Pris
		ArrayWrapper<PriWrapper> players = this->gameWrapper->GetOnlineGame().GetPRIs();
		for (int a = 0; a < players.Count(); a++ ) {
			// If oldstate has a score < than current and event is scored -> fire goal_scored	
			if (!this->PlayersState[a].Dirty && this->PlayersState[a].Goals < players.Get(a).GetMatchGoals()) {
				this->UpdatePlayersState();
				this->SendEvent("goal_scored", this->PlayersState[a]);
			}
		}
	}
}

void BakkesWebApi::SendEvent(string eventName, ApiPlayer player) {
	json::JSON event;
	event["event"] = eventName;
	event["player"] = player.getJson();
	this->SendWsPayload(
		event.dump()
	);
}

/**
 * Mark our stash as dirty
 */
void BakkesWebApi::ClearPlayersState() {
	/** Uncomment for Random crashes and nice artifacts,
	 * for (unsigned int a = 0; a < sizeof(OldPlayersState); a = a + 1) {
	 */
	 // Mark our stash of stats as dirty
	for(ApiPlayer &PlayerState : PlayersState) {
		PlayerState.Dirty = true;
	}
}

/**
 * Update players data
 */
void BakkesWebApi::UpdatePlayersState() {
	// Make sure we're online or NULL will strike :
	// Get all Pris
	if (gameWrapper->IsInOnlineGame()) {
		ArrayWrapper<PriWrapper> players = this->gameWrapper->GetOnlineGame().GetPRIs();
		for (int a = 0; a < players.Count(); a++ ) {
			PlayersState[a].Dirty = false;
			PlayersState[a].Goals = players.Get(a).GetMatchGoals();
			PlayersState[a].PlayerName = players.Get(a).GetPlayerName().ToString();
			PlayersState[a].PlayerID = players.Get(a).GetPlayerID();
			PlayersState[a].PlayerUniqueID = players.Get(a).GetUniqueId().ID;
			PlayersState[a].Kills = players.Get(a).GetKills();
			PlayersState[a].Assists = players.Get(a).GetMatchAssists();
			PlayersState[a].Saves = players.Get(a).GetMatchSaves();
			PlayersState[a].BallTouches = players.Get(a).GetBallTouches();
			PlayersState[a].IsBot = !players.Get(a).IsPlayer();
			PlayersState[a].MMR = gameWrapper->GetMMRWrapper().GetPlayerMMR(
				players.Get(a).GetUniqueId(),
				gameWrapper->GetMMRWrapper().GetCurrentPlaylist()
			);
			if (!players.Get(a).GetCar().IsNull() && !players.Get(a).GetCar().GetBoostComponent().IsNull()) {
				PlayersState[a].CurrentBoostAmount = players.Get(a).GetCar().GetBoostComponent().GetCurrentBoostAmount();
			}
			if (players.Get(a).GetUniqueId().ID == this->gameWrapper->GetSteamID()) {
				CurrentPlayer = PlayersState[a];
			}
		}
	}
}

/**
 * Returns a json array of current players.
 */
json::JSON BakkesWebApi::GetPlayersStateJson() {
	json::JSON json_players;
	for (ApiPlayer &PlayerState : PlayersState) {
		if (!PlayerState.Dirty) {
			json_players.append(PlayerState.getJson());
		}
	}
	return json_players;
}

/**
 * Mark our stash as dirty
 */
void BakkesWebApi::ClearTeamsState() {
	/** Uncomment for Random crashes and nice artifacts,
	 * for (unsigned int a = 0; a < sizeof(OldPlayersState); a = a + 1) {
	 */
	 // Mark our stash of stats as dirty
	for (ApiTeam &TeamState : TeamsState) {
		TeamState.Dirty = true;
	}
}

/**
 * Returns a json array of current teams.
 */
json::JSON BakkesWebApi::GetTeamsStateJson() {
	json::JSON json_teams;
	for (ApiTeam &TeamState : TeamsState) {
		if (!TeamState.Dirty) {
			json_teams.append(TeamState.getJson());
		}
	}
	return json_teams;
}

/**
 * Update teams data
 */
void BakkesWebApi::UpdateTeamsState() {
	if (gameWrapper->IsInOnlineGame()) {
		ArrayWrapper<TeamWrapper> teams = this->gameWrapper->GetOnlineGame().GetTeams();
		for (int a = 0; a < teams.Count(); a++) {
			TeamsState[a].Dirty = false;
			TeamsState[a].Goals = teams.Get(a).GetScore();
			if (!teams.Get(a).GetSanitizedTeamName().IsNull()) {
				TeamsState[a].Name = teams.Get(a).GetSanitizedTeamName().ToString();
			}
			else if (teams.Get(a).GetTeamIndex() == 1) {
				TeamsState[a].Name = "Orange";
			}
			else {
				TeamsState[a].Name = "Blue";
			}
			
		}
	}
}

/**
 * Starts the websocket server
 */
void BakkesWebApi::RunWsServer() {
	ws_server = new BakkesWebApiServer();
	ws_server->init_asio();
	ws_server->set_open_handler(websocketpp::lib::bind(&BakkesWebApi::OnWsOpen, this, _1));
	ws_server->set_close_handler(websocketpp::lib::bind(&BakkesWebApi::OnWsClose, this, _1));
	ws_server->set_close_handler(websocketpp::lib::bind(&BakkesWebApi::OnWsClose, this, _1));
	ws_server->set_http_handler(websocketpp::lib::bind(&BakkesWebApi::OnHttpRequest, this, _1));
	ws_server->listen(8323);
	ws_server->start_accept();
	ws_server->run();
}

/**
 * HTTP ... hum ... router ?
 */
void BakkesWebApi::OnHttpRequest(websocketpp::connection_hdl hdl) {
	BakkesWebApiServer::connection_ptr connection = ws_server->get_con_from_hdl(hdl);
	connection->append_header("Content-Type", "application/json");
	connection->append_header("Server", "BakkesWebApi/0.1");
	if (!gameWrapper->IsInOnlineGame()) {
		connection->set_body("{false}");
		connection->set_status(websocketpp::http::status_code::conflict);
		return;
	}
	if (connection->get_resource() == "/player") {
		connection->set_body(
			CurrentPlayer.getJson().dump()
		);
		connection->set_status(websocketpp::http::status_code::ok);
		return;
	}
	if (connection->get_resource() == "/players") {
		connection->set_body(
			GetPlayersStateJson().dump()
		);
		connection->set_status(websocketpp::http::status_code::ok);
		return;
	}
	if (connection->get_resource() == "/teams") {
		connection->set_body(
			GetTeamsStateJson().dump()
		);
		connection->set_status(websocketpp::http::status_code::ok);
		return;
	}
	connection->set_body("Not found");
	connection->set_status(websocketpp::http::status_code::not_found);
}

void BakkesWebApi::OnWsOpen(connection_hdl hdl) {
	// add a connection to the set
	ws_connections->insert(hdl);
}

void BakkesWebApi::OnWsClose(connection_hdl hdl) {
	// remove a connection from the set
	ws_connections->erase(hdl);
}

void BakkesWebApi::SendWsPayload(std::string payload) {
	// broadcast to all connections
	for (connection_hdl it : *ws_connections) {
		ws_server->send(it, payload , websocketpp::frame::opcode::text);
	}
}

void BakkesWebApi::onUnload() {
	gameWrapper->UnhookEvent("Function TAGame.PlayerController_TA.PostBeginPlay");
	gameWrapper->UnhookEvent("Function TAGame.GFxData_PRI_TA.HandleTeamChanged"); 
	gameWrapper->UnhookEvent("Function TAGame.Ball_TA.RecordCarHit");
	gameWrapper->UnhookEvent("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored");
	// Init websocket server
	if (ws_server != NULL) {
		ws_server->stop();
		ws_server->stop_listening();
	}
	ws_connections->clear();
}
