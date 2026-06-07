#include <iostream>
#include <string.h>
#include <vector>
#include <nlohmann/json.hpp>

namespace game {
	
	enum DamageType {
		physycs,
		heal,
		mechanics,
		unreal,
		physycs_splash,
		summon,
		special
	};

	class Base {
		int id;
		std::string codifier;
	};

	class CardItem : public Base{
		std::string name;
		std::string description;
		int level;
		int cost;
	};

	class Enemy : public Base {
		std::string name;
		std::string description;
		int level;
		int initiative;
		int hp;
		int moves;
		std::vector<int> enableCards;
		std::vector<int> hand;
	};

	class Player {
		int level;
		int initiative;
		int hp;
		int moves;
		std::vector<int> enableCards;
		std::vector<int> hand;
	};





}