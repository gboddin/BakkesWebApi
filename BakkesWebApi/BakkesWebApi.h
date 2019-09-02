#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#include <set>
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"
#include "bakkesmod\plugin\bakkesmodplugin.h"
#include "json.hpp"
using websocketpp::connection_hdl;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;
server* ws_server;
con_list m_connections;


struct PlayerState {
	int Goals = 0;
	string Name;
	bool Dirty = true;

};

class BakkesWebApi : public BakkesMod::Plugin::BakkesModPlugin
{
public:

	virtual void onLoad();
	void UpdateGoalStats(std::string eventNam);
	void ClearGoalStats(std::string eventName);
	PlayerState OldPlayersState[16];
private:
	void RunWsServer();
	void OnWsOpen(connection_hdl hdl);
	void OnWsClose(connection_hdl hdl);
	void SendWsPayload(string payload);
	json::JSON JsonPriWrapper(PriWrapper player);
	json::JSON JsonCarWrapper(CarWrapper car);
	json::JSON JsonTeamInfoWrapper(TeamInfoWrapper team);
	string GetGoalScoredEvent(PriWrapper player);
};
