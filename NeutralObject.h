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
        std::cout << "  -> Strategy applied: Health Restore!\n";
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
    Altar() : NeutralObject(std::make_shared<HealthRestoreStrategy>()) {}
    void interact(Unit& unit) override {
        std::cout << "Altar: Unit prayed.\n";
        strategy->apply(unit);
    }
};

inline void operator+(Unit& unit, TreasureChest& chest) { chest.interact(unit); }
inline void operator-(Unit& unit, Trap& trap) { trap.interact(unit); }
inline void operator*(Unit& unit, Altar& altar) { altar.interact(unit); }