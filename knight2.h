#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG
class BaseBag;
class Events
{
private:
    int total_events{};
    int *data;

public:
    explicit Events(const string& file_events);
    int count() const;
    int get(int i) const;
    ~Events();
};

class BaseOpponent
{
protected:
    int baseDamage;
    int gil;
    int levelO;

public:
    BaseOpponent(int i, int event_id);;
    int gilO() const { return gil; }
    int baseDamageO() const { return baseDamage; }
    int getLevelO() const {return levelO;}
};
class MadBear : public BaseOpponent
{
public:
    MadBear(int i, int event_id) : BaseOpponent(i, event_id)
    {
        gil = 100;
        baseDamage = 10;
    }
};
class Bandit : public BaseOpponent
{
public:
    Bandit(int i, int event_id) : BaseOpponent(i, event_id)
    {
        gil = 150;
        baseDamage = 15;
    }
};
class LordLupin : public BaseOpponent
{
public:
    LordLupin(int i, int event_id) : BaseOpponent(i, event_id)
    {
        gil = 450;
        baseDamage = 45;
    }
};
class Elf : public BaseOpponent
{
public:
    Elf(int i, int event_id) : BaseOpponent(i, event_id)
    {
        gil = 750;
        baseDamage = 75;
    }
};
class Troll : public BaseOpponent
{
public:
    Troll(int i, int event_id) : BaseOpponent(i, event_id)
    {
        gil = 800;
        baseDamage = 95;
    }
};

