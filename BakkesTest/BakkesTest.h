#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#include "bakkesmod\plugin\bakkesmodplugin.h"


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
};
