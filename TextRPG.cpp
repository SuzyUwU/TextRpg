#include <iostream>
#include <windows.h> 
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

const std::string SAVE_DIR = "save\\savefile\\";

struct Item {
    string name;
    int power;
    int magic;
    int health;
    int defend;
    int count;
    bool isEquipped;
    bool isConsumable;
    int heal;
};

struct Charatermoves {
    string name;
    int Id;
    int damage;
    int manacost;
    int heal, Aoe;
    bool bheal, bAoe;
    bool isEquipped;
};

struct Character {
    string name;
    int level, exp, maxExp, points;
    int magic, mana, maxmana;
    int strength, defend;
    int health, maxHealth;
    int gold;
    Item* inventory[100];
    int itemCount;
    bool isDefending;
    int defendTurnsLeft;
    Charatermoves Moves[10];
    int moveCount, maxweapon;
    string currentMap;
    int x, y;

    Item blacksmithQueue[10];
    int queueSize;
    bool hasCompletedAdventure;

    Character() : queueSize(0) {}
};

struct SpecialMove {
    string name;
    int healAmount;
    int cost;
};

struct Enemy {
    string name;
    int attack;
    int health;
    SpecialMove specialMoves[3];
    int specialMoveCount;
    Item loot[2];
    int lootChance[2]; 
    int pickChance;
    int expDrop;
    int goldDrop;
    int heal;
    int maxhp;
};

struct Map {
    bool isSafe;
    string name;
    int x, y;
    int levelRequired;

    Map() {}
    Map(string n, int xpos, int ypos, bool safe, int levelReq) : isSafe(safe), name(n), x(xpos), y(ypos), levelRequired(levelReq) {}
};

void ensureSaveDirectory() {
    if (!fs::exists(SAVE_DIR)) {
        fs::create_directories(SAVE_DIR);
    }
}

// Printing
void printWithDelay(const string& text, int delay = 15) {
    for (char ch : text) {
        cout << ch;
        Sleep(delay);
    }
    cout << endl;
}

// savingandloading
//saving
void SaveString(HANDLE hFile, const string& str) {
    DWORD written;
    int length = str.length();
    WriteFile(hFile, &length, sizeof(length), &written, NULL);
    WriteFile(hFile, str.c_str(), length, &written, NULL);
}

void saveCharacter(const Character& character, const string& filename) {
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cerr << "Could not open the file for writing." << endl;
        return;
    }

    DWORD written;

    // Save basic attributes
    WriteFile(hFile, &character.name, sizeof(character.name), &written, NULL);
    WriteFile(hFile, &character.level, sizeof(character.level), &written, NULL);
    WriteFile(hFile, &character.exp, sizeof(character.exp), &written, NULL);
    WriteFile(hFile, &character.maxExp, sizeof(character.maxExp), &written, NULL);
    WriteFile(hFile, &character.points, sizeof(character.points), &written, NULL);
    WriteFile(hFile, &character.magic, sizeof(character.magic), &written, NULL);
    WriteFile(hFile, &character.mana, sizeof(character.mana), &written, NULL);
    WriteFile(hFile, &character.maxmana, sizeof(character.maxmana), &written, NULL);
    WriteFile(hFile, &character.strength, sizeof(character.strength), &written, NULL);
    WriteFile(hFile, &character.defend, sizeof(character.defend), &written, NULL);
    WriteFile(hFile, &character.health, sizeof(character.health), &written, NULL);
    WriteFile(hFile, &character.maxHealth, sizeof(character.maxHealth), &written, NULL);
    WriteFile(hFile, &character.gold, sizeof(character.gold), &written, NULL);

    // Save inventory
    WriteFile(hFile, &character.itemCount, sizeof(character.itemCount), &written, NULL);
    for (int i = 0; i < character.itemCount; ++i) {
        SaveString(hFile, character.inventory[i]->name);
        WriteFile(hFile, &character.inventory[i]->power, sizeof(character.inventory[i]->power), &written, NULL);
        WriteFile(hFile, &character.inventory[i]->magic, sizeof(character.inventory[i]->magic), &written, NULL);
        WriteFile(hFile, &character.inventory[i]->health, sizeof(character.inventory[i]->health), &written, NULL);
        WriteFile(hFile, &character.inventory[i]->defend, sizeof(character.inventory[i]->defend), &written, NULL);
        WriteFile(hFile, &character.inventory[i]->count, sizeof(character.inventory[i]->count), &written, NULL);
        WriteFile(hFile, &character.inventory[i]->isEquipped, sizeof(character.inventory[i]->isEquipped), &written, NULL);
        WriteFile(hFile, &character.inventory[i]->isConsumable, sizeof(character.inventory[i]->isConsumable), &written, NULL);
        WriteFile(hFile, &character.inventory[i]->heal, sizeof(character.inventory[i]->heal), &written, NULL);
    }

    // Save moves
    WriteFile(hFile, &character.moveCount, sizeof(character.moveCount), &written, NULL);
    for (int i = 0; i < character.moveCount; ++i) {
        SaveString(hFile, character.Moves[i].name);
        WriteFile(hFile, &character.Moves[i].Id, sizeof(character.Moves[i].Id), &written, NULL);
        WriteFile(hFile, &character.Moves[i].damage, sizeof(character.Moves[i].damage), &written, NULL);
        WriteFile(hFile, &character.Moves[i].manacost, sizeof(character.Moves[i].manacost), &written, NULL);
        WriteFile(hFile, &character.Moves[i].heal, sizeof(character.Moves[i].heal), &written, NULL);
        WriteFile(hFile, &character.Moves[i].Aoe, sizeof(character.Moves[i].Aoe), &written, NULL);
        WriteFile(hFile, &character.Moves[i].bheal, sizeof(character.Moves[i].bheal), &written, NULL);
        WriteFile(hFile, &character.Moves[i].bAoe, sizeof(character.Moves[i].bAoe), &written, NULL);
        WriteFile(hFile, &character.Moves[i].isEquipped, sizeof(character.Moves[i].isEquipped), &written, NULL);
    }

    // Save other attributes
    SaveString(hFile, character.currentMap);
    WriteFile(hFile, &character.x, sizeof(character.x), &written, NULL);
    WriteFile(hFile, &character.y, sizeof(character.y), &written, NULL);

    CloseHandle(hFile);
}

void Saving(Character& hero) {
    char choice = ' ';
    string filename;
    while (choice != 'n') {
        printWithDelay("Are you sure to save your progress y/n");
        cin >> choice;
        if (choice == 'y') {
            printWithDelay("Type your save file name :");
            cin >> filename;
            ensureSaveDirectory();
            string fullPath = SAVE_DIR + filename + ".sav";
            saveCharacter(hero, fullPath);

            printWithDelay("Saving your current progress . . .");
            break;
        } else if (choice == 'n') {
            printWithDelay("Back to main menu");
            break;
        } else {
            printWithDelay("Invalid choice");
        }
    }
}

void LoadString(HANDLE hFile, string& str) {
    DWORD read;
    int length;
    ReadFile(hFile, &length, sizeof(length), &read, NULL);
    char* buffer = new char[length + 1];
    ReadFile(hFile, buffer, length, &read, NULL);
    buffer[length] = '\0';
    str = buffer;
    delete[] buffer;
}

