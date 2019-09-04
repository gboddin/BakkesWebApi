#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <set>
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"
#include "json.hpp"
#include "JsonExport.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
using websocketpp::connection_hdl;




struct PlayerState {
	int Goals = 0;
	string Name;
	bool Dirty = true;
};

class BakkesWebApi : public BakkesMod::Plugin::BakkesModPlugin
{
	typedef websocketpp::server<websocketpp::config::asio> server;
	typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;
public:
	virtual void onLoad();
	virtual void onUnload();
private:
	void HookMatchStarted(std::string eventNam);
	void HookPlayerChanged(std::string eventNam);
	void HookGoalScored(std::string eventNam);
	void UpdateGoalStats();
	void ClearGoalStats();
	void RunWsServer();
	void OnWsOpen(connection_hdl hdl);
	void OnHttpRequest(websocketpp::connection_hdl hdl);

	void OnWsClose(connection_hdl hdl);
	void SendWsPayload(string payload);
	PlayerState OldPlayersStates[16];
	JsonExport* jsonExport;
	server* ws_server;
	con_list* ws_connections;
};