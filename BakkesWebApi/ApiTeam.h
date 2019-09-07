#pragma once
#include "json.hpp"

class ApiTeam {
public:
	bool Dirty = true;
	std::string Name;
	int Goals;
	json::JSON getJson() {
		json::JSON json_team;
		json_team["Name"] = Name;
		json_team["Goals"] = Goals;
		return json_team;
	};
};