void loadCharacter(Character& character, const string& filename) {
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cerr << "Could not open the file for reading." << endl;
        return;
    }

    DWORD read;

    // Load basic attributes
    ReadFile(hFile, &character.name, sizeof(character.name), &read, NULL);
    ReadFile(hFile, &character.level, sizeof(character.level), &read, NULL);
    ReadFile(hFile, &character.exp, sizeof(character.exp), &read, NULL);
    ReadFile(hFile, &character.maxExp, sizeof(character.maxExp), &read, NULL);
    ReadFile(hFile, &character.points, sizeof(character.points), &read, NULL);
    ReadFile(hFile, &character.magic, sizeof(character.magic), &read, NULL);
    ReadFile(hFile, &character.mana, sizeof(character.mana), &read, NULL);
    ReadFile(hFile, &character.maxmana, sizeof(character.maxmana), &read, NULL);
    ReadFile(hFile, &character.strength, sizeof(character.strength), &read, NULL);
    ReadFile(hFile, &character.defend, sizeof(character.defend), &read, NULL);
    ReadFile(hFile, &character.health, sizeof(character.health), &read, NULL);
    ReadFile(hFile, &character.maxHealth, sizeof(character.maxHealth), &read, NULL);
    ReadFile(hFile, &character.gold, sizeof(character.gold), &read, NULL);

    // Load inventory
    ReadFile(hFile, &character.itemCount, sizeof(character.itemCount), &read, NULL);
    for (int i = 0; i < character.itemCount; ++i) {
        character.inventory[i] = new Item;
        LoadString(hFile, character.inventory[i]->name);
        ReadFile(hFile, &character.inventory[i]->power, sizeof(character.inventory[i]->power), &read, NULL);
        ReadFile(hFile, &character.inventory[i]->magic, sizeof(character.inventory[i]->magic), &read, NULL);
        ReadFile(hFile, &character.inventory[i]->health, sizeof(character.inventory[i]->health), &read, NULL);
        ReadFile(hFile, &character.inventory[i]->defend, sizeof(character.inventory[i]->defend), &read, NULL);
        ReadFile(hFile, &character.inventory[i]->count, sizeof(character.inventory[i]->count), &read, NULL);
        ReadFile(hFile, &character.inventory[i]->isEquipped, sizeof(character.inventory[i]->isEquipped), &read, NULL);
        ReadFile(hFile, &character.inventory[i]->isConsumable, sizeof(character.inventory[i]->isConsumable), &read, NULL);
        ReadFile(hFile, &character.inventory[i]->heal, sizeof(character.inventory[i]->heal), &read, NULL);
    }

    // Load moves
    ReadFile(hFile, &character.moveCount, sizeof(character.moveCount), &read, NULL);
    for (int i = 0; i < character.moveCount; ++i) {
        LoadString(hFile, character.Moves[i].name);
        ReadFile(hFile, &character.Moves[i].Id, sizeof(character.Moves[i].Id), &read, NULL);
        ReadFile(hFile, &character.Moves[i].damage, sizeof(character.Moves[i].damage), &read, NULL);
        ReadFile(hFile, &character.Moves[i].manacost, sizeof(character.Moves[i].manacost), &read, NULL);
        ReadFile(hFile, &character.Moves[i].heal, sizeof(character.Moves[i].heal), &read, NULL);
        ReadFile(hFile, &character.Moves[i].Aoe, sizeof(character.Moves[i].Aoe), &read, NULL);
        ReadFile(hFile, &character.Moves[i].bheal, sizeof(character.Moves[i].bheal), &read, NULL);
        ReadFile(hFile, &character.Moves[i].bAoe, sizeof(character.Moves[i].bAoe), &read, NULL);
        ReadFile(hFile, &character.Moves[i].isEquipped, sizeof(character.Moves[i].isEquipped), &read, NULL);
    }

    // Load other attributes
    LoadString(hFile, character.currentMap);
    ReadFile(hFile, &character.x, sizeof(character.x), &read, NULL);
    ReadFile(hFile, &character.y, sizeof(character.y), &read, NULL);

    CloseHandle(hFile);
}

void Load(Character& hero) {
    char choice = ' ';
    string filename;

    while (choice != 'n') {
        printWithDelay("Are you sure to load your progress y/n");
        cin >> choice;
        if (choice == 'y') {
            while (true) {
                printWithDelay("Type your save file name:");
                cin >> filename;

                // Attempt to open the file to check if it exists
                HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE) {
                    cerr << "Could not open the file. Please enter a valid file name." << endl;
                } else {
                    CloseHandle(hFile);
                    string fullPath = SAVE_DIR + filename + ".sav";
                    loadCharacter(hero, fullPath);
                    printWithDelay("Loading your save file . . .");
                    break;
                }
            }
            break;
        } else if (choice == 'n') {
            printWithDelay("Back to main menu");
            break;
        } else {
            printWithDelay("Invalid choice");
        }
    }
}

// random number
int getRandomNumber(int min, int max) {
    static unsigned int seed = GetTickCount();
    seed = (214013 * seed + 2531011);
    return min + (seed >> 16) % (max - min + 1);
}

// starting wapon
Item chooseStartingWeapon() {
    cout << "\n";
    printWithDelay("Dont Forget to equip your weapon \n");
    printWithDelay("Choose your starting weapon:");
    printWithDelay("1. Apprentice Staff (5 atk 25 mana)");
    printWithDelay("2. Wood Sword (10 atk)");
    printWithDelay("3. A Stick (7 atk 15 mana)");

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            return {"Apprentice Staff", 5, 5, 0, 0, 1, false, false, 0};
        case 2:
            return {"Wood Sword", 10, 0, 0, 0, 1, false, false, 0};
        case 3:
            return {"A Stick", 7, 3, 0, 0, 1, false, false, 0};
        default:
            printWithDelay("Invalid choice, defaulting to A Stick.");
            return {"A Stick", 7, 3, 0, 0, 1, false, false, 0};
    }
}

// Exp
int calculateRequiredExp(Character& hero, int level) {
    int requiredExp = 20;
    for (int i = 1; i < level; i++) {
        requiredExp += (requiredExp * 0.2);
    }
    hero.maxExp = requiredExp;
    return requiredExp;
}

void statsallocation (Character& hero) {

    while (hero.points > 0) {
        printWithDelay("=========================");
        printWithDelay("You have " + to_string(hero.points) + " stat points to allocate.");
        printWithDelay("1. Magic (current: " + to_string(hero.magic) + ")");
        printWithDelay("2. Strength (current: " + to_string(hero.strength) + ")");
        printWithDelay("3. Health (current: " + to_string(hero.maxHealth) + ")");
        printWithDelay("=========================");
        printWithDelay("Choose a stat to increase: ");
        
        int choice, count;
        cin >> choice;
        printWithDelay("how much do you want to allocate : ");
        cin >> count;
        if (hero.points >= count)
        {
            switch (choice) {
            case 1:
                hero.magic += count;
                break;
            case 2:
                hero.strength += count;
                break;
            case 3:
                hero.maxHealth += count;
                hero.health = hero.maxHealth;
                break;
            default:
                printWithDelay("Invalid choice.");
                continue;
        }
        }else
        {
            printWithDelay("you do have eough point \n");
        }
        printWithDelay("=========================");
        hero.points -= count;
    }
}

// levelup
void levelUp(Character& hero) {
    printWithDelay("=========================");
    char choice;
    hero.level++;
    hero.maxHealth += 1;
    printWithDelay("Level up! You are now level " + to_string(hero.level) + ".");
    hero.points += 3;
    printWithDelay("You have " + to_string(hero.points) + " stat points to allocate.");
    printWithDelay("=========================");
    printWithDelay("Allocate your stats y/n :");
    cin >> choice;
    if (choice == 'y')
    {
        statsallocation(hero);
    } else
    {
        printWithDelay("=========================");
    }
    hero.exp = hero.exp - (hero.maxExp);
    hero.health = hero.maxHealth;
}

void levelupcheck (Character& hero) {
    int i = calculateRequiredExp(hero, hero.level);
    hero.maxExp = i;
    if (hero.maxExp <= hero.exp)
    {
        levelUp(hero);
    }
    
}

// mapping
const int MAX_MAPS = 5;
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

void renderMap(const Map maps[], int mapCount, const Character& character) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (i == character.y && j == character.x) {
                cout << 'X' << " ";
            } else {
                bool isTargetLocation = false;
                for (int k = 0; k < mapCount; ++k) {
                    if (i == maps[k].y && j == maps[k].x) {
                        cout << 'O' << " ";
                        isTargetLocation = true;
                        break;
                    }
                }
                if (!isTargetLocation) {
                    cout << '#' << " ";
                }
            }
        }
        cout << endl;
    }
}

bool canAccessMap(const Map &location, int characterLevel) {
    return characterLevel >= location.levelRequired;
}

void checkMapSafety(const Map &location) {
    cout << "Checking safety status of " << location.name << " at coordinates (" << location.x << ", " << location.y << "): ";
    if (location.isSafe) {
        cout << "The location is safe." << endl;
    } else {
        cout << "The location is not safe!" << endl;
    }
    Sleep(1000);
}

