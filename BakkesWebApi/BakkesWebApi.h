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
	void HookMatchStarted(std::string eventNam);
	void HookPlayerChanged(std::string eventNam);
	void HookGoalScored(std::string eventNam);

	// Player state logic
	ApiPlayer PlayersState[16];
	ApiPlayer CurrentPlayer;
	void UpdatePlayersState();
	void ClearPlayersState();
	json::JSON GetPlayersStateJson();

	// Team state logic
	ApiTeam TeamsState[2];
	void UpdateTeamsState();
	void ClearTeamsState();
	json::JSON GetTeamsStateJson();

	// Event logic
	void SendEvent(string eventName, ApiPlayer player);

	// Server logic
	BakkesWebApiServer* ws_server;
	ConnectionSet* ws_connections;
	void RunWsServer();
	void OnWsOpen(connection_hdl hdl);
	void OnHttpRequest(connection_hdl hdl);
	void OnWsClose(connection_hdl hdl);
	void SendWsPayload(string payload);
};