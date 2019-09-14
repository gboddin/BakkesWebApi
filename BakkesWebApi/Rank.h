#pragma once
#include "json.hpp"

class Rank {
public:
	int playlistId;
	std::string Name;
	float upperMmr;
	float lowerMmr;
	bool MatchMmrUp(int playlistId, float mmr) {
		return this->playlistId == playlistId && this->lowerMmr > mmr && this->upperMmr < mmr;
	};
	bool MatchMmrDown(int playlistId, float mmr) {
		return this->playlistId == playlistId && this->lowerMmr > mmr && this->upperMmr > mmr;
	};
};