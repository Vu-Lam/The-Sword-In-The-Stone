#include "knight2.h"
/* * * BEGIN implementation of class Events * * */
Events::Events(const string& file_events)
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


/* * * BEGIN implementation of class BaseOpponent * * */
BaseOpponent::BaseOpponent(int i, int event_id) { levelO = (i + event_id) % 10 + 1; gil = 0; baseDamage=0;}

/* * * END implementation of class BaseOpponent * * */


/* * * BEGIN implementation of class Item * * */
Antidote::Antidote(ItemType t) : BaseItem(t) { type = ANTIDOTE; }
void Antidote::use(BaseKnight *knight) { knight->setIsPoisoned(false); knight->setAntidote(knight->getAntidote()-1);}
PhoenixDownI::PhoenixDownI(ItemType t) : BaseItem(t){ type = PHOENIXDOWNI;}
void PhoenixDownI::use(BaseKnight *knight) { knight->setHP(knight->getMaxHP()); }
PhoenixDownII::PhoenixDownII(ItemType t) : BaseItem(t) {type = PHOENIXDOWNII;}
bool PhoenixDownII::canUse(BaseKnight *knight) { return knight->getHP() < (knight->getMaxHP()/4); }
void PhoenixDownII::use(BaseKnight *knight) { knight->setHP(knight->getMaxHP()); }
PhoenixDownIII::PhoenixDownIII(ItemType t) : BaseItem(t){type = PHOENIXDOWNIII;}
void PhoenixDownIII::use(BaseKnight *knight) {
    if(knight->getHP()<1) knight->setHP(knight->getMaxHP()/3);
    else {
        int newHP = knight->getHP() + knight->getMaxHP()/4;
        knight->setHP(newHP);
    }
}
bool PhoenixDownIII::canUse(BaseKnight *knight) { return knight->getHP() < (knight->getMaxHP()/3); }
PhoenixDownIV::PhoenixDownIV(ItemType t) : BaseItem(t){type = PHOENIXDOWNIV;}
void PhoenixDownIV::use(BaseKnight *knight) {
    if(knight->getHP()<1) knight->setHP(knight->getMaxHP()/2);
    else {
        int newHP = knight->getHP() + knight->getMaxHP()/5;
        knight->setHP(newHP);
    }
}
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
    if (antidote > 0)
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
                BaseItem *j = head;
                while (j->next != nullptr)
                    j = j->next;
                j->next = temp;
                j->next->setType(ANTIDOTE);
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
//    cout << "Address found: \t" << i << "\tData found: " << i->type << endl;
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
    if(temp == nullptr) return "Bag[count=0]";
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
    BaseItem *temp = this->get(t);
    BaseItem *thuoc;
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
    if(head == nullptr) return false;
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
        head = nullptr; return;
    }
    BaseItem *temp = head;
    head = head->next;
    delete temp;
    totalI--;
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
    string s1 = "[Knight:id:" + to_string(id) + ",hp:" + to_string(hp) + ",maxhp:" + to_string(max_hp) + ",level:" + to_string(level) + ",gil:" + to_string(gil) + "," + bag->toString() + ",knight_type:" + typeString[type] + "]\n";
    return s1;
}
int BaseKnight::getMaxHP() const { return max_hp; }
int BaseKnight::getHP() const { return hp; }
bool BaseKnight::isPoisoned() const { return is_poisoned; }
int BaseKnight::getAntidote() const{ return antidote; }
int BaseKnight::getPhoenixDownI() const {return phoenixdownI;}
/* * * END implementation of class BaseKnight * * */


