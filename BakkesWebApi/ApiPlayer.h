#pragma once
#include "json.hpp"

class ApiPlayer {
public:
	bool Dirty = true;
	std::string PlayerName;
	int Goals;
	float MMR;
	int Assists;
	int Saves;
	int PlayerID;
	unsigned long long PlayerUniqueID;
	int Kills;
	int BallTouches;
	float CurrentBoostAmount;
	boolean IsBot;
	json::JSON getJson() {
		json::JSON json_player;
		json_player["PlayerName"] = PlayerName;
		json_player["Goals"] = Goals;
		json_player["MMR"] = MMR;
		json_player["Assists"] = Assists;
		json_player["Saves"] = Saves;
		json_player["PlayerID"] = PlayerID;
		json_player["Kills"] = Kills;
		json_player["BallTouches"] = BallTouches;
		json_player["CurrentBoostAmount"] = CurrentBoostAmount;
		json_player["PlayerUniqueID"] = PlayerUniqueID;
		return json_player;
	};
};