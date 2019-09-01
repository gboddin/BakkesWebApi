#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#include <set>
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"
#include "bakkesmod\plugin\bakkesmodplugin.h"

using websocketpp::connection_hdl;

typedef websocketpp::server<websocketpp::config::asio> server;


struct PlayerState {
	int Goals = 0;
	string Name;
	bool Dirty = true;
};

class BakkesTest : public BakkesMod::Plugin::BakkesModPlugin
{
public:

	virtual void onLoad();
	void UpdateGoalStats(std::string eventNam);
	void ClearGoalStats(std::string eventName);
	PlayerState OldPlayersState[16];
private:
	typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;
	server ws_server;
	con_list m_connections;
	void OnWsOpen(connection_hdl hdl);
	void OnWsClose(connection_hdl hdl);
	void SendWsPayload(string payload);
};
