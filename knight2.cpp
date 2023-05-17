#include "knight2.h"
/* * * BEGIN implementation of class Events * * */
Events::Events(const string& file_events)
{
    ifstream fin(file_events);
    if(!fin.is_open()) cout << "File events not found\n";
    fin >> total_events;
    int *arr_events = new int[total_events];
    for (int i = 0; i < total_events; i++)
    {
        fin >> arr_events[i];
    }
    data = arr_events;
    fin.close();
}
int Events::count() const { return total_events; }
int Events::get(int i) const { return data[i]; }
Events::~Events() { delete[] data; }
/* * * END implementation of class Events * * */


/* * * BEGIN implementation of class BaseOpponent * * */
BaseOpponent::BaseOpponent(int i, int event_id) { levelO = (i + event_id) % 10 + 1; gil = 0; baseDamage=0;}
/* * * END implementation of class BaseOpponent * * */


/* * * BEGIN implementation of class Item * * */
Antidote::Antidote(ItemType t) : BaseItem(t) { type = ANTIDOTE; }
void Antidote::use(BaseKnight *knight) { knight->setIsPoisoned(false); knight->setAntidote(knight->getAntidote()-1);}
ItemType Antidote::getType() { return type; }
void Antidote::setType(ItemType x) { type = x; }
bool Antidote::canUse(BaseKnight *knight) {return knight->isPoisoned(); }
PhoenixDownI::PhoenixDownI(ItemType t) : BaseItem(t){ type = PHOENIXDOWNI;}
void PhoenixDownI::use(BaseKnight *knight) { knight->setHP(knight->getMaxHP()); }
ItemType PhoenixDownI::getType() { return type; }
void PhoenixDownI::setType(ItemType x) { type = x; }
bool PhoenixDownI::canUse(BaseKnight *knight) {return knight->getHP() <= 0; }
PhoenixDownII::PhoenixDownII(ItemType t) : BaseItem(t) {type = PHOENIXDOWNII;}
bool PhoenixDownII::canUse(BaseKnight *knight) {return knight->getHP() < (knight->getMaxHP()/4); }
void PhoenixDownII::use(BaseKnight *knight) { knight->setHP(knight->getMaxHP()); }
ItemType PhoenixDownII::getType() { return type; }
void PhoenixDownII::setType(ItemType x) { type = x; }
PhoenixDownIII::PhoenixDownIII(ItemType t) : BaseItem(t){type = PHOENIXDOWNIII;}
void PhoenixDownIII::use(BaseKnight *knight) {
    if(knight->getHP()<1) knight->setHP(knight->getMaxHP()/3);
    else {
        int newHP = knight->getHP() + knight->getMaxHP()/4;
        knight->setHP(newHP);
    }
}
bool PhoenixDownIII::canUse(BaseKnight *knight) {return knight->getHP() < (knight->getMaxHP()/3); }
ItemType PhoenixDownIII::getType() { return type; }
void PhoenixDownIII::setType(ItemType x) { type = x; }
PhoenixDownIV::PhoenixDownIV(ItemType t) : BaseItem(t){type = PHOENIXDOWNIV;}
void PhoenixDownIV::use(BaseKnight *knight) {
    if(knight->getHP()<1) knight->setHP(knight->getMaxHP()/2);
    else {
        int newHP = knight->getHP() + knight->getMaxHP()/5;
        knight->setHP(newHP);
    }
}
ItemType PhoenixDownIV::getType() { return type; }
void PhoenixDownIV::setType(ItemType x) { type = x; }
bool PhoenixDownIV::canUse(BaseKnight *knight) {return knight->getHP() < (knight->getMaxHP()/2); }
/* * * END implementation of class Item * * */


