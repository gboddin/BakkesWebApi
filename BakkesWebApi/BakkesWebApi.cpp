#include "BakkesWebApi.h"
using websocketpp::connection_hdl;
using namespace std;
using placeholders::_1;
BAKKESMOD_PLUGIN(BakkesWebApi, "BakkesWebApi", "0.2.2", PLUGINTYPE_THREADED)

void BakkesWebApi::onLoad() {
	this->gameWrapper->HookEventPost("Function TAGame.PRI_TA.PostBeginPlay", std::bind(&BakkesWebApi::HookGameStarted, this, _1));
	this->gameWrapper->HookEventPost("Function TAGame.Ball_TA.RecordCarHit", std::bind(&BakkesWebApi::HookPlayerChanged, this, _1));
	this->gameWrapper->HookEventPost("Function TAGame.GFxData_PRI_TA.HandleTeamChanged", std::bind(&BakkesWebApi::HookPlayerChanged, this, _1));
	this->gameWrapper->HookEventPost("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored", std::bind(&BakkesWebApi::HookGoalScored, this, _1));
	this->ws_connections = new ConnectionSet();
	this->UpdateGameState();
	RunWsServer();
}

void BakkesWebApi::HookGameStarted(std::string eventName) {
	this->ClearPlayersState();
	this->ClearTeamsState();
}
void BakkesWebApi::HookPlayerChanged(std::string eventName) {
	if (gameWrapper->IsInOnlineGame()) {
		this->UpdatePlayersState();
		this->UpdateTeamsState();
	}
}

void BakkesWebApi::HookGoalScored(std::string eventName) {
	if (this->gameWrapper->IsInOnlineGame()) {
		ArrayWrapper<PriWrapper> players = this->gameWrapper->GetOnlineGame().GetPRIs();
		for (int a = 0; a < players.Count(); a++ ) {
			if (!this->PlayersState[a].Dirty && this->PlayersState[a].Goals < players.Get(a).GetMatchGoals()) {
				this->UpdatePlayersState();
				this->UpdateTeamsState();
				this->SendEvent("goal_scored", this->PlayersState[a]);
			}
		}
	}
}

void BakkesWebApi::UpdateGameState() {
	int playlistId = GAMETYPE_MENU;
	int teamSize = 0;
	bool gameChanged = false;

	if (GetCurrentGameType() != NULL && GetCurrentGameType()->GetPlaylist().memory_address != NULL) {
		playlistId = GetCurrentGameType()->GetPlaylist().GetPlaylistId();
		teamSize = GetCurrentGameType()->GetMaxTeamSize();
		this->GameState.SecondsRemaining = GetCurrentGameType()->GetSecondsRemaining();
		this->GameState.SecondsElapsed = GetCurrentGameType()->GetSecondsElapsed();
	}

	gameChanged = ((this->GameState.PlaylistId != playlistId) || (this->GameState.TeamSize != teamSize));

	
	this->GameState.PlaylistId = playlistId;
	this->GameState.TeamSize = teamSize;

	if(gameChanged) {
		this->SendEvent("game_changed", this->GameState);
	}
	
	// I'll be back
	this->gameWrapper->SetTimeout(std::bind(&BakkesWebApi::UpdateGameState, this), 1.0f);
}

void BakkesWebApi::ClearPlayersState() {
	for(ApiPlayer &PlayerState : PlayersState) {
		PlayerState.Dirty = true;
	}
}