enum KnightType
{
    PALADIN = 0,
    LANCELOT,
    DRAGON,
    NORMAL
};
class BaseKnight
{
protected:
    int id;
    int hp;
    int max_hp;
    int level;
    int gil;
    int antidote;
    int phoenixdownI;
    bool is_poisoned;
    KnightType type;
    BaseBag *bag;

public:
    BaseKnight() : id(0), hp(0), max_hp(0), level(0), gil(0), antidote(0), phoenixdownI(0), is_poisoned(false), type(NORMAL), bag(nullptr) {}
    static BaseKnight *create(int id, int max_hp, int level, int gil, int antidote, int phoenixdownI);
    string toString() ;
    int getMaxHP() const ;
    int getHP() const;
    void setHP(int x) {hp = x;}
    void setGil(int x) {gil = x;}
    int getGil() const{return gil;}
    KnightType getType() const { return type; }
    int getLevel() const {return level;}
    void setType(KnightType x) { type = x; }
    bool isPoisoned() const;
    int getAntidote() const;
    void setAntidote(int x) { antidote = x; }
    void setIsPoisoned(bool x) { is_poisoned = x;}
    int getPhoenixDownI() const;
    void setPhoenixDownI(int x) { phoenixdownI = x; }
    BaseBag* getBag() const {return bag;}
    void setBag (BaseBag *x) {bag = x;}
};
class Lancelot : public BaseKnight
{
public:
    static Lancelot *create(int id, int max_hp, int level, int gil, int antidote, int phoenixdownI)
    {
        auto *k = new Lancelot();
        k->id = id;
        k->max_hp = max_hp;
        k->hp = max_hp;
        k->level = level;
        k->gil = gil;
        k->antidote = antidote;
        k->phoenixdownI = phoenixdownI;
        k->is_poisoned = false;
        k->type = LANCELOT;
        return k;
    };
};
class ArmyKnights
{
private:
    int total_knights{};
    BaseKnight **array_knights;

public:
    explicit ArmyKnights(const string &file_armyknights);
    ~ArmyKnights();
    int count() const;
    bool fight(BaseOpponent *opponent) const;
    bool adventure(Events *events);
    BaseKnight *lastKnight() const;
    BaseKnight *getKnightAt(int iD) const;
    void deleteLastKnight();
    bool hasPaladinShield() const;
    bool hasLancelotSpear() const;
    bool hasGuinevereHair() const;
    bool hasExcaliburSword() const;
    BaseKnight **getArrayKnight() const {return array_knights;}
    void printInfo() const;
    static void printResult(bool win) ;
};
enum ItemType
{
    ANTIDOTE,
    PHOENIXDOWNI,
    PHOENIXDOWNII,
    PHOENIXDOWNIII,
    PHOENIXDOWNIV
};
class BaseItem
{
public:
    ItemType type;
    BaseItem *next;
    explicit BaseItem(ItemType t) { type = t; next = nullptr; }
    virtual ItemType getType() { return type; }
    virtual void setType(ItemType x) { type = x; }
    virtual bool canUse(BaseKnight *knight) = 0;
    virtual void use(BaseKnight *knight) = 0;
    virtual void getInfo()
    {
        cout << "\n----------------item info--------------\n";
        cout << "Type: " << type << endl;
        cout << "-----------------------------------------\n";
    }
    virtual ~BaseItem(){};
};
class Antidote : public BaseItem
{
public:
    explicit Antidote(ItemType t);
    ItemType getType()  { return type; }
    void setType(ItemType x)  { type = x; }
    bool canUse(BaseKnight *knight)  { return knight->isPoisoned(); }
    void use(BaseKnight *knight);
    void getInfo()
    {
        cout << "\n----------------item info--------------\n";
        cout << "Type: " << type << endl;
        cout << "-----------------------------------------\n";
    }
};
class PhoenixDownI : public BaseItem
{
public:
    explicit PhoenixDownI(ItemType t);
    ItemType getType()  { return type; }
    void setType(ItemType x)  { type = x; }
    bool canUse(BaseKnight *knight)  { return knight->getHP() < 0; }
    void use(BaseKnight *knight);
    void getInfo()
    {
        cout << "\n----------------item info--------------\n";
        cout << "Type: " << type << endl;
        cout << "-----------------------------------------\n";
    }
};
class PhoenixDownII : public BaseItem
{
public:
    explicit PhoenixDownII(ItemType t);
    ItemType getType()  { return type; }
    void setType(ItemType x)  { type = x; }
    bool canUse(BaseKnight *knight);
    void use(BaseKnight *knight);
    void getInfo()
    {
        cout << "\n----------------item info--------------\n";
        cout << "Type: " << type << endl;
        cout << "-----------------------------------------\n";
    }
};
class PhoenixDownIII : public BaseItem
{
public:
    explicit PhoenixDownIII(ItemType t);
    ItemType getType()  { return type; }
    void setType(ItemType x)  { type = x; }
    bool canUse(BaseKnight *knight);
    void use(BaseKnight *knight);
    void getInfo()
    {
        cout << "\n----------------item info--------------\n";
        cout << "Type: " << type << endl;
        cout << "-----------------------------------------\n";
    }
};
class PhoenixDownIV : public BaseItem
{
public:
    explicit PhoenixDownIV(ItemType t);
    ItemType getType()  { return type; }
    void setType(ItemType x)  { type = x; }
    bool canUse(BaseKnight *knight)  { return knight->getHP() < (knight->getMaxHP()/2); }
    void use(BaseKnight *knight);
    void getInfo()
    {
        cout << "\n----------------item info--------------\n";
        cout << "Type: " << type << endl;
        cout << "-----------------------------------------\n";
    }
};
class BaseBag
{
private:
    BaseItem *head;
    BaseKnight *knight;
    int totalI;
public:
    BaseBag(BaseKnight *k, int phoenixdownI, int antidote);
    BaseItem *getHead() { return head; }
    virtual bool insertFirst(BaseItem *item);
    bool canBeAdd(ItemType t);
    int getToTalItem() const {return totalI;}
    virtual BaseItem *get(ItemType itemType);
    virtual string toString() const;
    void useItem(ItemType t);
    bool hasPhoenixDown();
    bool hasAntidote();
    void deleteFistItem();
};

class KnightAdventure
{
private:
    ArmyKnights *armyKnights;
    Events *events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string& filein);
    void loadEvents(const string & filein);
    void pushGilToArmy(int x);
    ArmyKnights* getArmy() const {return armyKnights;}
    void run();
};
// Function
bool isPrime(int n);
bool isLancelot(int max_hp);
bool isPaladin(int max_hp);
bool isDragon(int max_hp);
#endif // __KNIGHT2_H__