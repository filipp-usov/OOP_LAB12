#include <iostream>
#include "GameField.h"
#include "Base.h"
#include "UnitFactory.h"

void runLab1() {
    std::cout << "\n LAB 1 SCENARIOS \n";
    std::cout << "\n Scenario 1.1: Creating field and populating via factories \n";
    GameField field(10, 10, 5); 
    
    InfantryFactory infFactory;
    ArcherFactory archFactory;
    CavalryFactory cavFactory;

    field.populateField(infFactory, "Swordsman", 0, 1, 1);
    field.populateField(archFactory, "LightArcher", 0, 4, 2);
    field.populateField(cavFactory, "Cataphract", 0, 7, 4);
    field.populateField(infFactory, "Spearman", 0, 2, 5);
    
    std::cout << "\nUnits on field: " << field.size() << "\n";
    std::cout << "Field visualization:\n";
    field.printField(); 
    std::cout << "\n";

    std::cout << " Scenario 1.2: Deep copy of the field \n";
    GameField copyField = field; 
    field.getUnit(0)->move(5, 5);
    std::cout << "First unit coords in original: (" 
              << field.getUnit(0)->getX() << ", " << field.getUnit(0)->getY() << ")\n";
    std::cout << "First unit coords in copy: (" 
              << copyField.getUnit(0)->getX() << ", " << copyField.getUnit(0)->getY() << ")\n\n";

    std::cout << " Scenario 1.3: Iteration and move semantics \n";
    for (auto it = field.begin(); it != field.end(); ++it) {
        Unit* u = *it;
        std::cout << "Found unit: " << u->getType() 
                  << " at position (" << u->getX() << ", " << u->getY() << ")\n";
    }
}

void runLab2() {
    std::cout << "\n LAB 2 SCENARIOS \n";
    InfantryFactory infFactory;

    std::cout << "\n Scenario 2.1: Base and Units creation (Observer) \n";
    Base myBase(5, 5, 1000, 5, 10, 500, &infFactory); 
    myBase.createUnit("Swordsman", 1, 1); 
    myBase.createUnit("Spearman", 2, 2);

    std::cout << "\n Scenario 2.2: Movement and Proxy \n";
    GameField newField;
    newField.initTerrain(10, 10);

    
    srand(42); 
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            int r = rand() % 100; 

            
            if (y == 1 && x >= 0 && x <= 5) {
                newField.setTerrain(x, y, std::make_shared<Plains>());
                continue;
            }

            if (r < 25) {
                newField.setTerrain(x, y, std::make_shared<Forest>());     
            }
            else if (r < 40) {
                newField.setTerrain(x, y, std::make_shared<Mountains>());  
            }
            else if (r < 55) {
                newField.setTerrain(x, y, std::make_shared<Swamp>());      
            }
            else {
                newField.setTerrain(x, y, std::make_shared<Plains>());     
            }
        }
    }

    
    newField.setTerrain(3, 1, std::make_shared<Forest>());
    newField.setTerrain(4, 1, std::make_shared<Mountains>());

    Unit* hero = myBase.getUnit(0);
    std::string heroType = hero->getType();

    std::cout << "\nInitial map (. - plains, * - forest, ^ - mountains, ~ - swamp):\n";
    newField.printMap(hero->getX(), hero->getY(), heroType);

    std::cout << "\nHero starts moving from (1, 1) to the right...\n";

    for (int step = 1; step <= 4; ++step) {
        int newX = hero->getX() + 1;
        int newY = hero->getY();

        std::cout << "\nStep " << step << ". Target: (" << newX << ", " << newY << ").\n";

        TerrainProxy proxy = newField.getTerrainProxy(newX, newY);

        if (proxy.isPassable(*hero)) {
            proxy.getSpeedModifier(*hero);
            hero->move(newX, newY);
            std::cout << "Success! Hero is now at (" << hero->getX() << ", " << hero->getY() << ").\n";
            newField.printMap(hero->getX(), hero->getY(), heroType);
        }
        else {
            std::cout << "Path is blocked! Hero stays in place.\n\n";
            break;
        }
    }

    std::cout << "\n Scenario 2.3: Interaction with objects (Overload) \n";
    
    TreasureChest chest;
    *hero + chest; 

    Trap trap;
    *hero - trap;

    Altar altar;
    *hero* altar;

    EnergySource source;
    *hero / source;

    std::cout << "\n Scenario 2.4: Unit destruction and Observer \n";
    std::cout << "Dealing lethal damage to Hero...\n";
    hero->takeDamage(500); 
}

int main() {
    setlocale(LC_ALL, "Russian");
    int choice = 0;
    while (true) {
        std::cout << "    MAIN MENU (MMO RPG)\n";
        std::cout << "1. Run Lab 1 scenarios\n";
        std::cout << "2. Run Lab 2 scenarios\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 1) runLab1();
        else if (choice == 2) runLab2();
        else if (choice == 0) break;
        else std::cout << "Invalid choice.\n";
    }
    return 0;
}