void BakkesWebApi::UpdatePlayersState() {
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
		PlayersState[a].IsBot = players.Get(a).GetbBot();
		PlayersState[a].TeamNum = players.Get(a).GetTeamNum();
		PlayersState[a].MMR = gameWrapper->GetMMRWrapper().GetPlayerMMR(
			players.Get(a).GetUniqueId(),
			gameWrapper->GetMMRWrapper().GetCurrentPlaylist()
		);
		PlayersState[a].RankTier = gameWrapper->GetMMRWrapper().GetPlayerRank(players.Get(a).GetUniqueId(), gameWrapper->GetMMRWrapper().GetCurrentPlaylist()).Tier;
		PlayersState[a].RankDivision = gameWrapper->GetMMRWrapper().GetPlayerRank(players.Get(a).GetUniqueId(), gameWrapper->GetMMRWrapper().GetCurrentPlaylist()).Division;
		PlayersState[a].RankMatchesPlayed = gameWrapper->GetMMRWrapper().GetPlayerRank(players.Get(a).GetUniqueId(), gameWrapper->GetMMRWrapper().GetCurrentPlaylist()).MatchesPlayed;

		if (!players.Get(a).GetCar().IsNull() && !players.Get(a).GetCar().GetBoostComponent().IsNull()) {
			PlayersState[a].CurrentBoostAmount = players.Get(a).GetCar().GetBoostComponent().GetCurrentBoostAmount();
		}
		if (players.Get(a).GetUniqueId().ID == this->gameWrapper->GetSteamID()) {
			CurrentPlayer = PlayersState[a];
		}
	}
}

json::JSON BakkesWebApi::GetPlayersStateJson() {
	json::JSON json_players;
	for (ApiPlayer &PlayerState : PlayersState) {
		if (!PlayerState.Dirty) {
			json_players.append(PlayerState.getJson());
		}
	}
	return json_players;
}

void BakkesWebApi::ClearTeamsState() {
	for (ApiTeam &TeamState : TeamsState) {
		TeamState.Dirty = true;
	}
}

json::JSON BakkesWebApi::GetTeamsStateJson() {
	json::JSON json_teams;
	for (ApiTeam &TeamState : TeamsState) {
		if (!TeamState.Dirty) {
			json_teams.append(TeamState.getJson());
		}
	}
	return json_teams;
}

void BakkesWebApi::UpdateTeamsState() {
	ArrayWrapper<TeamWrapper> teams = this->gameWrapper->GetOnlineGame().GetTeams();
	for (int a = 0; a < teams.Count(); a++) {
		TeamsState[a].Dirty = false;
		TeamsState[a].Goals = teams.Get(a).GetScore();
		TeamsState[a].TeamIndex = teams.Get(a).GetTeamIndex();
		TeamsState[a].TeamNum = teams.Get(a).GetTeamNum();
		if (!teams.Get(a).GetSanitizedTeamName().IsNull()) {
			TeamsState[a].Name = teams.Get(a).GetSanitizedTeamName().ToString();
		}
		else if (TeamsState[a].TeamIndex == 1) {
			TeamsState[a].Name = "Orange";
		}
		else {
			TeamsState[a].Name = "Blue";
		}
			
	}
}

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

void BakkesWebApi::SendEvent(string eventName, ApiPlayer player) {
	json::JSON event;
	event["event"] = eventName;
	event["player"] = player.getJson();
	this->SendWsPayload(
		event.dump()
	);
}

void BakkesWebApi::SendEvent(string eventName, ApiGame game) {
	json::JSON event;
	event["event"] = eventName;
	event["game"] = game.getJson();
	this->SendWsPayload(
		event.dump()
	);
}

void BakkesWebApi::SendEvent(string eventName, ApiGame game, ApiPlayer players[], ApiTeam teams[]) {
	json::JSON event;
	event["event"] = eventName;
	event["game"] = game.getJson();
	event["players"] = this->GetPlayersStateJson();

	this->SendWsPayload(
		event.dump()
	);
}


/**
 * HTTP ... hum ... router ?
 */
void BakkesWebApi::OnHttpRequest(websocketpp::connection_hdl hdl) {
	BakkesWebApiServer::connection_ptr connection = ws_server->get_con_from_hdl(hdl);
	connection->append_header("Content-Type", "application/json");
	connection->append_header("Server", "BakkesWebApi/0.1");
	if (connection->get_resource() == "/game") {
		connection->set_body(
			GameState.getJson().dump()
		);
		connection->set_status(websocketpp::http::status_code::ok);
		return;
	}
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