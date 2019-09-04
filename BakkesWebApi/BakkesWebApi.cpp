// BakkesWebApi.cpp : Defines the exported functions for the DLL application.
//
#include "BakkesWebApi.h"
using websocketpp::connection_hdl;

BAKKESMOD_PLUGIN(BakkesWebApi, "Stat n stuff over ws", "0.0.2", PLUGINTYPE_THREADED)

void BakkesWebApi::onLoad() {
	// Clean player stats from our OldPlayersState when game starts
	gameWrapper->HookEventPost("Function TAGame.PlayerController_TA.PostBeginPlay", std::bind(&BakkesWebApi::HookMatchStarted, this, std::placeholders::_1));
	// If someones moves team ( or join, or you join and they're in a team ), updates stats
	gameWrapper->HookEventPost("Function TAGame.GFxData_PRI_TA.HandleTeamChanged", std::bind(&BakkesWebApi::HookPlayerChanged, this, std::placeholders::_1));
	// That's fired everytime someone scores. Also Pri.GetGoals is updated before this is fired
	gameWrapper->HookEventPost("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored", std::bind(&BakkesWebApi::HookGoalScored, this, std::placeholders::_1));
	this->jsonExport = new JsonExport(gameWrapper);
	this->ws_connections = new con_list();
	RunWsServer();
}
void BakkesWebApi::HookMatchStarted(std::string eventName) {
	ClearGoalStats();
}
void BakkesWebApi::HookPlayerChanged(std::string eventName) {
	if (gameWrapper->IsInOnlineGame()) {
		UpdateGoalStats();
	}
}
void BakkesWebApi::HookGoalScored(std::string eventName) {
	if (gameWrapper->IsInOnlineGame()) {
		// Get all Pris
		ArrayWrapper<PriWrapper> players = this->gameWrapper->GetOnlineGame().GetPRIs();
		for (int a = 0; a < players.Count(); a = a + 1) {
			// If oldstate has a score < than current and event is scored -> fire goal_scored	
			if (!OldPlayersStates[a].Dirty  && OldPlayersStates[a].Goals < players.Get(a).GetMatchGoals()) {
				this->SendWsPayload(
					this->jsonExport->event("goal_scored", players.Get(a)).dump()
				);
			}
		}
	}
	UpdateGoalStats();
}
void BakkesWebApi::ClearGoalStats() {
	/** Uncomment for Random crashes and nice artifacts,
	 * for (unsigned int a = 0; a < sizeof(OldPlayersState); a = a + 1) {
	 */
	 // Mark our stash of stats as dirty
	for(PlayerState &OldPlayersState : OldPlayersStates) {
		OldPlayersState.Dirty = true;
	}
}

void BakkesWebApi::UpdateGoalStats() {
	// Make sure we're online or NULL will strike :
	// Get all Pris
	ArrayWrapper<PriWrapper> players = this->gameWrapper->GetOnlineGame().GetPRIs();
	for (int a = 0; a < players.Count(); a = a + 1) {
		OldPlayersStates[a] = {
			players.Get(a).GetMatchGoals(),
			players.Get(a).GetPlayerName().ToString(),
			false
		};
	}
}

/**
 * Starts the websocket server
 */
void BakkesWebApi::RunWsServer() {
	// Init websocket server
	if (ws_server == NULL) {
		ws_server = new server();
	}
	else {
		ws_server->stop();
	}
	ws_server->init_asio();
	ws_server->set_open_handler(
		websocketpp::lib::bind(&BakkesWebApi::OnWsOpen, this, websocketpp::lib::placeholders::_1)
	);
	ws_server->set_close_handler(
		websocketpp::lib::bind(&BakkesWebApi::OnWsClose, this, websocketpp::lib::placeholders::_1)
	);
	ws_server->set_close_handler(
		websocketpp::lib::bind(&BakkesWebApi::OnWsClose, this, websocketpp::lib::placeholders::_1)
	);

	ws_server->set_http_handler(
		websocketpp::lib::bind(&BakkesWebApi::OnHttpRequest, this, websocketpp::lib::placeholders::_1)
	);
	ws_server->listen(8323);
	ws_server->start_accept();
	ws_server->run();
}


void BakkesWebApi::OnHttpRequest(websocketpp::connection_hdl hdl) {
	// add a connection to the set
	server::connection_ptr connection = ws_server->get_con_from_hdl(hdl);
	if (!gameWrapper->IsInOnlineGame()) {
		connection->set_body("Not playing");
		connection->set_status(websocketpp::http::status_code::conflict);
		return;
	}
	if (connection->get_resource() == "/players") {
		connection->set_body(
			jsonExport->wrapper(gameWrapper->GetOnlineGame().GetPRIs()).dump()
		);
		connection->set_status(websocketpp::http::status_code::ok);
		return;
	}
	if (connection->get_resource() == "/teams") {
		connection->set_body(
			jsonExport->wrapper(gameWrapper->GetOnlineGame().GetTeams()).dump()
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
		ws_server->send(it, payload, websocketpp::frame::opcode::text);
	}
}

void BakkesWebApi::onUnload() {
	gameWrapper->UnhookEvent("Function TAGame.PlayerController_TA.PostBeginPlay");
	gameWrapper->UnhookEvent("Function TAGame.GFxData_PRI_TA.HandleTeamChanged");
	gameWrapper->UnhookEvent("Function TAGame.ArenaSoundManager_TA.HandlePlayerScored");
	ws_server->stop();
	ws_connections->clear();
}