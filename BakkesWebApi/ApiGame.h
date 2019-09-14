#pragma once
#include "json.hpp"
using namespace std;
class ApiGame {

public:
	enum GAMETYPE {
		GAMETYPE_REPLAY = 0,
		GAMETYPE_ONLINE = 1,
		GAMETYPE_FREEPLAY = 2,
		GAMETYPE_TRAINING = 3,
		GAMETYPE_SPECTATE = 4,
		GAMETYPE_MENU = 5
	};
	enum PLAYLIST {
		PLAYLIST_CASUAL_DUEL = 1,
		PLAYLIST_CASUAL_DOUBLES = 2,
		PLAYLIST_CASUAL_STANDARD = 3,
		PLAYLIST_CASUAL_CHAOS = 4,
		PLAYLIST_MENU = 5,
		PLAYLIST_PRIVATE = 6,
		PLAYLIST_FREEPLAY = 9,
		PLAYLIST_RANKED_DUEL = 10,
		PLAYLIST_RANKED_DOUBLES = 11,
		PLAYLIST_RANKED_SOLO = 12,
		PLAYLIST_RANKED_STANDARD = 13,
		PLAYLIST_RANKED_HOOPS = 27,
		PLAYLIST_RANKED_RUMBLE = 28,
		PLAYLIST_RANKED_DROPSHOT = 29,
		PLAYLIST_RANKED_SNOWDAY = 30
	};
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
		case(PLAYLIST_CASUAL_DUEL):
			return "Casual Duel";
		case(PLAYLIST_CASUAL_DOUBLES):
			return "Casual Doubles";
		case(PLAYLIST_CASUAL_STANDARD):
			return "Casual Standard";
		case(PLAYLIST_CASUAL_CHAOS):
			return "Casual Chaos";
		case(PLAYLIST_MENU):
			return "Menu";
		case(PLAYLIST_PRIVATE):
			return "Private " + to_string(this->TeamSize) + "v" + to_string(this->TeamSize);
		case(PLAYLIST_FREEPLAY):
			return "Freeplay";
		case(PLAYLIST_RANKED_DUEL):
			return "Ranked Duel";
		case(PLAYLIST_RANKED_DOUBLES):
			return "Ranked Doubles";
		case(PLAYLIST_RANKED_SOLO):
			return "Ranked Solo Standard";
		case(PLAYLIST_RANKED_STANDARD):
			return "Ranked Standard";
		case(14):
			return "Mutator Mashup";
		case(22):
			return "Tournament " + to_string(this->TeamSize) + "v" + to_string(this->TeamSize);
		case(PLAYLIST_RANKED_HOOPS):
			return "Ranked Hoops";
		case(PLAYLIST_RANKED_RUMBLE):
			return "Ranked Rumble";
		case(PLAYLIST_RANKED_DROPSHOT):
			return "Ranked Dropshot";
		case(PLAYLIST_RANKED_SNOWDAY):
			return "Ranked Snowday";
		default:
			return "Charting unknown territories";
		}
	}
};