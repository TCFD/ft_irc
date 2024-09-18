#include "../includes/Parsing.hpp"

void    Parsing::parsingHelp()
{
    // Pour tous les elmts dans _cmd, afficher la commande et la forme requise

    bool    save_duplicate = _duplicates_found;
    
    std::string actual_cmd_save;
    std::string actual_brut_form_save;

    PARSING_VECTOR_SPLIT actual_split_form_save;

    actual_cmd_save = _actualCmd;
    actual_brut_form_save = _actualBrutForm;
    actual_split_form_save = _actual_split_form;

    for (PARSING_MAP_CMD::iterator elmt = _cmd.begin(); elmt != _cmd.end(); elmt++)
    {
        _duplicates_found = 0;

        _actualCmd = (*elmt).first;
        _actualBrutForm = (*elmt).second.second;
        _actual_split_form = split(_actualBrutForm, ' ');

        if (_actualBrutForm.find(',') != std::string::npos)
            _duplicates_found = 1;

        errWriteCorrectForm("");
    }

    _actualCmd = actual_cmd_save;
    _actualBrutForm = actual_brut_form_save;
    _actual_split_form = actual_split_form_save;
    _duplicates_found = save_duplicate;
}