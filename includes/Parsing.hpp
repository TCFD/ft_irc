#pragma once


	#include <iostream>
	#include <stdexcept>
	#include <vector>
	#include <string>
	#include <sstream>
	#include <utility>
	#include <map>

	#define PARSING_MAP_OPT		std::map<std::string, int>
	#define PARSING_MAP_INFOS	std::map<std::string, std::string>
	#define PARSING_MAP_CMD		std::map<std::string, std::pair<int, std::string> >

	#define PARSING_ERR			"❌ ParsingError: "
	#define CONTAINER_ERR		"❌ ContainerError: "
	#define CMD_ERR				"❌ CmdError: "
	#define FORM_ERR			"❌ FormError: "


	class Parsing
	{

		public:

			Parsing();
			~Parsing();

			//template <typename M, typename K, typename V>
			//static void parsing_map_insert(M& map_used, K key, V value);

			std::pair<std::string, std::string> parsing_get_cmd(void);

			std::string const parsing_get_option(void);
			std::string const parsing_get_username(void);
			std::string const parsing_get_channel(void);

			void	cmd_treat_test(std::string brut_cmd);

		private:

			void	_check_form(std::vector<std::string> cmd_split, std::vector<std::string> form_split);
			void	_cmd_reset_status(void);

			PARSING_MAP_OPT		_options;
			PARSING_MAP_CMD		_cmd;
			PARSING_MAP_INFOS	_infos;

	};