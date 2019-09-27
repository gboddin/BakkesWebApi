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
	int TeamNum;
	int RankTier;
	int RankMatchesPlayed;
	int RankDivision;
	boolean IsBot;

	// stg tells me I have to learn mapping in C++ mmmh...
	enum {
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
		RANK_GRAND_CHAMP = 19
	};
	
	std::string GetRankName() {
		switch (this->RankTier) {
		case RANK_BRONZE_1:
			return "Bronze I";
		case RANK_BRONZE_2:
			return "Bronze 2";
		case RANK_BRONZE_3:
			return "Bronze 3";
		case RANK_SILVER_1:
			return "Silver I";
		case RANK_SILVER_2:
			return "Silver II";
		case RANK_SILVER_3:
			return "Silver III";
		case RANK_GOLD_1:
			return "Gold I";
		case RANK_GOLD_2:
			return "Gold II";
		case RANK_GOLD_3:
			return "Gold III";
		case RANK_PLAT_1:
			return "Platinum I";
		case RANK_PLAT_2:
			return "Platinum II";
		case RANK_PLAT_3:
			return "Platinum III";
		case RANK_DIAMOND_1:
			return "Diamond I";
		case RANK_DIAMOND_2:
			return "Diamond II";
		case RANK_DIAMOND_3:
			return "Diamond III";
		case RANK_CHAMP_1:
			return "Champion I";
		case RANK_CHAMP_2:
			return "Champion II";
		case RANK_CHAMP_3:
			return "Champion III";
		case RANK_GRAND_CHAMP:
			return "Grand Champion";
		};
		return "Unranked";
	};

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
		json_player["Rank"]["Division"] = RankDivision;
		json_player["Rank"]["Tier"] = RankTier;
		json_player["Rank"]["MatchesPlayed"] = RankMatchesPlayed;
		json_player["Rank"]["Name"] = this->GetRankName();
		return json_player;
	};

};