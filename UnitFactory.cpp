#include "UnitFactory.h"

Unit* InfantryFactory::createUnit(const std::string& type, int id, int x, int y) {
    if (type == "Swordsman") return new Swordsman(id, x, y);
    if (type == "Spearman") return new Spearman(id, x, y);
    return nullptr;
}

Unit* ArcherFactory::createUnit(const std::string& type, int id, int x, int y) {
    if (type == "LightArcher") return new LightArcher(id, x, y);
    if (type == "HeavyArcher") return new HeavyArcher(id, x, y);
    return nullptr;
}

Unit* CavalryFactory::createUnit(const std::string& type, int id, int x, int y) {
    if (type == "Cataphract") return new Cataphract(id, x, y);
    if (type == "HorseArcher") return new HorseArcher(id, x, y);
    return nullptr;
}