void setMapSafety(Map &location, bool safetyStatus) {
    location.isSafe = safetyStatus;
    cout << "Safety status of " << location.name << " has been updated." << endl;
    Sleep(1000);
}

Map createMap(string name, int x, int y, bool isSafe, int levelRequired) {
    return Map(name, x, y, isSafe, levelRequired);
}

Map maps[MAX_MAPS] = {
    createMap("Town", 0, 0, true, 1),
    createMap("Sewer", 0, 1, true, 3),
    createMap("Forest", 3, 3, true, 5),
    createMap("Cave", 5, 5, false, 7),
    createMap("Gem Cave", 7, 7, false, 9)
};

void moveCharacterToArea(Character& character, const Map& targetMap) {
    if (canAccessMap(targetMap, character.level)) {
        character.currentMap = targetMap.name;
        character.x = targetMap.x;
        character.y = targetMap.y;
        printWithDelay("Character moved to " + targetMap.name + " at coordinates (" + to_string(character.x) + ", " + to_string(character.y) + ").");
    } else {
        printWithDelay("You don't meet the level must be " + to_string(targetMap.levelRequired) + " requirement to access " + targetMap.name + ".");
    }
}

void displayCurrentLocation(const Character& character) {
    printWithDelay("Current Location: " + character.currentMap + " (" + to_string(character.x) + ", " + to_string(character.y) + ")");
}

void displayMapOptions(Character& hero, Map (&maps)[MAX_MAPS]) {
    renderMap(maps, MAX_MAPS, hero); 
    displayCurrentLocation(hero);
    printWithDelay("Available movement options:");

    for (int i = 0; i < MAX_MAPS; ++i) {
        if (maps[i].name != hero.currentMap) {
            printWithDelay(maps[i].name + ": Press " + maps[i].name[0] + " to move to " + maps[i].name);
        }
    }

    char choice;
    printWithDelay("Enter your choice: ");
    cin >> choice;

    for (int i = 0; i < MAX_MAPS; ++i) {
        if (toupper(choice) == maps[i].name[0]) {
            moveCharacterToArea(hero, maps[i]);
            renderMap(maps, MAX_MAPS, hero); 
            return;
        }
    }

    printWithDelay("Invalid choice. Please try again.");
}

//enemys
int randlootdrop (){
    int randomValue = getRandomNumber(1, 2);
    return randomValue;
}
 
void addEnemy(Enemy& enemy, const string& name, int attack, int health, const string& specialMoveName, int healAmount, const string& loot1, int loot1Chance, const string& loot2, int loot2Chance, int pickChance, int expDrop, int goldDrop) {
    enemy.name = name;
    enemy.attack = attack;
    enemy.health = health;
    enemy.specialMoves[2] = {specialMoveName, healAmount, health};
    enemy.specialMoveCount = 1;
    enemy.loot[0] = {loot1, 0, 0, 0, 0, randlootdrop(), false, false, 0};
    enemy.lootChance[0] = loot1Chance;
    enemy.loot[1] = {loot2, 0, 0, 0, 0, randlootdrop(), false, false, 0};
    enemy.lootChance[1] = loot2Chance;
    enemy.pickChance = pickChance;
    enemy.expDrop = expDrop;
    enemy.goldDrop = goldDrop;
    enemy.heal = healAmount;
    enemy.maxhp = health;
}

int enemydmg(Enemy& enemy, Character& hero){
    int damage = enemy.attack;
    if (hero.isDefending) {
        damage =((damage * 80 + 99) / 100) - (hero.defend * 0.5);
        hero.defendTurnsLeft--;
        if (hero.defendTurnsLeft == 0) {
            hero.isDefending = false;
        }
    } else if (!hero.isDefending)
    {
        damage = enemy.attack - ( hero.defend * 0.5);
    }
    return damage;
}

// generating the amount enemy betwen 1-3
void generateRandomEnemies(Enemy PotentialForestEnemies[], int PotentialForestCount, Enemy encounteredEnemies[], int& encounterCount) {
    int totalPickChance = 0;
    for (int i = 0; i < PotentialForestCount; ++i) {
        totalPickChance += PotentialForestEnemies[i].pickChance;
    }

    encounterCount = getRandomNumber(1, 3);

    for (int i = 0; i < encounterCount; ++i) {
        int randomValue = getRandomNumber(1, totalPickChance);
        int cumulativeChance = 0;
        for (int j = 0; j < PotentialForestCount; ++j) {
            cumulativeChance += PotentialForestEnemies[j].pickChance;
            if (randomValue <= cumulativeChance) {
                encounteredEnemies[i] = PotentialForestEnemies[j];
                break;
            }
        }
    }
    
    // Print the names of the encountered enemies
    printWithDelay("You encountered:");
    for (int i = 0; i < encounterCount; ++i) {
        printWithDelay("- " + encounteredEnemies[i].name);
    }
}

// mana system
void maxmanacheck(Character& hero) {
    hero.maxmana = 5 + (hero.magic * 5);
    hero.mana = hero.maxmana; 
}

// inventoryrelated
// equip and unequip
void eoren (){
        printWithDelay("=========================");
        printWithDelay("Your turn:");
        printWithDelay("1. equip");
        printWithDelay("2. unequip");
        printWithDelay("3. exit");
        printWithDelay("Your choice :");
        printWithDelay("=========================");
    int choice;
    cin >> choice;
}

void equipItem(Character& hero, int itemIndex) {
    if (itemIndex < 0 || itemIndex >= hero.itemCount) {
        printWithDelay("Invalid item index.");
        return;
    }

    Item* item = hero.inventory[itemIndex];
    if (hero.maxweapon > 3 && item->power < 1) {
        printWithDelay("Item cannot be equiped.");
        return;
    } else if(item->isEquipped == false && item->power > 0)
    {
        hero.health += item->health;
        hero.magic += item->magic;
        hero.strength += item->power;
        hero.defend += item->defend;
        hero.maxweapon++;

        item->isEquipped = true;
        printWithDelay("Equipped " + item->name + ".");
    } 
}

void unequipItem(Character& hero, int itemIndex) {
    if (itemIndex < 0 || itemIndex >= hero.itemCount) {
        printWithDelay("Invalid item index.");
        return;
    }

    Item* item = hero.inventory[itemIndex];
    if (!item->isEquipped) {
        printWithDelay("Item is not equipped.");
        return;
    } else if (item->isEquipped) {
        hero.health -= item->health;
        hero.magic -= item->magic;
        hero.strength -= item->power;
        hero.defend -= item->defend;
        hero.maxweapon--;

        item->isEquipped = false;
        printWithDelay("Unequipped " + item->name + ".");
    }
}

//searching
void searchInventory(const Character& character, const string& itemName) {
    bool found = false;
    for (int i = 0; i < character.itemCount; ++i) {
        if (character.inventory[i]->name == itemName) {
            printWithDelay("Found " + itemName + " in your inventory with " + to_string(character.inventory[i]->power) + " power and " + to_string(character.inventory[i]->count) + " count.");
            found = true;
        }
    }
    if (!found) {
        printWithDelay("Item " + itemName + " not found in your inventory.");
    }
}

//sorting ivetory
void bubbleSortByName(Item* inventory[], int itemCount) {
    for (int i = 0; i < itemCount - 1; ++i) {
        for (int j = 0; j < itemCount - i - 1; ++j) {
            if (inventory[j]->name > inventory[j + 1]->name) {
                swap(inventory[j], inventory[j + 1]);
            }
        }
    }
    printWithDelay("Inventory sorted by name.");
}

void bubbleSortByPower(Item* inventory[], int itemCount) {
    for (int i = 0; i < itemCount - 1; ++i) {
        for (int j = 0; j < itemCount - i - 1; ++j) {
            if (inventory[j]->power < inventory[j + 1]->power) {
                swap(inventory[j], inventory[j + 1]);
            }
        }
    }
    printWithDelay("Inventory sorted by power.");
}

