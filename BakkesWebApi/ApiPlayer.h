#pragma once
#include "json.hpp"
#include "ApiGame.h"
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
	int TeamNum;
	boolean IsBot;
	json::JSON getJson() {
		json::JSON json_player;
		json_player["PlayerName"] = PlayerName;
		json_player["Goals"] = Goals;
		json_player["MMR"] = MMR;
		json_player["Assists"] = Assists;
		json_player["Saves"] = Saves;
		json_player["PlayerID"] = PlayerID;
		json_player["PlayerUniqueID"] = PlayerUniqueID;
		json_player["Kills"] = Kills;
		json_player["BallTouches"] = BallTouches;
		json_player["CurrentBoostAmount"] = CurrentBoostAmount;
		json_player["TeamNum"] = TeamNum;
		json_player["IsBot"] = IsBot;
		return json_player;
	};


	enum RANK {
		RANK_UNRANKED = 0,
		RANK_BRONZE_1 = 1,
		RANK_BRONZE_2 = 2,
		RANK_BRONZE_3 = 3,

		RANK_SILVER_1 = 4,
		RANK_SILVER_2 = 5,
		RANK_SILVER_3 = 6,

		RANK_GOLD_1 = 7,
		RANK_GOLD_2 = 8,
		RANK_GOLD_3 = 9,

		RANK_PLAT_1 = 10,
		RANK_PLAT_2 = 11,
		RANK_PLAT_3 = 12,

		RANK_DIAMOND_1 = 13,
		RANK_DIAMOND_2 = 14,
		RANK_DIAMOND_3 = 15,

		RANK_CHAMP_1 = 16,
		RANK_CHAMP_2 = 17,
		RANK_CHAMP_3 = 18,

		RANK_GRAND_CHAMP = 19,

	};

	typedef std::map<int,std::pair<float, int>> mmrMap;

	mmrMap SuperMap = {
		{ApiGame::PLAYLIST_CASUAL_STANDARD ,{0,RANK_UNRANKED}},
		{1,{0,0}}
	};
};