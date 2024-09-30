#include "../includes/Parsing.hpp"

// Initialisation de la liste d'emojis
static EMOJI good_emojis;
static EMOJI fail_emojis;

// Fonction pour obtenir un emoji aléatoire
std::string getRandomEmoji(bool GoodOrNot)
{
    EMOJI emojis;
    if (GoodOrNot)
        emojis = get_good_emoji();
    else
        emojis = get_fail_emoji();
        
    int index = rand() % emojis.size(); // Choisir un index aléatoire
    return (emojis[index]); // Retourner l'emoji correspondant
}

const EMOJI get_fail_emoji()
{
    // Initialiser le générateur de nombres aléatoires
    srand(static_cast<unsigned int>(time(0)));

    fail_emojis.push_back("🗿");
    fail_emojis.push_back("🫵");
    fail_emojis.push_back("👽");
    fail_emojis.push_back("🤖");
    fail_emojis.push_back("🤯");
    fail_emojis.push_back("🧐");
    fail_emojis.push_back("🫢");
    fail_emojis.push_back("😬");
    fail_emojis.push_back("😿");
    fail_emojis.push_back("👁 👄 👁");
    fail_emojis.push_back("👎");
    fail_emojis.push_back("💀");

    return (fail_emojis);
}

const EMOJI get_good_emoji()
{
    // Initialiser le générateur de nombres aléatoires
    srand(static_cast<unsigned int>(time(0)));

    good_emojis.push_back("😃");
    good_emojis.push_back("😎");
    good_emojis.push_back("🤓");
    good_emojis.push_back("😉");
    good_emojis.push_back("🫦");
    good_emojis.push_back("👌");
    good_emojis.push_back("👍");
    good_emojis.push_back("😸");
    good_emojis.push_back("🙂‍↔️");
    good_emojis.push_back("😁");
    good_emojis.push_back("😜");
    good_emojis.push_back("🫡");

    return (good_emojis);
}