void bubbleSortByCount(Item* inventory[], int itemCount) {
    for (int i = 0; i < itemCount - 1; ++i) {
        for (int j = 0; j < itemCount - i - 1; ++j) {
            if (inventory[j]->count < inventory[j + 1]->count) {
                swap(inventory[j], inventory[j + 1]);
            }
        }
    }
    printWithDelay("Inventory sorted by count.");
}

void sortInventory(Character& character, int sortBy) {
    switch (sortBy) {
        case 1:
            bubbleSortByName(character.inventory, character.itemCount);
            break;
        case 2:
            bubbleSortByPower(character.inventory, character.itemCount);
            break;
        case 3:
            bubbleSortByCount(character.inventory, character.itemCount);
            break;
        default:
            printWithDelay("Invalid sort option.");
            break;
    }
}

void manageInventory(Character& hero) {
    int choice;
    do {
        printWithDelay("=========================");
        printWithDelay("Inventory Management:");
        for (int i = 0; i < hero.itemCount; ++i) {
            string equipStatus = hero.inventory[i]->isEquipped ? " (Equipped)" : "";
            printWithDelay(to_string(i + 1) + ". " + hero.inventory[i]->name + equipStatus);
        }
        printWithDelay("0. Back to Main Menu");
        printWithDelay("Enter the number of the item to equip/unequip, or 0 to go back:");
        printWithDelay("=========================");

        cin >> choice;
        if (choice > 0 && choice <= hero.itemCount) {
            int itemIndex = choice - 1;
            if (hero.inventory[itemIndex]->isEquipped) {
                unequipItem(hero, itemIndex);
                hero.maxweapon--;
            } else {
                equipItem(hero, itemIndex);
                hero.maxweapon++;
            }
        } else if (choice != 0) {
            printWithDelay("Invalid choice.");
        }
    } while (choice != 0);
}

//display
void displayInventory(Character& character) {
    const int itemsPerPage = 5; 
    int totalPages = (character.itemCount + itemsPerPage - 1) / itemsPerPage;
    int currentPage = 1; 
    string equip;
    

    while (true) {
        printWithDelay("=========================");
        printWithDelay("Name: " + character.name);
        printWithDelay("(" + to_string(character.gold) + " gold)");
        printWithDelay("=========================");
        printWithDelay("Inventory (Page " + to_string(currentPage) + " of " + to_string(totalPages) + "):");

        int startIdx = (currentPage - 1) * itemsPerPage;
        int endIdx = min(startIdx + itemsPerPage, character.itemCount);

        for (int i = startIdx; i < endIdx; ++i) {
            if (character.inventory[i]->power > 0 || character.inventory[i]->magic > 0 || character.inventory[i]->health > 0 || character.inventory[i]->defend > 0) {
                if (character.inventory[i]->isEquipped == true)
                {
                    equip = "true";
                } else
                {
                    equip = "false";
                }
                printWithDelay(to_string(i + 1) + ". " + character.inventory[i]->name + " (" + to_string(character.inventory[i]->power) + " Power)" + " (" + to_string(character.inventory[i]->magic*5) + " Mana)" +" (" + to_string(character.inventory[i]->health) + " Hp)" +" (" + to_string(character.inventory[i]->defend) + " Def)" + "( equip :" + equip + " )");
            } else {
                printWithDelay(to_string(i + 1) + ". " + character.inventory[i]->name + " (" + to_string(character.inventory[i]->count) + ")");
            }
        }
        printWithDelay("=========================");
        printWithDelay(" 'f' searching \n 's' sorting\n 'n' next page\n 'p' prev page\n 'e' equip, unequip, and replace item\n 'q' to quit \n your choice :");

        char input;
        cin >> input;

        if (input == 'n' && currentPage < totalPages) {
            currentPage++;
        } else if (input == 'p' && currentPage > 1) {
            currentPage--;
        } else if (input == 'q') {
            break;
        }else if (input == 'e') {
            manageInventory(character);
            break;
        } else if (input == 'f') {
            string searchItem;
            printWithDelay("Enter the item name to search: ");
            cin.ignore();
            getline(cin, searchItem);
            searchInventory(character, searchItem);
        } else if (input == 's') {
            char sortChoice;
            printWithDelay("Sort by: (n)ame\n (p)ower\n (c)ount");
            cin >> sortChoice;
            if (sortChoice == 'n') {
                sortInventory(character, 1);
            } else if (sortChoice == 'p') {
                sortInventory(character, 2);
            } else if (sortChoice == 'c') {
                sortInventory(character, 3);
            }
        } else {
            printWithDelay("Invalid input, try again.");
        }
    }
}

//loot handleing
void addItemToInventory(Character& character, const Item& newItem) {
    
    for (int i = 0; i < character.itemCount; ++i) {
        if (character.inventory[i]->name == newItem.name) {
            if (newItem.power > 0) {
                printWithDelay("Item with power > 0 and the same name already exists in your inventory.");
                return;
            } else {
                character.inventory[i]->count += newItem.count;
                printWithDelay("Increased " + newItem.name + " count to " + to_string(character.inventory[i]->count) + ".");
                return;
            }
        }
    }

    
    if (character.itemCount < 100) {
        character.inventory[character.itemCount] = new Item(newItem);
        character.itemCount++;
        printWithDelay("Added " + newItem.name + " to your inventory.");
    } else {
        printWithDelay("Inventory is full. Cannot add " + newItem.name + ".");
    }
}

void handleLootDrop(Character& hero, const Enemy& enemy) {
    for (int i = 0; i < 2; ++i) {
        if (getRandomNumber(1, 100) <= enemy.lootChance[i]) {
            addItemToInventory(hero, enemy.loot[i]);
            printWithDelay("You gained " + enemy.loot[i].name + " ("  + to_string(enemy.loot[i].count) + " count).");
        }
    }
}

//consumables
void useConsumables(Character& character) {
    int consumableCounter = 0;
    printWithDelay("Consumable Items in Inventory:\n");
    for (int i = 0; i < character.itemCount; ++i) {
        if (character.inventory[i]->isConsumable) {
            printWithDelay(to_string(consumableCounter + 1) + ". " + character.inventory[i]->name + " (" + to_string(character.inventory[i]->count) + ") Heals: " + to_string(character.inventory[i]->heal) + "\n");
            consumableCounter++;
        }
    }

    if (consumableCounter == 0) {
        printWithDelay("No consumable items available.\n");
        return;
    }

    int choice;
    printWithDelay("Choose a consumable item to use (1-" + to_string(consumableCounter) + "): ");
    cin >> choice;

    if (choice < 1 || choice > consumableCounter) {
        printWithDelay("Invalid choice!\n");
        return;
    }

    Item* consumableItem = nullptr;
    int consumableIndex = 0;
    for (int i = 0; i < character.itemCount; ++i) {
        if (character.inventory[i]->isConsumable) {
            consumableIndex++;
            if (consumableIndex == choice) {
                consumableItem = character.inventory[i];
                break;
            }
        }
    }

    character.health += consumableItem->heal;
    if (character.health > character.maxHealth) {
        character.health = character.maxHealth;
    }
    printWithDelay("You Healed " + to_string(consumableItem->heal) + "HP\n");

    consumableItem->count--;

    if (consumableItem->count <= 0) {
        delete consumableItem;
        for (int i = consumableIndex - 1; i < character.itemCount - 1; ++i) {
            character.inventory[i] = character.inventory[i + 1];
        }
        character.inventory[--character.itemCount] = nullptr;
    }
}

//character
// Naming
string getCharacterName() {
    string name;
    printWithDelay("Enter your character's name: ");
    cin >> ws;
    getline(cin, name);
    return name;
}

