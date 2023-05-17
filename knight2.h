#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG
class BaseBag;
class Events
{
private:
    int total_events;
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
    void setLevel(int x) {level = x;}
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
    BaseBag* getBag() const {return bag;}
    void setBag (BaseBag *x) {bag = x;}
};
class ArmyKnights
{
private:
    int total_knights;
    BaseKnight **array_knights;
    bool PaladinShield, LancelotSpear, GuinevereHair,ExcaliburSword;
public:
    explicit ArmyKnights(const string &file_armyknights);
    ~ArmyKnights();
    int count() const;
    bool fight(BaseOpponent *opponent) const;
    BaseKnight *lastKnight() const;
    BaseKnight *getKnightAt(int iD) const;
    void deleteLastKnight();
    bool hasPaladinShield() const;
    bool hasLancelotSpear() const;
    bool hasGuinevereHair() const;
    bool hasExcaliburSword() const;
    void setPaladinShield(bool x) { PaladinShield = x;}
    void setLancelotSpear(bool x) {LancelotSpear = x;}
    void setGuinevereHair(bool x) {GuinevereHair = x;}
    void setExcaliburSword(bool x) {ExcaliburSword = x;}
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
    virtual ~BaseItem()= default;
};
class Antidote : public BaseItem
{
public:
    explicit Antidote(ItemType t);
    ItemType getType() override;
    void setType(ItemType x) override;
    bool canUse(BaseKnight *knight) override;
    void use(BaseKnight *knight) override;
};
class PhoenixDownI : public BaseItem
{
public:
    explicit PhoenixDownI(ItemType t);
    ItemType getType() override;
    void setType(ItemType x) override;
    bool canUse(BaseKnight *knight) override;
    void use(BaseKnight *knight) override;
};
class PhoenixDownII : public BaseItem
{
public:
    explicit PhoenixDownII(ItemType t);
    ItemType getType() override;
    void setType(ItemType x) override;
    bool canUse(BaseKnight *knight) override;
    void use(BaseKnight *knight) override;
};
class PhoenixDownIII : public BaseItem
{
public:
    explicit PhoenixDownIII(ItemType t);
    ItemType getType() override;
    void setType(ItemType x) override;
    bool canUse(BaseKnight *knight) override;
    void use(BaseKnight *knight) override;
};
class PhoenixDownIV : public BaseItem
{
public:
    explicit PhoenixDownIV(ItemType t);
    ItemType getType() override;
    void setType(ItemType x) override;
    bool canUse(BaseKnight *knight) override;
    void use(BaseKnight *knight) override;
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
    virtual BaseItem *get(ItemType itemType);
    virtual string toString() const;
    void useItem(ItemType t);
    bool hasPhoenixDown();
    bool hasAntidote();
    void deleteFistItem();
    ~BaseBag();
};

class KnightAdventure
{
private:
    ArmyKnights *armyKnights;
    Events *events;
    bool metOmegaWeapon;
    bool metHades;
public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string& filein);
    void loadEvents(const string & filein);
    void pushGilToArmy(int x);
    void setMetOmega(bool x) {metOmegaWeapon = x;}
    bool metOmega() const {return metOmegaWeapon;}
    void setMetHades(bool x) {metHades = x;}
    bool met_Hades() const {return metHades;}
    void utilizePhoenix();
    void findPhoenix();
    bool fightUltimecia();
    void pushItemToArmy(BaseItem* i);
    void run();
};
// Function
bool isPrime(int n);
bool isLancelot(int max_hp);
bool isPaladin(int max_hp);
bool isDragon(int max_hp);
#endif // __KNIGHT2_H__