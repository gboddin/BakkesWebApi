#pragma once
#pragma comment( lib, "BakkesMod.lib" )

#include <set>
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"
#include "json.hpp"
#include "ApiPlayer.h"
#include "ApiTeam.h"
#include "ApiGame.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
using websocketpp::connection_hdl;

class BakkesWebApi : public BakkesMod::Plugin::BakkesModPlugin
{
	typedef websocketpp::server<websocketpp::config::asio> BakkesWebApiServer;
	typedef std::set<connection_hdl, std::owner_less<connection_hdl>> ConnectionSet;

public:
	virtual void onLoad();
	virtual void onUnload();
private:
	// Hook logic
	void HookMatchStarted();
	void HookGameStarted(std::string eventNam);
	void HookPlayerChanged(std::string eventNam);
	void HookGoalScored(std::string eventNam);

	// Player state logic
	ApiPlayer PlayersState[16];
	ApiPlayer CurrentPlayer;
	void UpdatePlayersState();
	void ClearPlayersState();
	json::JSON GetPlayersStateJson();

	// Team state logic
	ApiTeam TeamsState[4];
	void UpdateTeamsState();
	void ClearTeamsState();
	json::JSON GetTeamsStateJson();

	// Game state logic
	ApiGame GameState;
	void UpdateGameState();
	void ClearGameState();
	json::JSON GetGameStateJson();

	// Event logic
	void SendEvent(string eventName, ApiGame game);
	void SendEvent(string eventName, ApiPlayer player);
	//void SendEvent(string eventName, ApiPlayer player, ApiTeam team);
	//void SendEvent(string eventName, ApiPlayer player[], ApiTeam team[]);
	
	// Server logic
	BakkesWebApiServer* ws_server;
	ConnectionSet* ws_connections;
	void RunWsServer();
	void OnHttpRequest(connection_hdl hdl);	
	void SendWsPayload(string payload);
	void OnWsOpen(connection_hdl hdl) { this->ws_connections->insert(hdl); }
	void OnWsClose(connection_hdl hdl) { this->ws_connections->erase(hdl); }
	
	// Generic game type helper
	enum GAMETYPE {
		GAMETYPE_REPLAY = 0,
		GAMETYPE_ONLINE = 1,
		GAMETYPE_FREEPLAY = 2,
		GAMETYPE_TRAINING = 3,
		GAMETYPE_SPECTATE = 4,
		GAMETYPE_MENU = 5
	};
	ServerWrapper* GetCurrentGameType() {
		if (gameWrapper->IsInReplay()) {
			return &this->gameWrapper->GetGameEventAsReplay();
		}
		else if (gameWrapper->IsInOnlineGame()) {
			return &this->gameWrapper->GetOnlineGame();
		}
		else if (gameWrapper->IsInFreeplay()) {
			return &this->gameWrapper->GetGameEventAsServer();
		}
		else if (gameWrapper->IsInCustomTraining()) {
			return &this->gameWrapper->GetGameEventAsServer();
		}
		else if (gameWrapper->IsSpectatingInOnlineGame()) {
			return &this->gameWrapper->GetOnlineGame();
		}
		else {
			return NULL;
		}
	}
};