// weaponupgrade
void upgradeWeapon(Character& character) {
    const int MAX_EQUIPPED_WEAPONS = 3;
    int equippedWeaponIndices[MAX_EQUIPPED_WEAPONS];
    int equippedCount = 0;

    for (int i = 0; i < character.itemCount && equippedCount < MAX_EQUIPPED_WEAPONS; ++i) {
        if (character.inventory[i]->isEquipped) {
            equippedWeaponIndices[equippedCount++] = i;
        }
    }

    if (equippedCount == 0) {
        printWithDelay("You don't have any weapons equipped.\n");
        return;
    }

    printWithDelay("Choose a weapon to upgrade:");
    for (int i = 0; i < equippedCount; ++i) {
        int index = equippedWeaponIndices[i];
        int upgradePrice = character.inventory[index]->power * 5;
        printWithDelay(to_string(i + 1) + ". " + character.inventory[index]->name + " Upgrade cost: " + to_string(upgradePrice) + " gold");
    }
    printWithDelay(to_string(equippedCount + 1) + ". Exit");

    int choice;
    cin >> choice;

    if (choice < 1 || choice > equippedCount + 1) {
        printWithDelay("Invalid choice!\n");
        return;
    }

    if (choice == equippedCount + 1) {
        printWithDelay("Exiting upgrade menu...\n");
        return;
    }

    int selectedWeaponIndex = equippedWeaponIndices[choice - 1];
    int upgradePrice = character.inventory[selectedWeaponIndex]->power * 5;
    if (character.gold < upgradePrice) {
        printWithDelay("You don't have enough gold to upgrade this weapon.\n");
        return;
    }

    Item* item = character.inventory[selectedWeaponIndex];
    character.gold -= upgradePrice;
    character.health -= item->health;
    character.magic -= item->magic;
    character.strength -= item->power;
    character.defend -= item->defend;
    character.inventory[selectedWeaponIndex]->power = static_cast<int>(character.inventory[selectedWeaponIndex]->power * 1.2);
    character.health += item->health;
    character.magic += item->magic;
    character.strength += item->power;
    character.defend += item->defend;
    printWithDelay(character.inventory[selectedWeaponIndex]->name + " has been upgraded!\n");
}

//shops
void addToBlacksmithQueue(Character& character, const Item& item) {
    if (character.queueSize < 10) {
        character.hasCompletedAdventure = false;
        character.blacksmithQueue[character.queueSize++] = item;
        printWithDelay(item.name + " added to the blacksmith queue. Claim it after completing an adventure.\n");
    } else {
        printWithDelay("Blacksmith queue is full!\n");
    }
}

bool isBlacksmithQueueEmpty(const Character& character) {
    return character.queueSize == 0;
}

Item claimFromBlacksmithQueue(Character& character) {
    if (!isBlacksmithQueueEmpty(character)) {
        Item itemToClaim = character.blacksmithQueue[0];

        for (int i = 0; i < character.queueSize - 1; ++i) {
            character.blacksmithQueue[i] = character.blacksmithQueue[i + 1];
        }
        character.queueSize--;

        return itemToClaim;
    } else {
        printWithDelay("No items in the blacksmith queue.\n");
        return {"", 0, 0, 0, 0, 0, false, false, 0};
    }
}

void blacksmithShop(Character& character) {
    Item shopItems[3] = {
        {"Iron Sword", 15, 0, 0, 0, 1, false, false, 0},
        {"Magic Axe", 10, 5, 0, 0, 1, false, false, 0},
        {"Shield", 0, 0, 0, 5, 1, false, false, 0}
    };
    int itemPrices[3] = {50, 70, 40};

    while (true) {
        printWithDelay("(" + to_string(character.gold) + " gold)");
        printWithDelay("Welcome to the Blacksmith! Choose an option:\n");
        printWithDelay("1. Buy Weapon\n2. Upgrade Weapon\n3. Claim Weapon from Queue\n4. Exit\n");
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            printWithDelay("Invalid input! Please enter a number.\n");
            continue;
        }

        if (choice == 1) {
            printWithDelay("Weapons available for purchase:\n");
            for (int i = 0; i < 3; ++i) {
                cout << i + 1 << ". " << shopItems[i].name << " - " << itemPrices[i] << " gold\n";
            }
            printWithDelay("Choose a weapon to buy (1-3):\n");
            int itemChoice;
            cin >> itemChoice;

            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(10000, '\n'); 
                printWithDelay("Invalid input! Please enter a number.\n");
                continue;
            }

            if (itemChoice >= 1 && itemChoice <= 3) {
                if (character.gold >= itemPrices[itemChoice - 1]) {
                    character.gold -= itemPrices[itemChoice - 1];
                    addToBlacksmithQueue(character, shopItems[itemChoice - 1]);
                } else {
                    printWithDelay("Not enough gold!\n");
                }
            } else {
                printWithDelay("Invalid choice!\n");
            }
        } else if (choice == 2) {
            upgradeWeapon(character);
        } else if (choice == 3) {
            if (character.hasCompletedAdventure) {
                if (!isBlacksmithQueueEmpty(character)) {
                    Item claimedItem = claimFromBlacksmithQueue(character);
                    if (claimedItem.name != "") {
                        addItemToInventory(character, claimedItem);
                        printWithDelay("Claimed " + claimedItem.name + " from the queue.\n");
                    }
                } else {
                    printWithDelay("No items in the blacksmith queue to claim.\n");
                }
            } else {
                printWithDelay("You need to complete an adventure before claiming an item.\n");
            }
        } else if (choice == 4) {
            printWithDelay("Exiting Blacksmith...\n");
            break;
        } else {
            printWithDelay("Invalid choice!\n");
        }
    }
}

void bakeryShop(Character &character) {
    Item shopItems[3] = {
        {"Bread", 0, 0, 10, 0, 1, false, true, 10},
        {"Potion", 0, 0, 20, 0, 1, false, true, 20},
        {"Cake", 0, 0, 30, 0, 1, false, true, 30}
    };
    int itemPrices[3] = {5, 10, 20};

    while (true) {
        printWithDelay("(" + to_string(character.gold) + " gold)");
        printWithDelay("Welcome to the Bakery! Choose an option:\n");
        printWithDelay("1. Buy Consumable\n2. Exit\n");
        int choice;
        cin >> choice;

        if (choice == 1) {
            printWithDelay("Consumables available for purchase:\n");
            for (int i = 0; i < 3; ++i) {
                cout << i + 1 << ". " << shopItems[i].name << " - " << itemPrices[i] << " gold\n";
            }
            printWithDelay("Choose a consumable to buy (1-3):\n");
            int itemChoice;
            cin >> itemChoice;

            if (itemChoice >= 1 && itemChoice <= 3) {
                if (character.gold >= itemPrices[itemChoice - 1]) {
                    character.gold -= itemPrices[itemChoice - 1];
                    addItemToInventory(character, shopItems[itemChoice - 1]);
                } else {
                    printWithDelay("Not enough gold!\n");
                }
            } else {
                printWithDelay("Invalid choice!\n");
            }
        } else if (choice == 2) {
            printWithDelay("Exiting Bakery...\n");
            break;
        } else {
            printWithDelay("Invalid choice!\n");
        }
    }
}

int itemprice(Character &character, int i) {
    int price = 0;
    if (character.inventory[i]->name == "Mushroom") {
        price = 2;
    } else if (character.inventory[i]->name == "Rock") {
        price = 2;
    } else if (character.inventory[i]->name == "Herbs") {
        price = 5;
    } else if (character.inventory[i]->name == "Stick") {
        price = 3;
    } else if (character.inventory[i]->name == "Iron") {
        price = 10;
    } else if (character.inventory[i]->name == "Gems") {
        price = 20;
    } else if (character.inventory[i]->name == "Diamonds") {
        price = 30;
    } else {
        printWithDelay("The item doesn't have value\n");
        return 0;
    }
    return price;
}

void deleteItemFromInventory(Character& character, int index) {
    delete character.inventory[index];
    for (int i = index; i < character.itemCount - 1; ++i) {
        character.inventory[i] = character.inventory[i + 1];
    }
    character.inventory[--character.itemCount] = nullptr;
    printWithDelay("DEBUG: Item deleted and inventory updated\n");
}

