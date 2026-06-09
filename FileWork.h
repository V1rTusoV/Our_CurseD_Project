#pragma once

#ifndef FILE_H
#define FILE_H

#include <cstdlib>   
#include <ctime>     
#include <algorithm> 
#include <set>
#include "game.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <string>
#include <ctime>
#include <cstdlib>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace file {

	class FileManager {

	public:

		void to_json(json& j, const game::Enemy& e);
		void from_json(const json& j, game::Enemy& e);

		void to_json(json& j, const std::vector<game::Enemy>& vec);
		void from_json(const json& j, std::vector<game::Enemy>& vec);

		void to_json(json& j, const game::CardItem& c);
		void from_json(const json& j, game::CardItem& c);

		void to_json(json& j, const std::vector<game::CardItem>& vec);
		void from_json(const json& j, std::vector<game::CardItem>& vec);

		void writeEnemiesToFile(const std::string& filename, const std::vector<game::Enemy>& enemies);
		std::vector<game::Enemy> readEnemiesFromFile(const std::string& filename);

		void writeCardsToFile(const std::string& filename, const std::vector<game::CardItem>& cards);
		std::vector<game::CardItem> readCardsFromFile(const std::string& filename);

		void test();

		std::string damageTypeToString(game::DamageType dt);
		game::DamageType stringToDamageType(const std::string& str);
	};

}


#endif