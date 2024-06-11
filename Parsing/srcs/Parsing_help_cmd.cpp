#include "../includes/Parsing.hpp"

void    Parsing::parsing_help()
{
    // Pour tous les elmts dans _cmd, afficher la commande et la forme requise

    bool    save_duplicate = _duplicates_found;
    
    std::string actual_cmd_save;
    std::string actual_brut_form_save;

    PARSING_VECTOR_SPLIT actual_split_form_save;

    actual_cmd_save = _actual_cmd;
    actual_brut_form_save = _actual_brut_form;
    actual_split_form_save = _actual_split_form;

    for (PARSING_MAP_CMD::iterator elmt = _cmd.begin(); elmt != _cmd.end(); elmt++)
    {
        _duplicates_found = 0;

        _actual_cmd = (*elmt).first;
        _actual_brut_form = (*elmt).second.second;
        _actual_split_form = split(_actual_brut_form, ' ');

        if (_actual_brut_form.find(',') != std::string::npos)
            _duplicates_found = 1;

        err_write_correct_form("");
    }

    _actual_cmd = actual_cmd_save;
    _actual_brut_form = actual_brut_form_save;
    _actual_split_form = actual_split_form_save;
    _duplicates_found = save_duplicate;
}