void hunterShop(Character &character) {
    bool exitShop = false;
    while (!exitShop) {
        printWithDelay("(" + to_string(character.gold) + " gold)\n");
        printWithDelay("Welcome to the Hunter Shop! Choose an option:\n");
        printWithDelay("1. Sell Item\n2. Exit\n");
        
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                if (character.itemCount == 0) {
                    printWithDelay("Your inventory is empty.\n");
                    break;
                }

                printWithDelay("Your inventory:\n");
                for (int i = 0; i < character.itemCount; ++i) {
                    printWithDelay(to_string(i + 1) + ". " + character.inventory[i]->name + " (Count: " + to_string(character.inventory[i]->count) + ")\n");
                }
                printWithDelay("Choose an item to sell (1-" + to_string(character.itemCount) + "):\n");

                int itemChoice;
                cin >> itemChoice;

                if (itemChoice >= 1 && itemChoice <= character.itemCount) {
                    Item* selectedItem = character.inventory[itemChoice - 1];
                    int sellPrice = itemprice(character, itemChoice - 1);
                    if (sellPrice == 0) {
                        printWithDelay("This item cannot be sold.\n");
                        break;
                    }

                    int maxSellable = selectedItem->count - 1;
                    if (maxSellable <= 0) {
                        printWithDelay("Cannot sell this item as only 1 is left.\n");
                        break;
                    }

                    printWithDelay("Enter the quantity to sell (1-" + to_string(maxSellable) + "):\n");
                    int quantity;
                    cin >> quantity;

                    if (quantity <= 0 || quantity > maxSellable) {
                        printWithDelay("Invalid quantity! Must be at least 1 and at most " + to_string(maxSellable) + ".\n");
                        break;
                    }

                    character.gold += sellPrice * quantity;
                    selectedItem->count -= quantity;

                    printWithDelay("Sold " + to_string(quantity) + " " + selectedItem->name + "(s) for " + to_string(sellPrice * quantity) + " gold.\n");
                } else {
                    printWithDelay("Invalid choice!\n");
                }
                break;

            case 2:
                printWithDelay("Exiting Hunter Shop...\n");
                exitShop = true;
                break;

            default:
                printWithDelay("Invalid choice!\n");
                break;
        }
    }
}

void displayShopMenu(Character &character){
    while (true) {
        int shopChoice;
        printWithDelay("Choose a shop to visit:\n1. Blacksmith\n2. Bakery\n3. Hunter Shop\n4. Exit\n");
        cin >> shopChoice;

        if (shopChoice == 1) {
            blacksmithShop(character);
        } else if (shopChoice == 2) {
            bakeryShop(character);
        } else if (shopChoice == 3) {
            hunterShop(character);
        } else if (shopChoice == 4) {
            printWithDelay("Exiting the game...\n");
            break;
        } else {
            printWithDelay("Invalid choice!\n");
        }
    }
}

// moves
void addMoves(Character& hero, const string& movename, int Id, int damage, int manacost, int heal, int Aoe, bool bheal, bool bAoe, bool equip) {
    if (hero.moveCount < 30) { 
        hero.Moves[hero.moveCount] = {movename, Id, damage, manacost, heal, Aoe, bheal, bAoe, equip};
        hero.moveCount++;
    } else {
        printWithDelay("Cannot add more moves, maximum limit reached.");
    }
}

int movedamage(Character& hero, int i){
    int damage = hero.Moves[i].damage + hero.strength;
    return damage;
}

void moveheal(Character& hero, int i){
    hero.health += hero.Moves[i].heal;
    if (hero.health > hero.maxHealth) {
        hero.health = hero.maxHealth;
    }
    printWithDelay("You healed " + to_string(hero.Moves[i].heal) + " HP.");
}

int moveAoe(Character& hero, int i){
    int damage = hero.Moves[i].Aoe + (hero.strength / 2);
    return damage;
}

int movemanacost(Character& hero, int i){
    int check;
    check = hero.mana - hero.Moves[i].manacost;
    if (check <= 0)
    {
        printWithDelay("Not enough mana to use " + hero.Moves[i].name + ".");
        return 0;
    } else
    {
        hero.mana -= hero.Moves[i].manacost;
        return 1;
    }
    
    
}

int movecounter(Character& hero, int choice){
    int move = 0;
    int j = 0, i = 0;
    while (i < hero.moveCount)
    {
        if (hero.Moves[i].isEquipped)
        {
            move++;
            if (choice == move)
            {
                j = i;
                break;
            }
            
        }
        i++;
    }
    
    return j;
}

void usemove(Character& hero, int i){
        if (hero.Moves[i].bheal) {
            moveheal(hero, i);
        }
}

void equipmove(Character& hero) {
    int equippedCount = 0;
    for (int i = 0; i < hero.moveCount; ++i) {
        if (hero.Moves[i].isEquipped) {
            equippedCount++;
        }
    }

    printWithDelay("Enter the move number to equip/replace: ");
    int moveNumber;
    cin >> moveNumber;

    if (moveNumber < 1 || moveNumber > hero.moveCount) {
        printWithDelay("Invalid move number.");
        return;
    }

    if (hero.Moves[moveNumber - 1].isEquipped) {
        printWithDelay("Move is already equipped.");
        return;
    }

    if (equippedCount >= 4) {
        printWithDelay("You already have 4 moves equipped. Choose a move to replace:");
        for (int i = 0; i < hero.moveCount; ++i) {
            if (hero.Moves[i].isEquipped) {
                printWithDelay(to_string(i + 1) + ". " + hero.Moves[i].name);
            }
        }

        int replaceNumber;
        cin >> replaceNumber;

        if (replaceNumber < 1 || replaceNumber > hero.moveCount || !hero.Moves[replaceNumber - 1].isEquipped) {
            printWithDelay("Invalid move number.");
            return;
        }

        hero.Moves[replaceNumber - 1].isEquipped = false;
        printWithDelay("Unequipped " + hero.Moves[replaceNumber - 1].name + ".");
    }

    hero.Moves[moveNumber - 1].isEquipped = true;
    printWithDelay("Equipped " + hero.Moves[moveNumber - 1].name + ".");
}

void unequipMove(Character& hero, int moveNumber) {
    if (moveNumber < 1 || moveNumber > hero.moveCount) {
        printWithDelay("Invalid move number.");
        return;
    }

    if (!hero.Moves[moveNumber - 1].isEquipped) {
        printWithDelay("Move is not equipped.");
        return;
    }

    hero.Moves[moveNumber - 1].isEquipped = false;
    printWithDelay("Unequipped " + hero.Moves[moveNumber - 1].name + ".");
}

void displayMoves(Character& hero) {
    printWithDelay("=========================");
    printWithDelay("Moves:");
    for (int i = 0; i < hero.moveCount; ++i) {
        printWithDelay(to_string(i + 1) + ". " + hero.Moves[i].name);
        if (hero.Moves[i].damage > 0) {
            printWithDelay("   Deals " + to_string(hero.Moves[i].damage) + " Damage + hero strength");
        }
        if (hero.Moves[i].bheal) {
            printWithDelay("   Heals " + to_string(hero.Moves[i].heal) + " HP");
        }
        if (hero.Moves[i].bAoe) {
            printWithDelay("   Deals " + to_string(hero.Moves[i].Aoe) + " AoE Damage");
        }
        printWithDelay("   Costs " + to_string(hero.Moves[i].manacost) + " Mana");
        if (hero.Moves[i].isEquipped) {
            printWithDelay("   (Equipped)");
        }
        printWithDelay("=========================");
    }

    printWithDelay("Options: ");
    printWithDelay("1. Equip/Replace Move");
    printWithDelay("2. Unequip Move");
    printWithDelay("3. Exit");

    int choice;
    cin >> choice;

    if (choice == 1) {
        equipmove(hero);
    } else if (choice == 2) {
        printWithDelay("Enter the move number to unequip: ");
        int moveNumber;
        cin >> moveNumber;
        unequipMove(hero, moveNumber);
    } else if (choice == 3) {
        return;
    } else {
        printWithDelay("Invalid choice.");
    }
}

void displayMovesequip(Character& hero) {
    
    int i = 0, j = 0;
    while (i < hero.moveCount) {
        if (hero.Moves[i].isEquipped){
            printWithDelay(to_string(j + 1) + ". " + hero.Moves[i].name);
            j++;
        if (hero.Moves[i].damage > 0) {
            printWithDelay("   Deals " + to_string(hero.Moves[i].damage) + " Damage + hero strength");
        }
        if (hero.Moves[i].bheal) {
            printWithDelay("   Heals " + to_string(hero.Moves[i].heal) + " HP");
        }
        if (hero.Moves[i].bAoe) {
            printWithDelay("   Deals " + to_string(hero.Moves[i].Aoe) + " AoE Damage");
        }
        printWithDelay("   Costs " + to_string(hero.Moves[i].manacost) + " Mana");
        }
        i++;
    }
}

