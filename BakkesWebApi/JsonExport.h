#pragma once
#include "json.hpp"
#include "bakkesmod/wrappers/gamewrapper.h"
class JsonExport
{
public:
	std::shared_ptr<GameWrapper> gameWrapper;
	JsonExport(std::shared_ptr<GameWrapper> LocalGameWrapper) {
		gameWrapper = LocalGameWrapper;
	}
	json::JSON wrapper(ArrayWrapper<PriWrapper> players) {
		json::JSON json_players;
		for (int a = 0; a < players.Count(); a = a + 1) {
			json_players.append(wrapper(players.Get(a)));
		}
		return json_players;
	}
	json::JSON wrapper(ArrayWrapper<TeamInfoWrapper> teams) {
		json::JSON json_teams;
		for (int a = 0; a < teams.Count(); a = a + 1) {
			json_teams.append(wrapper(teams.Get(a)));
		}
		return json_teams;
	}
	json::JSON wrapper(ArrayWrapper<TeamWrapper> teams) {
		json::JSON json_teams;
		for (int a = 0; a < teams.Count(); a = a + 1) {
			json_teams.append(wrapper(teams.Get(a)));
		}
		return json_teams;
	}
	json::JSON wrapper(PriWrapper player) {
		json::JSON json_player;
		json_player["PlayerID"] = player.GetPlayerID();
		json_player["UniqueId"] = player.GetUniqueId().ID;
		json_player["PlayerName"] = player.GetPlayerName().ToString();
		json_player["MatchGoals"] = player.GetMatchGoals();
		json_player["MatchAssists"] = player.GetMatchAssists();
		json_player["MatchScore"] = player.GetMatchScore();
		json_player["Kills"] = player.GetKills();
		json_player["BallTouches"] = player.GetBallTouches();
		json_player["TeamInfo"] = this->wrapper(player.GetTeam());
		json_player["MMR"] = this->GetPlayerMMR(player);
		json_player["CurrentBoostAmount"] = player.GetCar().GetBoostComponent().GetCurrentBoostAmount();
		return json_player;
	}
	json::JSON event(string eventName, PriWrapper player) {
		json::JSON json_event;
		json_event["event"] = eventName;
		json_event["player"] = this->wrapper(player);
		return json_event;
	}
	json::JSON event(string eventName, TeamInfoWrapper team) {
		json::JSON json_event;
		json_event["event"] = eventName;
		json_event["team"] = this->wrapper(team);
		return json_event;
	}
	json::JSON wrapper(TeamInfoWrapper team) {
		json::JSON json_team;
		json_team["TeamIndex"] = team.GetTeamIndex();
		json_team["TeamNum"] = team.GetTeamNum();
		json_team["TeamNum2"] = team.GetTeamNum2();
		json_team["Size"] = team.GetSize();
		json_team["TeamName"] = team.GetTeamName().ToString();
		json_team["Score"] = team.GetScore();
		return json_team;
	}
	json::JSON wrapper(TeamWrapper team) {
		json::JSON json_team;
		json_team["TeamIndex"] = team.GetTeamIndex();
		json_team["TeamNum"] = team.GetTeamNum();
		json_team["TeamNum2"] = team.GetTeamNum2();
		json_team["Size"] = team.GetSize();
		json_team["TeamName"] = team.GetTeamName().ToString();
		json_team["SanitizedTeamName"] = team.GetSanitizedTeamName().ToString();
		json_team["Score"] = team.GetScore();
		return json_team;
	}

	/**
     * Get MMR from PriWrapper
     */
	unsigned long long GetPlayerMMR(PriWrapper player) {
		// we'll have to see why there's no ID here ...
		return 0;
		MMRWrapper mmrWrapper = gameWrapper->GetMMRWrapper();
		if(mmrWrapper.IsRanked(mmrWrapper.GetCurrentPlaylist())) {
			return mmrWrapper.GetPlayerMMR(
				player.GetUniqueId(),
				mmrWrapper.GetCurrentPlaylist()
			);
		}
	}
};



