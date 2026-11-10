#include "GameField.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>

GameField::GameField(int w, int h, int maxU) : width(w), height(h), maxUnits(maxU) {
    initTerrain(w, h);
}
GameField::~GameField() { clear(); }

void GameField::clear() {
    for (Unit* u : units) delete u;
    units.clear();
}

GameField::GameField(const GameField& other) {
    width = other.width; height = other.height; maxUnits = other.maxUnits;
    for (Unit* u : other.units) units.push_back(u->clone());
    terrainMap = other.terrainMap;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        clear();
        width = other.width; height = other.height; maxUnits = other.maxUnits;
        for (Unit* u : other.units) units.push_back(u->clone());
        terrainMap = other.terrainMap;
    }
    return *this;
}

GameField::GameField(GameField&& other) noexcept {
    width = other.width; height = other.height; maxUnits = other.maxUnits;
    units = std::move(other.units); other.units.clear();
    terrainMap = std::move(other.terrainMap);
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        clear();
        width = other.width; height = other.height; maxUnits = other.maxUnits;
        units = std::move(other.units); other.units.clear();
        terrainMap = std::move(other.terrainMap);
    }
    return *this;
}

bool GameField::addUnit(Unit* unit) {
    if (!unit) return false;
    if (units.size() >= maxUnits) { delete unit; return false; }
    if (unit->getX() < 0 || unit->getX() >= width || unit->getY() < 0 || unit->getY() >= height) { delete unit; return false; }
    for (Unit* u : units) {
        if (u->getX() == unit->getX() && u->getY() == unit->getY()) { delete unit; return false; }
    }
    units.push_back(unit); return true;
}

void GameField::removeUnit(int index) {
    if (index >= 0 && index < (int)units.size()) {
        delete units[index];
        units.erase(units.begin() + index);
    }
}

Unit* GameField::getUnit(int index) const {
    if (index >= 0 && index < (int)units.size()) return units[index];
    return nullptr;
}

void GameField::populateField(UnitFactory& factory, const std::string& type, int id, int x, int y) {
    Unit* newUnit = factory.createUnit(type, id, x, y);
    if (newUnit) addUnit(newUnit);
}

void GameField::initTerrain(int w, int h) {
    width = w; height = h;
    terrainMap.assign(h, std::vector<std::shared_ptr<Terrain>>(w, std::make_shared<Plains>()));
}

void GameField::setTerrain(int x, int y, std::shared_ptr<Terrain> t) {
    if (y >= 0 && y < height && x >= 0 && x < width) terrainMap[y][x] = t;
}

TerrainProxy GameField::getTerrainProxy(int x, int y) {
    return TerrainProxy(terrainMap[y][x]);
}

void GameField::addObject(int x, int y, std::unique_ptr<NeutralObject> obj) {
    objects[{x, y}] = std::move(obj);
}

NeutralObject* GameField::getObjectAt(int x, int y) {
    auto it = objects.find({x, y});
    return (it != objects.end()) ? it->second.get() : nullptr;
}

void GameField::printField() const {
    std::vector<std::vector<char>> grid(height, std::vector<char>(width, '.'));
    for (Unit* u : units) {
        char symbol = '?';
        std::string type = u->getType();
        if (type == "Swordsman") symbol = 'M';         
        else if (type == "Spearman") symbol = 'K';     
        else if (type == "LightArcher") symbol = 'L';  
        else if (type == "HeavyArcher") symbol = 'T';  
        else if (type == "Cataphract") symbol = 'C';   
        else if (type == "HorseArcher") symbol = 'H';  
        grid[u->getY()][u->getX()] = symbol; 
    }
    std::cout << "  "; for (int i = 0; i < width; ++i) std::cout << "X "; std::cout << "\n";
    for (int i = 0; i < height; ++i) {
        std::cout << "X ";
        for (int j = 0; j < width; ++j) std::cout << grid[i][j] << " ";
        std::cout << "X\n";
    }
    std::cout << "  "; for (int i = 0; i < width; ++i) std::cout << "X "; std::cout << "\n";
}


void GameField::printMap(int heroX, int heroY, std::string heroType) const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    std::cout << "   ";
    for (int i = 0; i < width; ++i) std::cout << i << " ";
    std::cout << "\n";

    std::cout << "  ";
    for (int i = 0; i < width; ++i) std::cout << "X ";
    std::cout << "\n";

    for (int i = 0; i < height; ++i) {
        std::cout << i << " X ";

        for (int j = 0; j < width; ++j) {
            char symbol = '.'; 
            int color = 8;

            
            if (i == heroY && j == heroX) {
                if (heroType == "Swordsman") symbol = 'S';
                else if (heroType == "Spearman") symbol = 'K';
                else if (heroType == "LightArcher") symbol = 'L';
                else if (heroType == "HeavyArcher") symbol = 'T';
                else if (heroType == "Cataphract") symbol = 'C';
                else if (heroType == "HorseArcher") symbol = 'H';

                color = 14; 
            }
            else if (terrainMap[i][j]) {
                std::string tName = terrainMap[i][j]->getName();
                if (tName == "Forest") { symbol = '*'; color = 10; }
                else if (tName == "Mountains") { symbol = '^'; color = 6; }
                else if (tName == "Swamp") { symbol = '~'; color = 1; }
            }

            SetConsoleTextAttribute(hConsole, color);
            std::cout << symbol << " ";
        }

        SetConsoleTextAttribute(hConsole, 7); 
        std::cout << "X\n";
    }

    std::cout << "  ";
    for (int i = 0; i < width; ++i) std::cout << "X ";
    std::cout << "\n";
}