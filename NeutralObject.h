#pragma once
#include "Unit.h"
#include <iostream>
#include <memory>

class IInteractionStrategy {
public:
    virtual ~IInteractionStrategy() = default;
    virtual void apply(Unit& unit) = 0;
};

class HealthRestoreStrategy : public IInteractionStrategy {
public:
    void apply(Unit& unit) override {
        std::cout << "  -> Strategy applied: Health Restore (+50 HP)!\n";
        unit.getHealth().increase(50); 
    }
};

class AttackBoostStrategy : public IInteractionStrategy {
public:
    void apply(Unit& unit) override {
        std::cout << "  -> Strategy applied: Attack Boost! (Restoring 20 HP as proxy)\n";
        unit.getHealth().increase(20); 
    }
};

class NeutralObject {
protected:
    std::shared_ptr<IInteractionStrategy> strategy;
public:
    NeutralObject(std::shared_ptr<IInteractionStrategy> strat) : strategy(strat) {}
    virtual ~NeutralObject() = default;
    virtual void interact(Unit& unit) = 0;
};

class TreasureChest : public NeutralObject {
public:
    TreasureChest() : NeutralObject(std::make_shared<HealthRestoreStrategy>()) {}
    void interact(Unit& unit) override {
        std::cout << "Chest: Unit found resources.\n";
        strategy->apply(unit);
    }
};

class Trap : public NeutralObject {
public:
    Trap() : NeutralObject(nullptr) {}
    void interact(Unit& unit) override {
        std::cout << "Trap: Unit takes damage!\n";
        unit.takeDamage(20);
    }
};

class Altar : public NeutralObject {
public:
    Altar() : NeutralObject(nullptr) {} 
    void interact(Unit& unit) override {
        std::cout << "Altar: Unit prays. ";
        if (unit.getType() == "Swordsman" || unit.getType() == "Spearman") {
            std::cout << "(Warrior buff)\n";
            std::make_shared<AttackBoostStrategy>()->apply(unit);
        } else {
            std::cout << "(Mage/Archer buff)\n";
            std::make_shared<HealthRestoreStrategy>()->apply(unit);
        }
    }
};

class EnergySource : public NeutralObject {
public:
    EnergySource() : NeutralObject(std::make_shared<HealthRestoreStrategy>()) {}
    void interact(Unit& unit) override {
        std::cout << "Source: Full restore!\n";
        unit.getHealth().increase(100);
    }
};


inline void operator+(Unit& unit, TreasureChest& chest) { chest.interact(unit); }
inline void operator-(Unit& unit, Trap& trap) { trap.interact(unit); }
inline void operator*(Unit& unit, Altar& altar) { altar.interact(unit); }
inline void operator/(Unit& unit, EnergySource& source) { source.interact(unit); } 