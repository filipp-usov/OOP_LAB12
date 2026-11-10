#pragma once
#include <string>
#include "Attributes.h"
#include "Observer.h"

class Unit : public IObservable {
protected:
    int id;
    int x, y;
    Health health;
    Armor armor;
    Attack attack;

public:
    Unit(int id, int x, int y, int hp, int arm, int atk);
    virtual ~Unit() = default;

    int getId() const { return id; }
    int getX() const { return x; }
    int getY() const { return y; }
    Health& getHealth() { return health; } 
    const Health& getHealth() const { return health; }
    
    void move(int newX, int newY) { x = newX; y = newY; }

    void takeDamage(int dmg) {
        int finalDmg = armor.reduceDamage(dmg); 
        health.decrease(finalDmg);
        notify({EventType::Damaged, id, "Unit damaged"});
        if (health.isZero()) {
            notify({EventType::Destroyed, id, "Unit destroyed"});
        }
    }

    virtual std::string getType() const = 0;
    virtual Unit* clone() const = 0;
};


class Infantry : public Unit {
public:
    Infantry(int id, int x, int y, int hp, int arm, int atk) : Unit(id, x, y, hp, arm, atk) {}
};

class Swordsman : public Infantry {
public:
    Swordsman(int id, int x, int y) : Infantry(id, x, y, 100, 10, 20) {}
    std::string getType() const override { return "Swordsman"; }
    Unit* clone() const override { return new Swordsman(*this); }
};

class Spearman : public Infantry {
public:
    Spearman(int id, int x, int y) : Infantry(id, x, y, 80, 15, 15) {}
    std::string getType() const override { return "Spearman"; }
    Unit* clone() const override { return new Spearman(*this); }
};


class Archer : public Unit {
public:
    Archer(int id, int x, int y, int hp, int arm, int atk) : Unit(id, x, y, hp, arm, atk) {}
};

class LightArcher : public Archer {
public:
    LightArcher(int id, int x, int y) : Archer(id, x, y, 60, 5, 25) {}
    std::string getType() const override { return "LightArcher"; }
    Unit* clone() const override { return new LightArcher(*this); }
};

class HeavyArcher : public Archer {
public:
    HeavyArcher(int id, int x, int y) : Archer(id, x, y, 90, 10, 20) {}
    std::string getType() const override { return "HeavyArcher"; }
    Unit* clone() const override { return new HeavyArcher(*this); }
};


class Cavalry : public Unit {
public:
    Cavalry(int id, int x, int y, int hp, int arm, int atk) : Unit(id, x, y, hp, arm, atk) {}
};

class Cataphract : public Cavalry {
public:
    Cataphract(int id, int x, int y) : Cavalry(id, x, y, 120, 20, 30) {}
    std::string getType() const override { return "Cataphract"; }
    Unit* clone() const override { return new Cataphract(*this); }
};

class HorseArcher : public Cavalry {
public:
    HorseArcher(int id, int x, int y) : Cavalry(id, x, y, 80, 10, 25) {}
    std::string getType() const override { return "HorseArcher"; }
    Unit* clone() const override { return new HorseArcher(*this); }
};