/* * * BEGIN implementation of class BaseBag * * */
BaseBag::BaseBag(BaseKnight *k, int phoenixdownI, int antidote) {
    knight = k;
    totalI = 0;
    head = nullptr;
    if (phoenixdownI > 0)
    {
        for (int i = 0; i < phoenixdownI; i++)
        {
            BaseItem *temp = new PhoenixDownI(PHOENIXDOWNI);
            if (head == nullptr)
            {
                head = temp;
                head->setType(PHOENIXDOWNI);
            }
            else
            {
                BaseItem *j = head;
                while (j->next != nullptr)
                    j = j->next;
                j->next = temp;
                j->next->setType(PHOENIXDOWNI);
            }
            ++totalI;
        }
    }
    if (antidote > 0 && knight->getType() != DRAGON)
    {
        for (int i = 0; i < antidote; i++)
        {
            BaseItem *temp = new Antidote(ANTIDOTE);
            if (head == nullptr)
            {
                head = temp;
                head->setType(ANTIDOTE);
            }
            else
            {
                temp->next = head;
                head = temp;
            }
            ++totalI;
        }
    }
}
bool BaseBag::insertFirst(BaseItem *item) {
    if (canBeAdd(item->getType()))
    {
        item->next = head;
        head = item;
        head->setType(item->getType());
        ++totalI;
        return true;
    }
    else
        return false;
}
bool BaseBag::canBeAdd(ItemType t) {
    if (knight->getType() == DRAGON && t == ANTIDOTE)
        return false;
    if (knight->getType() == NORMAL && totalI < 16)
        return true;
    if (knight->getType() == LANCELOT && totalI < 19)
        return true;
    if (knight->getType() == DRAGON && totalI < 14)
        return true;
    if (knight->getType() == PALADIN)
        return true;
    return false;
}
BaseItem* BaseBag::get(ItemType itemType) {
    BaseItem *i = head;
    while (i->getType() != itemType)
        i = i->next;
    BaseItem *result = i;
    ItemType c = head->getType();
    head->setType(i->getType());
    i->setType(c);
    BaseItem *OldHead = head;
    head = head->next;
    delete OldHead;
    return result;
    return nullptr;
}
string BaseBag::toString() const {
    string s = "Bag[count=" + to_string(totalI) + ";";
    BaseItem *temp = head;
    if(temp == nullptr) return "Bag[count=0;]";
    while (temp != nullptr)
    {
        string type = (temp->type == ANTIDOTE) ? "Antidote" : (temp->type == PHOENIXDOWNI) ? "PhoenixI"
                                                                                           : (temp->type == PHOENIXDOWNII)  ? "PhoenixII"
                                                                                                                            : (temp->type == PHOENIXDOWNIII) ? "PhoenixIII"
                                                                                                                                                             : "PhoenixIV";
        s += type;
        s += temp->next != nullptr ? "," : "]";
        temp = temp->next;
    }
    return s;
}
void BaseBag::useItem(ItemType t) {
    if(head == nullptr) return;
    --totalI;
    BaseItem *thuoc = this->get(t);
    if (t == PHOENIXDOWNI) {
        thuoc = new PhoenixDownI(PHOENIXDOWNI);
    }
    else if (t == PHOENIXDOWNII) {
        thuoc = new PhoenixDownII(PHOENIXDOWNII);
    }
    else if (t == PHOENIXDOWNIII) {
        thuoc = new PhoenixDownIII(PHOENIXDOWNIII);
    }
    else if (t == PHOENIXDOWNIV) {
        thuoc = new PhoenixDownIV(PHOENIXDOWNIV);
    }
    else {
        thuoc = new Antidote(ANTIDOTE);
    }
    thuoc->use(knight);
}
bool BaseBag::hasPhoenixDown() {
    if(!totalI) return false;
    BaseItem *temp = head;
    while(temp!= nullptr) {
        if(temp->type!=ANTIDOTE) return true;
        temp = temp->next;
    }
    return false;
}
bool BaseBag::hasAntidote() {
    if(head == nullptr) return false;
    BaseItem *temp = head;
    while(temp != nullptr) {
        if(temp->type==ANTIDOTE) return true;
        temp = temp->next;
    }
    return false;
}
void BaseBag::deleteFistItem() {
    if(totalI < 3) {
        head = nullptr;
        totalI = 0; return;
    }
    BaseItem *temp = head;
    head = head->next;
    delete temp;
    totalI--;
}
void BaseBag::deleteBag() {
    while (head!= nullptr) {
        auto* temp = head;
        head = head->next;
        delete temp;
    }
}
BaseBag::~BaseBag() {
    while (head != nullptr) {
        deleteFistItem();
    }
    delete knight;
}
/* * * END implementation of class BaseBag * * */


