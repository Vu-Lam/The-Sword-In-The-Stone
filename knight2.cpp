#include "knight2.h"
/* * * BEGIN implementation of class Events * * */
Events::Events(const string file_events)
{
    fstream fin(file_events);
    fin >> total_events;
    int *arr_events = new int[total_events];
    for (int i = 0; i < total_events; i++)
    {
        fin >> arr_events[i];
    }
    data = arr_events;
}
int Events::count() const { return total_events; }
int Events::get(int i) const { return data[i]; }
Events::~Events() { delete[] data; }
/* * * END implementation of class Events * * */

/* * * BEGIN implementation of class BaseBag * * */

/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */
string BaseKnight::toString() const
{
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id) + ",hp:" + to_string(hp) + ",maxhp:" + to_string(maxhp) + ",level:" + to_string(level) + ",gil:" + to_string(gil) + "," + bag->toString() + ",knight_type:" + typeString[knightType] + "]";
    return s;
}

/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
ArmyKnights::ArmyKnights(const string &file_armyknights)
{
    fstream fin(file_armyknights);
    fin >> total_knights;
    BaseKnight *k = new BaseKnight[total_knights];
    for (int i = 0; i < total_knights; i++)
    {
        int hp, level, phoenixdownI, gil, antidote;
        fin >> hp;
        fin >> level;
        fin >> phoenixdownI;
        fin >> gil;
        fin >> antidote;
        k[i].create(i + 1, hp, level, gil, antidote, phoenixdownI);
    }
    array_knights = k;
}
ArmyKnights::~ArmyKnights() { delete[] array_knights; }
void ArmyKnights::printInfo() const
{
    cout << "No. knights: " << this->count();
    if (this->count() > 0)
    {
        BaseKnight *lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
         << ";LancelotSpear:" << this->hasLancelotSpear()
         << ";GuinevereHair:" << this->hasGuinevereHair()
         << ";ExcaliburSword:" << this->hasExcaliburSword()
         << endl
         << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const
{
    cout << (win ? "WIN" : "LOSE") << endl;
}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class BaseOpponent * * */
BaseOpponent::BaseOpponent(int i, int event_id) { levelO = (i + event_id) % 10 + 1; }
/* * * END implementation of class BaseOpponent * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure()
{
    armyKnights = nullptr;
    events = nullptr;
}

/* * * END implementation of class KnightAdventure * * */