// Battle
void battle(Character& hero, Enemy enemies[], int enemyCount) {
    while (hero.health > 0 && enemyCount > 0) {
        cout << "\n";
        printWithDelay(hero.name + " (" + to_string(hero.health) + "/" + to_string(hero.maxHealth) + " HP) (" + to_string(hero.mana) + "/" + to_string(hero.maxmana) + " Mana) ");
        printWithDelay("=========================");
        printWithDelay("Your turn:");
        printWithDelay("1. Act");
        printWithDelay("2. Defend");
        printWithDelay("3. Item");
        printWithDelay("4. Run");
        printWithDelay("=========================");

        int choice;
        cin >> choice;
        cout << "\n";

        switch (choice)
        {
        case 1:
        {
            while (choice >= 0)
            {
                for (int i = 0; i < enemyCount; ++i) {
                    printWithDelay(to_string(i + 1) + ". " + enemies[i].name + " (HP: " + to_string(enemies[i].health) + ")");
                }
                displayMovesequip(hero);
                printWithDelay("Chose your move:");
                int move;
                cin >> move;
                int i = movecounter(hero, move);
                int j = movemanacost(hero,move);
                if (j == 1){
                    if (hero.Moves[i].damage > 0) {
                    printWithDelay("Choose the enemy to attack:");
                        for (int i = 0; i < enemyCount; ++i) {
                            printWithDelay(to_string(i + 1) + ". " + enemies[i].name + " (HP: " + to_string(enemies[i].health) + ")");
                        }
                        int enemyChoice;
                        cin >> enemyChoice;
                        cout << "\n";
                        
                        if (enemyChoice < 1 || enemyChoice > enemyCount) {
                            printWithDelay("Invalid choice, skipping turn.");
                        } else {
                            Enemy& targetEnemy = enemies[enemyChoice - 1];
                            int damage = movedamage(hero, i);
                            targetEnemy.health -= damage;
                            printWithDelay("You dealt " + to_string(damage) + " damage to " + targetEnemy.name + ".");
                            if (targetEnemy.health <= 0) {
                                cout << "\n";
                                printWithDelay(targetEnemy.name + " has been defeated!");
                                hero.gold += targetEnemy.goldDrop;
                                printWithDelay("You gained " + to_string(targetEnemy.goldDrop) + " gold.");
                                hero.exp += targetEnemy.expDrop;
                                printWithDelay("You gained " + to_string(targetEnemy.expDrop) + " experience points.");
                                handleLootDrop(hero, targetEnemy);
                                for (int i = enemyChoice - 1; i < enemyCount - 1; ++i) {
                                    enemies[i] = enemies[i + 1];
                                }
                                enemyCount--;
                            }
                        }
                    } else if (hero.Moves[i].Aoe > 1){ 
                        int damage = moveAoe(hero, i);
                        for (int j = 0; j < enemyCount; ++j) {
                            enemies[j].health -= damage;
                            printWithDelay("You dealt " + to_string(damage) + " damage to " + enemies[j].name + ".");
                            if (enemies[j].health <= 0) {
                                printWithDelay(enemies[j].name + " has been defeated!");
                                hero.gold += enemies[j].goldDrop;
                                printWithDelay("You gained " + to_string(enemies[j].goldDrop) + " gold.");
                                hero.exp += enemies[j].expDrop;
                                printWithDelay("You gained " + to_string(enemies[j].expDrop) + " experience points.");
                                handleLootDrop(hero, enemies[j]);
                                for (int k = j; k < enemyCount - 1; ++k) {
                                    enemies[k] = enemies[k + 1];
                                }
                                enemyCount--;
                                j--; 
                            }
                        
                        }
                    }
                    usemove(hero, i);
                    break;
                } else
                {
                    printWithDelay("Invalid choice pls chose again");
                    continue;
                }
                
            } break;
            
            
        }
        case 2:
            hero.isDefending = true;
            hero.defendTurnsLeft = 2;
            printWithDelay("You are defending and will take 20% less damage for 2 turns.");
            break;
        case 3:
            useConsumables(hero);
            break;
        case 4:
            printWithDelay("You ran away!");
            hero.gold = hero.gold - 10;
            return;
        default:
            printWithDelay("Invalid choice.");
            continue;
    
        }
        // Enemies' turn
        for (int i = 0; i < enemyCount; ++i) {
            Enemy& enemy = enemies[i];
            if (getRandomNumber(1, 100) <= 5 && enemy.specialMoveCount > 0) {
                int healAmount = enemy.heal;
                enemy.health += healAmount;
                if (healAmount > enemy.maxhp)
                {
                    healAmount = enemy.maxhp;
                } 
                printWithDelay(enemy.name + " used " + enemy.specialMoves[0].name + " and healed " + to_string(healAmount) + " HP.");
                
            } else {
                int damage = enemydmg(enemy, hero);
                hero.health -= damage;
                printWithDelay(enemy.name + " dealt " + to_string(damage) + " damage to you.");
            }
        }

        if (hero.health <= 0) {
            printWithDelay("You were defeated.");
            return;
        }
    }

    if (hero.health > 0) {
        printWithDelay("You defeated all enemies!");
        hero.hasCompletedAdventure = true;
    }
}

// Menus
//Ingame
void displayInGameMenu(Character& hero) {
    maxmanacheck(hero);
    levelupcheck(hero);
    hero.health = hero.maxHealth;
    cout << "\n";
    printWithDelay(hero.name + " (" + to_string(hero.health) + "/" + to_string(hero.maxHealth) + " HP)");
    printWithDelay("(" + to_string(hero.exp) + "/" + to_string(hero.maxExp) + " Exp)");
    printWithDelay("(" + to_string(hero.gold) + " gold)");

    printWithDelay("=========================");
    printWithDelay("      In-Game Menu       ");
    printWithDelay("=========================");
    printWithDelay("1. Adventure");
    printWithDelay("2. Inventory");
    printWithDelay("3. Moves");
    printWithDelay("4. Map");
    printWithDelay("5. Stats");
    printWithDelay("6. Save");
    if (hero.currentMap == "Town") {
        printWithDelay("7. Shop"); 
    }
    printWithDelay("0. Main Menu");
    printWithDelay("=========================");
    printWithDelay("Choose an option: ");
}

// Adventure and adding enemy
void townenemy(Character& hero){
    Enemy PotentialForestEnemies[4];
        addEnemy(PotentialForestEnemies[0], "Shroom", 0, 20, "Heal", 1, "Mushroom", 100, "Rock", 25, 90, 5, 5);
        addEnemy(PotentialForestEnemies[1], "Vaggie", 1, 15, "Heal", 5, "Herbs", 100, "Rock", 50, 50, 5, 5);
        addEnemy(PotentialForestEnemies[2], "Vaggies army", 3, 15, "Heal", 1, "Herbs", 75, "Stick", 75, 45, 10, 10);
        addEnemy(PotentialForestEnemies[3], "Old tree", 5, 25, "heal", 10, "Mushroom", 50, "Stick", 100, 15, 15, 15);

        Enemy encounteredEnemies[3];
        int encounterCount;
        generateRandomEnemies(PotentialForestEnemies, 4, encounteredEnemies, encounterCount);

        battle(hero, encounteredEnemies, encounterCount);
}

void sewerenemy(Character& hero){
    Enemy PotentialForestEnemies[4];
        addEnemy(PotentialForestEnemies[0], "Lushy", 2, 20, "Heal", 1, "Stick", 100, "Herbs", 25, 90, 10, 10);
        addEnemy(PotentialForestEnemies[1], "Lush roach", 3, 15, "Heal", 2, "Herbs", 75, "Rock", 50, 70, 10, 15);
        addEnemy(PotentialForestEnemies[2], "Rat", 5, 15, "Heal", 1, "Rock", 10, "Iron", 10, 45, 20, 20);
        addEnemy(PotentialForestEnemies[3], "Rat queen", 7, 25, "heal", 10, "Iron", 20, "Rock", 70, 15, 30, 30);

        Enemy encounteredEnemies[3];
        int encounterCount;
        generateRandomEnemies(PotentialForestEnemies, 4, encounteredEnemies, encounterCount);

        battle(hero, encounteredEnemies, encounterCount);
}