/* * * BEGIN implementation of class BaseKnight * * */
BaseKnight* BaseKnight:: create(int id, int max_hp, int level, int gil, int antidote, int phoenixdownI) {
    auto *k = new BaseKnight();
    k->id = id;
    k->max_hp = max_hp;
    k->hp = max_hp;
    k->level = level;
    k->gil = gil;
    k->antidote = antidote;
    k->phoenixdownI = phoenixdownI;
    return k;
}
string BaseKnight::toString()
{
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    string s1 = "[Knight:id:" + to_string(id) + ",hp:" + to_string(hp) + ",maxhp:" + to_string(max_hp) + ",level:" + to_string(level) + ",gil:" + to_string(gil) + "," + bag->toString() + ",knight_type:" + typeString[type] + "]";
    return s1;
}
int BaseKnight::getMaxHP() const { return max_hp; }
int BaseKnight::getHP() const { return hp; }
bool BaseKnight::isPoisoned() const { return is_poisoned; }
int BaseKnight::getAntidote() const{ return antidote; }
BaseKnight::~BaseKnight() {
     bag->deleteBag();
     delete bag;
}
/* * * END implementation of class BaseKnight * * */


/* * * BEGIN implementation of class ArmyKnights * * */
ArmyKnights::ArmyKnights(const string &file_armyknights)
{
    PaladinShield = false; LancelotSpear = false; GuinevereHair = false; ExcaliburSword = false;
    ifstream fin(file_armyknights);
    if(!fin.is_open()) cout << "File armyknights not found\n";
    fin >> total_knights;
    auto **k = new BaseKnight *[total_knights];
    for (int i = 0; i < total_knights; i++)
    {
        int hp, level, phoenixdownI, gil, antidote;
        fin >> hp;
        fin >> level;
        fin >> phoenixdownI;
        fin >> gil;
        fin >> antidote;
        k[i] = new BaseKnight();
        k[i] = k[i]->create(i + 1, hp, level, gil, antidote, phoenixdownI);
        if (isLancelot(k[i]->getMaxHP()))
            k[i]->setType(LANCELOT);
        if (isPaladin(k[i]->getMaxHP()))
            k[i]->setType(PALADIN);
        if (isDragon(k[i]->getMaxHP()))
            k[i]->setType(DRAGON);
        k[i]->setBag(new BaseBag(k[i], phoenixdownI, antidote));
    }
    array_knights = k;
    fin.close();
}
ArmyKnights::~ArmyKnights() {
    for(int i = 0; i < total_knights; i++) {
        array_knights[i]->getBag()->deleteBag();
        delete array_knights[i];
    }
    delete[] array_knights;
}
int ArmyKnights::count() const {return total_knights;}
BaseKnight* ArmyKnights::lastKnight() const {
    return array_knights[total_knights-1];
}
BaseKnight* ArmyKnights::getKnightAt(int iD) const {
    return array_knights[iD-1];
}
bool ArmyKnights::fight(BaseOpponent *opponent) const {
    return lastKnight()->getLevel() >= opponent->getLevelO();
}
void ArmyKnights::deleteLastKnight() {
    delete array_knights[total_knights-1];
    --total_knights;
}
bool ArmyKnights::hasExcaliburSword() const {return ExcaliburSword;}
bool ArmyKnights::hasGuinevereHair() const {return GuinevereHair;}
bool ArmyKnights::hasLancelotSpear() const {return LancelotSpear;}
bool ArmyKnights::hasPaladinShield() const {return PaladinShield;}
void ArmyKnights::printInfo() const
{
    cout << "No. knights: " << this->count();
    if (this->count() > 0)
    {
        auto *lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
         << ";LancelotSpear:" << this->hasLancelotSpear()
         << ";GuinevereHair:" << this->hasGuinevereHair()
         << ";ExcaliburSword:" << this->hasExcaliburSword()
         << endl
         << string(50, '-') << endl;
}
void ArmyKnights::printResult(bool win)
{
    cout << (win ? "WIN" : "LOSE") << endl;
}
/* * * END implementation of class ArmyKnights * * */


/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure()
{
    metHades = false; metOmegaWeapon = false;
    armyKnights = nullptr;
    events = nullptr;
}
void KnightAdventure::loadArmyKnights(const std::string& filein) {
    armyKnights = new ArmyKnights(filein);

}
void KnightAdventure::loadEvents(const std::string & filein) {
    events = new Events(filein);
}
void KnightAdventure::utilizePhoenix() {
    if (armyKnights->lastKnight()->getBag()->hasPhoenixDown()) {
        //Step 1 => find phoenixdown
        BaseItem *temp = armyKnights->lastKnight()->getBag()->getHead();
        while (temp->type == ANTIDOTE) temp = temp->next;
        int phoenixType = temp->getType();
        if(phoenixType == 1) {
            auto *phoenixToUse = new PhoenixDownI(PHOENIXDOWNI);
            if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                armyKnights->lastKnight()->getBag()->useItem(temp->type);
                delete phoenixToUse;
            }
        }
        if(phoenixType == 2) {
            auto *phoenixToUse = new PhoenixDownII(PHOENIXDOWNII);
            if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                armyKnights->lastKnight()->getBag()->useItem(temp->type);
                delete phoenixToUse;
            }
        }
        if(phoenixType == 3) {
            auto *phoenixToUse = new PhoenixDownIII(PHOENIXDOWNIII);
            if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                armyKnights->lastKnight()->getBag()->useItem(temp->type);
                delete phoenixToUse;
            }
        }
        if(phoenixType == 4) {
            auto *phoenixToUse = new PhoenixDownIV(PHOENIXDOWNIV);
            if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                armyKnights->lastKnight()->getBag()->useItem(temp->type);
                delete phoenixToUse;
            }
        }
    }
        //Step 2 => call phoenix
    else if (armyKnights->lastKnight()->getHP() < 1 && armyKnights->lastKnight()->getGil() >= 100) {
        int newGil = armyKnights->lastKnight()->getGil() - 100;
        armyKnights->lastKnight()->setGil(newGil);
        armyKnights->lastKnight()->setHP(armyKnights->lastKnight()->getMaxHP() / 2);
    } else if (armyKnights->lastKnight()->getHP() < 1) armyKnights->deleteLastKnight();
    else return;
}
void KnightAdventure::findPhoenix() {
    if (armyKnights->lastKnight()->getBag()->hasPhoenixDown()) {
        //Step 1 => find phoenixdown
        bool have_use = false;
        BaseItem *temp = armyKnights->lastKnight()->getBag()->getHead();
        while (temp->type == PHOENIXDOWNIV) temp = temp->next;
        if(!have_use) {
            int phoenixType = temp->getType();
            if (phoenixType == 1) {
                auto *phoenixToUse = new PhoenixDownI(PHOENIXDOWNI);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 2) {
                auto *phoenixToUse = new PhoenixDownII(PHOENIXDOWNII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 3) {
                auto *phoenixToUse = new PhoenixDownIII(PHOENIXDOWNIII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 4) {
                auto *phoenixToUse = new PhoenixDownIV(PHOENIXDOWNIV);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
        }
        while (temp->type == PHOENIXDOWNIII) temp = temp->next;
        if(!have_use) {
            int phoenixType = temp->getType();
            if (phoenixType == 1) {
                auto *phoenixToUse = new PhoenixDownI(PHOENIXDOWNI);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 2) {
                auto *phoenixToUse = new PhoenixDownII(PHOENIXDOWNII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 3) {
                auto *phoenixToUse = new PhoenixDownIII(PHOENIXDOWNIII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 4) {
                auto *phoenixToUse = new PhoenixDownIV(PHOENIXDOWNIV);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
        }
        while (temp->type == PHOENIXDOWNII) temp = temp->next;
        if(!have_use) {
            int phoenixType = temp->getType();
            if (phoenixType == 1) {
                auto *phoenixToUse = new PhoenixDownI(PHOENIXDOWNI);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 2) {
                auto *phoenixToUse = new PhoenixDownII(PHOENIXDOWNII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 3) {
                auto *phoenixToUse = new PhoenixDownIII(PHOENIXDOWNIII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 4) {
                auto *phoenixToUse = new PhoenixDownIV(PHOENIXDOWNIV);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
        }
        while (temp->type == PHOENIXDOWNI) temp = temp->next;
        if(!have_use) {
            int phoenixType = temp->getType();
            if (phoenixType == 1) {
                auto *phoenixToUse = new PhoenixDownI(PHOENIXDOWNI);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 2) {
                auto *phoenixToUse = new PhoenixDownII(PHOENIXDOWNII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 3) {
                auto *phoenixToUse = new PhoenixDownIII(PHOENIXDOWNIII);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
            if (phoenixType == 4) {
                auto *phoenixToUse = new PhoenixDownIV(PHOENIXDOWNIV);
                if (phoenixToUse->canUse(armyKnights->lastKnight())) {
                    armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    have_use = true;
                    delete phoenixToUse;
                }
            }
        }
    }
}
bool KnightAdventure::fightUltimecia() {
//    cout << "Have 3 item\n";
    double UltimeciaHP = 5000;
    // combat
    int idToFight = armyKnights->count();
    int totalKnight = armyKnights->count();
    double knightBaseDamage[3] = {0.06, 0.05, 0.075};
    // Each knight attack Ultimecia
    for(int i = 0; i < totalKnight; i++) {
        if(armyKnights->getKnightAt(idToFight)->getType() != NORMAL) {
            KnightType t = armyKnights->getKnightAt(idToFight)->getType();
            int damage = armyKnights->getKnightAt(idToFight)->getHP()*armyKnights->getKnightAt(idToFight)->getLevel()*knightBaseDamage[t];
            UltimeciaHP-=damage;
        }
        if(UltimeciaHP > 0) armyKnights->deleteLastKnight();
        else break;
        idToFight--;
    }
    return UltimeciaHP<1;
}
void KnightAdventure::pushGilToArmy(int x) {
    int gilToPush = x;
    int idToPush = armyKnights->count();
    while(gilToPush>0) {
        int newGilOfKnight = armyKnights->getKnightAt(idToPush)->getGil() + gilToPush;
        if(newGilOfKnight > 999) {
            gilToPush = newGilOfKnight-999;
            newGilOfKnight = 999;
        }
        else {
            gilToPush = 0;
        }
        armyKnights->getKnightAt(idToPush)->setGil(newGilOfKnight);
        --idToPush;
        if(!idToPush) break;
    }
}
void KnightAdventure::pushItemToArmy(BaseItem *i) {
    int idToPush = armyKnights->count();
//    cout << "Item type: " << i->getType();
//    cout << "Call to push item\n";
    int totalKnight = armyKnights->count();
    bool added = false;
    for(int j = 0; j < totalKnight; ++j) {
        if(armyKnights->getKnightAt(idToPush)->getBag()->canBeAdd(i->type)) {
            armyKnights->getKnightAt(idToPush)->getBag()->insertFirst(i);
            added = true;
        }
        if (added) break;
        --idToPush;
    }
}
void KnightAdventure::run() {
    for (int i = 0; i < events->count(); i++) {
//        cout << "Event: " << events->get(i) << endl;
        if (events->get(i) < 6) {
            BaseOpponent *gau = nullptr;
            if (events->get(i) == 1)
                gau = new MadBear(i, events->get(i));
            if (events->get(i) == 2)
                gau = new Bandit(i, events->get(i));
            if (events->get(i) == 3)
                gau = new LordLupin(i, events->get(i));
            if (events->get(i) == 4)
                gau = new Elf(i, events->get(i));
            if (events->get(i) == 5)
                gau = new Troll(i, events->get(i));
            if (armyKnights->fight(gau) || armyKnights->lastKnight()->getType()==LANCELOT || armyKnights->lastKnight()->getType()==PALADIN) {
                int newGil = armyKnights->lastKnight()->getGil();
                newGil += gau->gilO();
                newGil = newGil > 999 ? 999 : newGil;
                armyKnights->lastKnight()->setGil(newGil);
//                cout << "Win doi thu co dame: " << gau->baseDamageO() << endl;
            }
            else {
                int newHP = armyKnights->lastKnight()->getHP();
                int opponentDamage =  gau->baseDamageO() * (gau->getLevelO() - armyKnights->lastKnight()->getLevel());
                newHP-=opponentDamage;
//                cout << "Lose doi thu co dame: " << opponentDamage;
                armyKnights->lastKnight()->setHP(newHP);
//                cout << " \t newHP: " << newHP << endl;
                if (newHP <= 0) {
                    this->utilizePhoenix();
                }
                else this->findPhoenix();

            }
        }
        else if (events->get(i) == 6) {
            auto *Tornbery = new BaseOpponent(i, events->get(i));
            if (armyKnights->fight(Tornbery)) {
                int newLevel = armyKnights->lastKnight()->getLevel();
                newLevel++;
                newLevel = newLevel > 10 ? 10 : newLevel;
                armyKnights->lastKnight()->setLevel(newLevel);
            }
            else {
//                cout << "Lose Tornbery\n";
                if (armyKnights->lastKnight()->getType() == DRAGON) {
//                    cout << "After event 6: ";
                    armyKnights->printInfo();
                    continue;
                }
//                cout << "Lose Tornbery but is not Dragon\n";
                armyKnights->lastKnight()->setIsPoisoned(true);
                if (armyKnights->lastKnight()->getBag()->hasAntidote()) {
                    armyKnights->lastKnight()->getBag()->useItem(ANTIDOTE);
                }
                else {
                    //Trung doc
//                    cout << "Trung doc\n";
                    for (int j = 0; j < 3; j++) {
                        armyKnights->lastKnight()->getBag()->deleteFistItem();
                    }
                    int newHP = armyKnights->lastKnight()->getHP();
                    newHP -= 10;
                    armyKnights->lastKnight()->setHP(newHP);
                    if (newHP <= 0) {
                        this->utilizePhoenix();
                    }
                    else this->findPhoenix();

                }
            }
            delete Tornbery;
        }
        else if (events->get(i) == 7) {
            auto *Queen = new BaseOpponent(i,events->get(i));
            if(armyKnights->fight(Queen)) {
                int newGilOfLastKnight = armyKnights->lastKnight()->getGil()*2;
                if(newGilOfLastKnight>999) {
                    int gillToPush = newGilOfLastKnight-999;
                    newGilOfLastKnight = 999;
                    this->pushGilToArmy(gillToPush);
                }
                armyKnights->lastKnight()->setGil(newGilOfLastKnight);
            }
            else {
                if (armyKnights->lastKnight()->getType() != PALADIN) {
                    int newGilOfLastKnight = armyKnights->lastKnight()->getGil() / 2;
                    armyKnights->lastKnight()->setGil(newGilOfLastKnight);
                }
            }
            delete Queen;
        }
        else if (events->get(i) == 8) {
            if(armyKnights->lastKnight()->getType()==PALADIN) {
                int plusHP = armyKnights->lastKnight()->getHP() < armyKnights->lastKnight()->getMaxHP()/3 ? armyKnights->lastKnight()->getMaxHP()/5 : 0;
                int newHP = armyKnights->lastKnight()->getHP() + plusHP;
                armyKnights->lastKnight()->setHP(newHP);
            }
            else if(armyKnights->lastKnight()->getGil() < 50) {
                armyKnights->printInfo();
                continue;
            }
            else {
                if (armyKnights->lastKnight()->getHP() < armyKnights->lastKnight()->getMaxHP() / 3) {
                    int newGil = armyKnights->lastKnight()->getGil() - 50;
                    armyKnights->lastKnight()->setGil(newGil);
                    int newHP = armyKnights->lastKnight()->getHP() + armyKnights->lastKnight()->getMaxHP() / 5;
                    armyKnights->lastKnight()->setHP(newHP);
                }
            }
        }
        else if (events->get(i) == 9) {
            armyKnights->lastKnight()->setHP(armyKnights->lastKnight()->getMaxHP());
        }
        else if (events->get(i) == 10) {
            if(this->metOmega()) {armyKnights->printInfo(); continue;}
            if((armyKnights->lastKnight()->getLevel() == 10 && armyKnights->lastKnight()->getHP()==armyKnights->lastKnight()->getMaxHP())||(armyKnights->lastKnight()->getType()==DRAGON)) {
                armyKnights->lastKnight()->setLevel(10);
                armyKnights->lastKnight()->setGil(999);
                setMetOmega(true);
            }
            else {
                armyKnights->lastKnight()->setHP(0);
                this->utilizePhoenix();
            }
        }
        else if (events->get(i) == 11) {
            if(this->met_Hades()) {armyKnights->printInfo(); continue;}
            if((armyKnights->lastKnight()->getLevel() == 10) || (armyKnights->lastKnight()->getType() == PALADIN && armyKnights->lastKnight()->getLevel() >= 8)) {
                armyKnights->setPaladinShield(true);
                // Have met Hades update
                setMetHades(true);
            } else {
                armyKnights->lastKnight()->setHP(0);
                this->utilizePhoenix();
            }
        }
        else if (events->get(i) == 99) {
            bool winUltimecia = armyKnights->hasExcaliburSword();
            bool has3Item = armyKnights->hasGuinevereHair() && armyKnights->hasPaladinShield() &&
                            armyKnights->hasLancelotSpear();
            if (winUltimecia) {
                armyKnights->printInfo();
                continue;
            }
            if (!has3Item) {
                // Lose cus dont have enough item => delete all knight
                int totalKnight = armyKnights->count();
                for (int j = 0; j < totalKnight; ++j) {
                    armyKnights->deleteLastKnight();
                }
            } else bool loseUtimecia = this->fightUltimecia();
        }
        else if (events->get(i) == 112) {
            auto * drug = new PhoenixDownII(PHOENIXDOWNII);
            this->pushItemToArmy(drug);
        }
        else if (events->get(i) == 113) {
            auto * drug = new PhoenixDownIII(PHOENIXDOWNIII);
            this->pushItemToArmy(drug);
        }
        else if (events->get(i) == 114) {
            auto * drug = new PhoenixDownIV(PHOENIXDOWNIV);
            this->pushItemToArmy(drug);
        }
        else if (events->get(i) == 95) {
            armyKnights->setPaladinShield(true);
        }
        else if (events->get(i) == 96) {
            armyKnights->setLancelotSpear(true);
        }
        else if (events->get(i) == 97) {
            armyKnights->setGuinevereHair(true);
        }
        else if (events->get(i) == 98) {
            bool has3Item = armyKnights->hasGuinevereHair() && armyKnights->hasPaladinShield() && armyKnights->hasLancelotSpear();
            armyKnights->setExcaliburSword(has3Item);
        }
        armyKnights->printInfo();
        // If there is no knight -> break loop to declare lose!
        if(!armyKnights->count()) break;
    }
//    cout << "Number of knight after all: " << armyKnights->count() << endl;
    bool win = armyKnights->count();
    armyKnights->printResult(win);
}
KnightAdventure::~KnightAdventure() {
    delete  armyKnights;
    delete  events;
}
/* * * END implementation of class KnightAdventure * * */
// Function
bool isPrime(int n) {
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    for (int i = 2; i < n; i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}
bool isLancelot(int maxhp){
    return maxhp == 888;
}
bool isPaladin(int maxhp){
    return isPrime(maxhp);
}
bool isDragon(int maxhp)
{
    if (maxhp < 100 || maxhp > 999)
        return false;
    int a = maxhp % 10;
    int b = maxhp % 100 / 10;
    int c = maxhp / 100;
    if (a == 0 || b == 0 || c == 0)
        return false;
    if (a * a == b * b + c * c || b * b == a * a + c * c || c * c == b * b + a * a)
        return true;
    else
        return false;
}
// Fix all extra tc in BKel
// First tc44 fixed
