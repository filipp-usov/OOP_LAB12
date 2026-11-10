#pragma once
#include "Unit.h"
#include <string>

class UnitFactory {
public:
    virtual ~UnitFactory() = default;
    virtual Unit* createUnit(const std::string& type, int id, int x, int y) = 0;
};

class InfantryFactory : public UnitFactory {
public:
    Unit* createUnit(const std::string& type, int id, int x, int y) override;
};

class ArcherFactory : public UnitFactory {
public:
    Unit* createUnit(const std::string& type, int id, int x, int y) override;
};

class CavalryFactory : public UnitFactory {
public:
    Unit* createUnit(const std::string& type, int id, int x, int y) override;
};