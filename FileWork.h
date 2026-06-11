#pragma once

#ifndef FILE_H
#define FILE_H

//Include block
#include <cstdlib>   
#include <ctime>     
#include <algorithm> 
#include <set>
#include "game.h"
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <cmath>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

//Using block
using json = nlohmann::json;

//file namespace
namespace file {

	class FileManager {

	public:
		//serializers block
		void to_json(json& j, const game::Enemy& e);
		void to_json(json& j, const std::vector<game::Enemy>& vec);
		void to_json(json& j, const game::CardItem& c);
		void to_json(json& j, const std::vector<game::CardItem>& vec);
		void to_json(json& j, const game::Event& e);
		void to_json(json& j, const std::vector<game::Event>& vec);

		//deserializers block
		void from_json(const json& j, game::Enemy& e);
		void from_json(const json& j, std::vector<game::Enemy>& vec);
		void from_json(const json& j, game::CardItem& c);
		void from_json(const json& j, std::vector<game::CardItem>& vec);
        void from_json(const json& j, game::Event& e);
		void from_json(const json& j, std::vector<game::Event>& vec);

		//write block
		void writeEnemiesToFile(const std::string& filename, const std::vector<game::Enemy>& enemies);
		void writeCardsToFile(const std::string& filename, const std::vector<game::CardItem>& cards);
		void writeEventsToFile(const std::string& filename, const std::vector<game::Event>& events);

		//read block
		std::vector<game::Enemy> readEnemiesFromFile(const std::string& filename);
		std::vector<game::CardItem> readCardsFromFile(const std::string& filename);
		std::vector<game::Event> readEventsFromFile(const std::string& filename);

		//Sub functions
		std::string damageTypeToString(game::DamageType dt);
		game::DamageType stringToDamageType(const std::string& str);
		std::string eventTypeToString(game::EventType et);
		game::EventType stringToEventType(const std::string& str);

	};

}


#endif