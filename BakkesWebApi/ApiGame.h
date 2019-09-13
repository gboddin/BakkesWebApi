#pragma once
#include "json.hpp"
using namespace std;
class ApiGame {
public:
	int PlaylistId = 5;
	int TeamSize = 0;
	int SecondsRemaining = 0;
	float SecondsElapsed = 0;
	json::JSON getJson() {
		json::JSON json_game;
		json_game["PlaylistName"] = this->GetPlaylistName();
		json_game["PlaylistId"] = PlaylistId;
		json_game["TeamSize"] = TeamSize;
		json_game["SecondsRemaining"] = SecondsRemaining;
		json_game["SecondsElapsed"] = SecondsElapsed;
		return json_game;
	};

	/**
	 * Credits to Henry https://github.com/segalll/DiscordRPCPlugin/blob/master/DiscordRPCPlugin/DiscordRPCPlugin.cpp
	 */
	std::string GetPlaylistName() {
		switch (this->PlaylistId) {
		case(1):
			return "Casual Duel";
		case(2):
			return "Casual Doubles";
		case(3):
			return "Casual Standard";
		case(4):
			return "Casual Chaos";
		case(5):
			return "Menu";
		case(6):
			return "Private " + to_string(this->TeamSize) + "v" + to_string(this->TeamSize);
		case(9):
			return "Freeplay";
		case(10):
			return "Ranked Duel";
		case(11):
			return "Ranked Doubles";
		case(12):
			return "Ranked Solo Standard";
		case(13):
			return "Ranked Standard";
		case(14):
			return "Mutator Mashup";
		case(22):
			return "Tournament " + to_string(this->TeamSize) + "v" + to_string(this->TeamSize);
		case(27):
			return "Ranked Hoops";
		case(28):
			return "Ranked Rumble";
		case(29):
			return "Ranked Dropshot";
		case(30):
			return "Ranked Snowday";
		default:
			return "Charting unknown territories";
		}
	}
};