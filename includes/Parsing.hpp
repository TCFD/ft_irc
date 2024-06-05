#pragma once


	#include <iostream>
	#include <stdexcept>
	#include <vector>
	#include <string>
	#include <sstream>
	#include <utility>
	#include <map>

	#define PARSING_VECTOR_SPLIT	std::vector<std::string>

	#define PARSING_MAP_OPT			std::map<std::string, int>
	#define PARSING_MAP_ERR			std::map<std::string, std::string>
	#define PARSING_MAP_INFOS		std::map<std::string, std::string>
	#define PARSING_MAP_CMD			std::map<std::string, std::pair<int, std::string> >

	#define PARSING_ERR				"❌ ParsingError: "
	#define CONTAINER_ERR			"❌ ContainerError: "
	#define CMD_ERR					"❌ CmdError: "
	#define FORM_ERR				"❌ FormError: "

	#define HELP_COMMAND			"</command>"
	#define HELP_CHANNEL			"<#channel>"
	#define HELP_USERNAME			"<username>"
	#define HELP_PASSWORD			"<password>"
	#define HELP_MESSAGE			"<:message>"
	#define HELP_OPTION				"<-option>"


	class Parsing
	{

		public:

			Parsing();
			~Parsing();

			std::pair<std::string, std::string> parsing_get_cmd(void);

			std::string const parsing_get_option(void);
			std::string const parsing_get_username(void);
			std::string const parsing_get_channel(void);

			void	err_miss_elmt(PARSING_VECTOR_SPLIT& cmd_split);
			void	err_write_correct_form();
			void	cmd_treat_test(std::string brut_cmd);

			bool	form_verification(PARSING_VECTOR_SPLIT& cmd_split,
						PARSING_VECTOR_SPLIT& form_split);

			// EXCEPTION
			class ParsingInvalidSyntax : public std::exception
			{
				public:

			  		ParsingInvalidSyntax(const std::string& str1)
			    		: _m_msg(str1){}

			  		virtual const char* what() const throw()
			  		{
			    		return (_m_msg.c_str());
			  		}

					 virtual ~ParsingInvalidSyntax() throw() {}
				
				private:
					
					std::string	_m_msg;
			};

		private:

			std::string		_any_duplicates(PARSING_VECTOR_SPLIT& cmd_split, std::string& cmd_form);
			std::string		_actual_cmd;
			std::string		_actual_brut_form;


			void 	_err_form_writing(PARSING_VECTOR_SPLIT& form);

			void	_check_form(PARSING_VECTOR_SPLIT cmd_split, PARSING_VECTOR_SPLIT form_split);
			void	_cmd_reset_status(void);
			
			bool	_elmt_attribution(char identifier, std::string CMDSplit_value);
			bool	_attribution_info_channel(std::string& CMDSplit_value);
			bool	_attribution_info_option(std::string& CMDSplit_value);
			bool	_attribution_info_message(std::string& CMDSplit_value);
			bool	_attribution_info_username(std::string& CMDSplit_value);
			bool	_attribution_info_password(std::string& CMDSplit_value);


			bool	_duplicates_found;

			PARSING_VECTOR_SPLIT	_actual_split_form;
			PARSING_MAP_ERR			_err_map;

			PARSING_MAP_OPT			_options;
			PARSING_MAP_CMD			_cmd;
			PARSING_MAP_INFOS		_infos;
	};


	PARSING_VECTOR_SPLIT	split(const std::string &str, char delimiter);
	std::string const&		byidx(PARSING_VECTOR_SPLIT& v, int index);
	bool					find_one_of_them(std::string cara_search, std::string& str_origin);

	//////////// TEMPLATES ////////////

	template <typename M>
	bool find_key_in_container(M& map_used, std::string key)
	{
		if (map_used.find(key) != map_used.end())
			return (true);
	    return (false);
	}

	template <typename M>
	std::string const check_infos(M& map_used, std::string keyword)
	{
	    typename M::iterator it = map_used.find(keyword);
	    if (it != map_used.end())
	        return it->second;
	    return "";
	}

	template <typename TA, typename TB>
	std::pair<TA, TB> pair_it(TA a, TB b)
	{
	    return std::make_pair(a, b);
	}