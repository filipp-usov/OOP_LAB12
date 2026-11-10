#pragma once
#include "Observer.h"
#include "UnitFactory.h"
#include <vector>
#include <iostream>

class Base : public IObserver {
private:
    int health;
    int maxUnits;
    int currentUnits;
    std::vector<Unit*> units;
    int nextUnitId;
    UnitFactory* factory;

public:
    Base(int hp, int maxU, UnitFactory* f) 
        : health(hp), maxUnits(maxU), currentUnits(0), nextUnitId(1), factory(f) {}

    void createUnit(const std::string& type, int x, int y) {
        if (currentUnits >= maxUnits) {
            std::cout << "Base: Unit limit reached!\n";
            return;
        }
        Unit* newUnit = factory->createUnit(type, nextUnitId++, x, y);
        if (newUnit) {
            newUnit->subscribe(this);
            units.push_back(newUnit);
            currentUnits++;
            std::cout << "Base: Created unit " << type << " (ID: " << newUnit->getId() << ")\n";
        }
    }

    void onUnitEvent(const UnitEvent& event) override {
        if (event.type == EventType::Destroyed) {
            std::cout << "[Observer] Base notified: Unit ID " << event.unitId << " destroyed!\n";
            currentUnits--;
        } else if (event.type == EventType::Damaged) {
            std::cout << "[Observer] Base notified: Unit ID " << event.unitId << " took damage.\n";
        }
    }
    
    Unit* getUnit(int index) { return units[index]; }
};