/* * * BEGIN implementation of class ArmyKnights * * */
ArmyKnights::ArmyKnights(const string &file_armyknights)
{
    fstream fin(file_armyknights);
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
}
ArmyKnights::~ArmyKnights() {
    for(int i = 0; i < total_knights; i++) delete array_knights[i];
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
    return lastKnight()->getLevel() > opponent->getLevelO();
}
void ArmyKnights::deleteLastKnight() {
    delete array_knights[total_knights-1];
    --total_knights;
}
bool ArmyKnights::hasExcaliburSword() const {return false;}
bool ArmyKnights::hasGuinevereHair() const {return false;}
bool ArmyKnights::hasLancelotSpear() const {return false;}
bool ArmyKnights::hasPaladinShield() const {return false;}
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
    armyKnights = nullptr;
    events = nullptr;
}
void KnightAdventure::loadArmyKnights(const std::string& filein) {
    armyKnights = new ArmyKnights(filein);
}
void KnightAdventure::loadEvents(const std::string & filein) {
    events = new Events(filein);
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
    }
}
void KnightAdventure::run() {
    for (int i = 0; i < events->count(); i++) {
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
            if (armyKnights->fight(gau) || armyKnights->lastKnight()->getType()==LANCELOT) {
                int newGil = armyKnights->lastKnight()->getGil();
                newGil += gau->gilO();
                newGil = newGil > 999 ? 999 : newGil;
                armyKnights->lastKnight()->setGil(newGil);
//                cout << "Win doi thu co dame: " << gau->baseDamageO() << endl;
            }
            else {
//                cout << "Lose doi thu co dame: " << gau->baseDamageO();
                int newHP = armyKnights->lastKnight()->getHP();
                newHP -= gau->baseDamageO() * (gau->getLevelO() - armyKnights->lastKnight()->getLevel());
//                cout << " \t newHP: " << newHP << endl;
                if (newHP > 0) armyKnights->lastKnight()->setHP(newHP);
                if (newHP <= 0) {
                    if (armyKnights->lastKnight()->getBag()->hasPhoenixDown()) {                //Step 1 => find phoenixdown
//                        cout << "Call step 1\n";
                        BaseItem *temp = armyKnights->lastKnight()->getBag()->getHead();
                        while (temp->type == ANTIDOTE) temp = temp->next;
                        if (temp->canUse(armyKnights->lastKnight()))
                            armyKnights->lastKnight()->getBag()->useItem(temp->type);
                    }
                        //Step 2 => call phoenix
                    else if (armyKnights->lastKnight()->getGil() >= 100) {
                        int newGil = armyKnights->lastKnight()->getGil() - 100;
                        armyKnights->lastKnight()->setGil(newGil);
                        armyKnights->lastKnight()->setHP(armyKnights->lastKnight()->getMaxHP() / 2);
                    } else armyKnights->deleteLastKnight();
                }

            }
            armyKnights->printInfo();
        }
        else if (events->get(i) == 6) {
            auto *Tornbery = new BaseOpponent(i, events->get(i));
            if (armyKnights->fight(Tornbery)) {
                int newLevel = armyKnights->lastKnight()->getLevel();
                newLevel++;
                newLevel = newLevel > 10 ? 10 : newLevel;
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
                    for (int j = 0; j < 3; j++) {
                        armyKnights->lastKnight()->getBag()->deleteFistItem();
                    }
                    int newHP = armyKnights->lastKnight()->getHP();
                    newHP -= 10;
                    if (newHP > 0) armyKnights->lastKnight()->setHP(newHP);
                    if (newHP <= 0) {
                        if (armyKnights->lastKnight()->getBag()->hasPhoenixDown()) {
                            //Step 1 => find phoenixdown
                            BaseItem *temp = armyKnights->lastKnight()->getBag()->getHead();
                            while (temp->type == ANTIDOTE) temp = temp->next;
                            if (temp->canUse(armyKnights->lastKnight()))
                                armyKnights->lastKnight()->getBag()->useItem(temp->type);
                        }
                            //Step 2 => call phoenix
                        else if (armyKnights->lastKnight()->getGil() >= 100) {
                            int newGil = armyKnights->lastKnight()->getGil() - 100;
                            armyKnights->lastKnight()->setGil(newGil);
                            armyKnights->lastKnight()->setHP(armyKnights->lastKnight()->getMaxHP() / 2);
                        } else armyKnights->deleteLastKnight();
                    }

                }
            }
            armyKnights->printInfo();
        }
        else if (events->get(i) == 7) {
            auto *Queen = new BaseOpponent(i,events->get(i));
            if(armyKnights->fight(Queen)) {
                int newGilOfLastKnight = armyKnights->lastKnight()->getGil()*2;
                if(newGilOfLastKnight>999) {
                    int gillToPush = newGilOfLastKnight-999;
                    newGilOfLastKnight = 999;
                    pushGilToArmy(gillToPush);
                }
                armyKnights->lastKnight()->setGil(newGilOfLastKnight);
            }
            else {
                int newGilOfLastKnight = armyKnights->lastKnight()->getGil() / 2 ;
                armyKnights->lastKnight()->setGil(newGilOfLastKnight);
            }
        }
    }
}
KnightAdventure::~KnightAdventure() {
    delete [] armyKnights;
    delete [] events;
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
bool isDragon(int maxhp){
    if (maxhp < 100 || maxhp > 999)
        return false;
    int a = maxhp % 10;
    int b = maxhp % 100 / 10;
    int c = maxhp / 100;
    if (a * a == b * b + c * c || b * b == a * a + c * c || c * c == b * b + a * a)
        return true;
    else
        return false;
}

