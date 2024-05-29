#pragma once


	#include <iostream>
	#include <stdexcept>
	#include <tuple>
	#include <map>

	#define PARSING_MAP_OPT		std::map<std::string, int>
	#define PARSING_MAP_INFOS	std::map<std::string, std::string>
	#define PARSING_MAP_CMD		std::map<std::string, int>

	#define PARSING_ERR			"❌ ParsingError: "

	class Parsing
	{

		public:

			Parsing();
			~Parsing();

			template <typename M, typename K, typename V>
			static void parsing_map_insert(M& map_used, K key, V value);

			static std::string const parsing_get_option(void);
			static std::string const parsing_get_cmd(void);
			static std::string const parsing_get_username(void);
			static std::string const parsing_get_channel(void);

		private:

			PARSING_MAP_OPT		_options;
			PARSING_MAP_CMD		_cmd
			PARSING_MAP_INFOS	_infos;

	};