void forestenemy(Character& hero){
    Enemy PotentialForestEnemies[4];
        addEnemy(PotentialForestEnemies[0], "Mossy", 4, 30, "Heal", 1, "Stick", 100, "Herbs", 25, 90, 15, 15);
        addEnemy(PotentialForestEnemies[1], "Crawler", 6, 25, "Heal", 5, "Stick", 75, "Rock", 50, 70, 15, 25);
        addEnemy(PotentialForestEnemies[2], "Mossy knight", 10, 30, "Heal", 1, "Iron", 30, "Herbs", 100, 45, 40, 40);
        addEnemy(PotentialForestEnemies[3], "Mossy brute", 15, 40, "heal", 10, "Iron", 50, "Herbs", 75, 15, 45, 45);

        Enemy encounteredEnemies[3];
        int encounterCount;
        generateRandomEnemies(PotentialForestEnemies, 4, encounteredEnemies, encounterCount);

        battle(hero, encounteredEnemies, encounterCount);
}

void caveenemy(Character& hero){
    Enemy PotentialForestEnemies[4];
        addEnemy(PotentialForestEnemies[0], "Rocky", 6, 40, "Heal", 1, "Rock", 100, "Iron", 25, 90, 15, 15);
        addEnemy(PotentialForestEnemies[1], "Cave Creeper", 8, 35, "Heal", 5, "Rock", 75, "Iron", 50, 70, 15, 25);
        addEnemy(PotentialForestEnemies[2], "Shadowy Figure", 12, 40, "Heal", 1, "Gems", 30, "Iron", 100, 45, 40, 40);
        addEnemy(PotentialForestEnemies[3], "Rocky Brute", 18, 45, "heal", 10, "Gems", 50, "Iron", 75, 15, 45, 45);

        Enemy encounteredEnemies[3];
        int encounterCount;
        generateRandomEnemies(PotentialForestEnemies, 4, encounteredEnemies, encounterCount);

        battle(hero, encounteredEnemies, encounterCount);
}

void gemcaveenemy(Character& hero){
    Enemy PotentialForestEnemies[4];
        addEnemy(PotentialForestEnemies[0], "Gemy", 10, 40, "Heal", 1, "Iron", 100, "Gems", 25, 90, 15, 15);
        addEnemy(PotentialForestEnemies[1], "Fairy Minner", 15, 30, "Heal", 5, "Iron", 75, "Gems", 50, 70, 15, 25);
        addEnemy(PotentialForestEnemies[2], "Ruby Turtle", 6, 60, "Heal", 1, "Diamonds", 30, "Gems", 100, 45, 40, 40);
        addEnemy(PotentialForestEnemies[3], "Diamond Guardian", 18, 45, "heal", 10, "Diamonds", 50, "Gems", 75, 15, 45, 45);

        Enemy encounteredEnemies[3];
        int encounterCount;
        generateRandomEnemies(PotentialForestEnemies, 4, encounteredEnemies, encounterCount);

        battle(hero, encounteredEnemies, encounterCount);
}

void startAdventure(Character& hero) {
    if (hero.currentMap == "Town")
    {
        townenemy(hero);
    } else if (hero.currentMap == "Sewer")
    {
        sewerenemy(hero);
    }
    else if (hero.currentMap == "Forest")
    {
        forestenemy(hero);
    } else if (hero.currentMap == "Cave")
    {
        caveenemy(hero);
    } else if (hero.currentMap == "Gem Cave")
    {
        gemcaveenemy(hero);
    }
     
}

//showing ur stats
void Showstats (Character& hero){
        printWithDelay("=========================");
        printWithDelay("           Stats         ");
        printWithDelay("=========================");
        printWithDelay("Name: " + hero.name);
        printWithDelay("Level: " + to_string(hero.level));
        printWithDelay("Experience: " + to_string(hero.exp)+ "/" + to_string(hero.maxExp));
        printWithDelay("Mana: " + to_string(hero.mana)+ "/" + to_string(hero.maxmana));
        printWithDelay("Strength: " + to_string(hero.strength));
        printWithDelay("Health: " + to_string(hero.health) + "/" + to_string(hero.maxHealth));
        printWithDelay("=========================");
        cout << "\n";
}

// handleing InGameMainMenu
void handleInGameMenu(Character& hero) {
    int choice;

    while (true) {
        displayInGameMenu(hero);
        cin >> choice;

        switch (choice) {
            case 1:
                startAdventure(hero);
                break;
            case 2:
                displayInventory(hero);
                break;
            case 3:
                displayMoves(hero);
                break;
            case 4:
                displayMapOptions(hero, maps);
                break;
            case 5:
                Showstats(hero);
                break;
            case 6:
                Saving(hero);
                break;
            case 7:
                if (hero.currentMap == "Town") {
                    displayShopMenu(hero);
                } else {
                    printWithDelay("You can only access the shop in the Town.");
                }
                break;
            case 0:
                printWithDelay("Returning to main menu...");
                return;
            default:
                printWithDelay("Invalid choice, please try again.");
                break;
        }
    }
}

// Main menu display la
void displayMainMenu() {
    printWithDelay("=========================");
    printWithDelay("     RPG Game Menu       ");
    printWithDelay("=========================");
    printWithDelay("1. Start Game");
    printWithDelay("2. Load Game");
    printWithDelay("3. Save Game"); 
    printWithDelay("4. Exit");
    printWithDelay("=========================");
    printWithDelay("Choose an option: ");
}

// Function to handle main menu options
void handleMainMenu() {
    Character hero;
    int choice;
    hero.currentMap = "Town";

    while (true) {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                printWithDelay("Starting new game...");
                hero.name = getCharacterName();
                hero.defend = 1;
                hero.queueSize = 0;
                hero.level = 1;
                hero.exp = 0;
                hero.maxExp = 0;
                hero.magic = 0;
                hero.maxmana = 0;
                hero.mana = 0;
                hero.strength = 1;
                hero.health = 10;
                hero.maxHealth = 10;
                hero.gold = 10;
                hero.itemCount = 0;
                hero.hasCompletedAdventure = false;
                hero.isDefending = false;
                hero.defendTurnsLeft = 0;
                hero.x = 0;
                hero.y = 0;
                hero.inventory[0] = new Item(chooseStartingWeapon());
                hero.itemCount = 1;
                hero.points = 0;
                hero.moveCount = 0;
                {
                    Item snowpie = {"Snow Pie", 0, 0, 0, 0, 5, false,  true, 10};
                    Item cookie = {"cookie", 0, 0, 0, 0, 5, false,  true, 5};
                    addItemToInventory(hero, snowpie);
                    addItemToInventory(hero, cookie);
                }
                addMoves(hero, "Smack", 1, 1, 0, 0, 0, false, false, true);
                addMoves(hero, "Heal", 2, 0, 5, 5, 0, true, false, true);
                addMoves(hero, "Slash", 3, 0, 5, 0, 2, false, true, true);
                addMoves(hero, "Stab", 4, 3, 3, 0, 0, false, false, true);
                addMoves(hero, "Great Slash", 5, 0, 15, 0, 6, false, true, false);
                addMoves(hero, "Great Heal", 6, 0, 15, 15, 0, true, false, false);
                addMoves(hero, "Fury Stabs", 7, 5, 15, 0, 0, false, false, false);
                addMoves(hero, "Leach Stabs", 8, 5, 25, 10, 0, true, false, false);
                addMoves(hero, "Greatest Heal", 9, 0, 30, 25, 0, true, false, false);
                handleInGameMenu(hero);
                break;
            case 2:
                Load(hero);
                handleInGameMenu(hero);
                break;
            case 3:
                Saving(hero);
                break;
            case 4:
                printWithDelay("Exiting game. Goodbye!");
                for (int i = 0; i < hero.itemCount; ++i) {
                    delete hero.inventory[i];
                }
                return;
            default:
                printWithDelay("Invalid choice. Please try again.");
                break;
        }
    }
}

int main() {
    
    handleMainMenu();
    return 0;
}