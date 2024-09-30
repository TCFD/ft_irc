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
	#define HELP_OPTION				"<-/+option>"
	#define HELP_HOST				"<localhost>"


	//////////// CLASS PARSING ////////////
	class Parsing
	{

		public:

			Parsing(void);
			~Parsing(void);

		// GETTERS
			std::string parsingGetCommand(void) const;
			std::string parsingGetMessage(void);
			std::string parsingGetPassword(void);
			std::string parsingGetOption(void);
			std::string parsingGetUsername(void);
			std::string parsingGetChannel(void);
			std::string parsingGetHost(void);
			void		cmdStatus(void);

		// MAIN METHODS
			void    		parsingHelp();
			void			errMissElmt(PARSING_VECTOR_SPLIT& cmd_split);
			void			errWriteCorrectForm(std::string gap);
			std::string		cmdTreatTest(std::string brut_cmd);
			bool			formVerification(PARSING_VECTOR_SPLIT& cmd_split,
								PARSING_VECTOR_SPLIT& form_split);

			std::pair<std::string, std::string> parsingGetCmd(void);

			// EXCEPTION
			class ParsingInvalidSyntax : public std::exception
			{
				public:

			  		ParsingInvalidSyntax(const std::string& str1)
			    		: _mMsg(str1){}

			  		virtual const char* what() const throw()
			  		{
			    		return (_mMsg.c_str());
			  		}

					 virtual ~ParsingInvalidSyntax(void) throw() {}
				
				private:
					
					std::string	_mMsg;
			};
			
			class ParsingInvalidCommand : public std::exception
			{
				public:

			  		ParsingInvalidCommand(const std::string& str1)
			    		: _mMsg(str1){}

			  		virtual const char* what() const throw()
			  		{
			    		return (_mMsg.c_str());
			  		}

					 virtual ~ParsingInvalidCommand(void) throw() {}
				
				private:
					
					std::string	_mMsg;
			};

		private:

			bool					_duplicates_found;
			std::string				_actualCmd;
			std::string				_actualBrutForm;
			std::string				_brutCmd;
			PARSING_VECTOR_SPLIT	_actual_split_form;
			PARSING_MAP_ERR			_err_map;
			PARSING_MAP_OPT			_options;
			PARSING_MAP_CMD			_cmd;
			PARSING_MAP_INFOS		_infos;
			
	
			// void	_check_form(PARSING_VECTOR_SPLIT cmd_split, PARSING_VECTOR_SPLIT form_split);
			std::string		anyDuplicates(PARSING_VECTOR_SPLIT& cmd_split, std::string& cmd_form);
			void 			errFormWriting(PARSING_VECTOR_SPLIT& form);
			void			cmdResetStatus(void);
			bool			elmtAttribution(char identifier, std::string CMDSplit_value);
			bool			attributionInfoChannel(std::string& CMDSplit_value);
			bool			attributionInfoOption(std::string& CMDSplit_value);
			bool			attributionInfoMessage(std::string& CMDSplit_value);
			bool			attributionInfoUsername(std::string& CMDSplit_value);
			bool			attributionInfoPassword(std::string& CMDSplit_value);
			bool			attributionInfoHost(std::string& CMDSplit_value);
	};

	//////////// FUNCTIONS ////////////

	PARSING_VECTOR_SPLIT	split(const std::string &str, char delimiter);
	PARSING_VECTOR_SPLIT	remove_between_angles(const PARSING_VECTOR_SPLIT& input);
	std::string const&		byidx(PARSING_VECTOR_SPLIT& v, int index);
	std::string				str_cut(std::string str, int start_idx, int end_idx);
	std::string				concat_vector_elmt(PARSING_VECTOR_SPLIT tab);
	bool					find_one_of_them(std::string cara_search, std::string& str_origin);
	int						len_of_tab_with_intervals(PARSING_VECTOR_SPLIT tab, char cara1, char cara2);
	int						tab_idx_elmt(PARSING_VECTOR_SPLIT tab, char cara);

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