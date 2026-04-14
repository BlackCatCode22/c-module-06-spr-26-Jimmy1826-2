#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;

// ---------- GLOBALS ----------
map<string, int> speciesCount;
map<string, queue<string>> namePool;

// ---------- BASE CLASS ----------
class Animal {
protected:
    string name, species, sex, color, origin;
    int age, weight;
    string arrivalDate, birthDate, uniqueID;

public:
    Animal(string sp, int a, string sx, string col, int wt, string org)
        : species(sp), age(a), sex(sx), color(col), weight(wt), origin(org) {}

    void setName(string n) { name = n; }
    void setArrivalDate(string d) { arrivalDate = d; }
    void setBirthDate(string d) { birthDate = d; }
    void setUniqueID(string id) { uniqueID = id; }

    string getSpecies() { return species; }

    virtual void print(ofstream &out) {
        out << uniqueID << "; "
            << name << "; birth date " << birthDate << "; "
            << color << " color; "
            << sex << "; "
            << weight << " pounds; "
            << "from " << origin << "; "
            << "arrived " << arrivalDate << endl;
    }
};

// ---------- DERIVED CLASSES ----------
class Hyena : public Animal {
public:
    Hyena(int a, string sx, string col, int wt, string org)
        : Animal("hyena", a, sx, col, wt, org) {}
};

class Lion : public Animal {
public:
    Lion(int a, string sx, string col, int wt, string org)
        : Animal("lion", a, sx, col, wt, org) {}
};

class Tiger : public Animal {
public:
    Tiger(int a, string sx, string col, int wt, string org)
        : Animal("tiger", a, sx, col, wt, org) {}
};

class Bear : public Animal {
public:
    Bear(int a, string sx, string col, int wt, string org)
        : Animal("bear", a, sx, col, wt, org) {}
};

// ---------- FUNCTIONS ----------

// Generate Birthday
string genBirthDay(int age, string season) {
    int currentYear = 2024;
    int birthYear = currentYear - age;

    if (season == "spring") return to_string(birthYear) + "-03-15";
    if (season == "summer") return to_string(birthYear) + "-06-15";
    if (season == "fall")   return to_string(birthYear) + "-09-15";
    if (season == "winter") return to_string(birthYear) + "-12-15";

    return to_string(birthYear) + "-01-01";
}

// Generate Unique ID
string genUniqueID(string species) {
    speciesCount[species]++;
    string prefix = species.substr(0, 2);

    prefix[0] = toupper(prefix[0]);
    prefix[1] = tolower(prefix[1]);

    int num = speciesCount[species];

    string id = prefix + (num < 10 ? "0" : "") + to_string(num);
    return id;
}

// Load names
void loadNames() {
    ifstream file("animalNames.txt");
    if (!file) throw runtime_error("Error opening animalNames.txt");

    string line, species;
    while (getline(file, line)) {
        if (line == "Hyena") species = "hyena";
        else if (line == "Lion") species = "lion";
        else if (line == "Tiger") species = "tiger";
        else if (line == "Bear") species = "bear";
        else {
            namePool[species].push(line);
        }
    }
}

// Assign name
string getName(string species) {
    if (namePool[species].empty()) {
        throw runtime_error("No names left for " + species);
    }
    string name = namePool[species].front();
    namePool[species].pop();
    return name;
}

// Parse animal line
Animal* parseAnimal(string line) {
    int age, weight;
    string sex, species, color, origin, season;

    // AGE
    age = stoi(line.substr(0, line.find(" ")));

    // SEX
    if (line.find("female") != string::npos) sex = "female";
    else sex = "male";

    // SPECIES
    if (line.find("hyena") != string::npos) species = "hyena";
    else if (line.find("lion") != string::npos) species = "lion";
    else if (line.find("tiger") != string::npos) species = "tiger";
    else species = "bear";

    // SEASON
    if (line.find("spring") != string::npos) season = "spring";
    else if (line.find("summer") != string::npos) season = "summer";
    else if (line.find("fall") != string::npos) season = "fall";
    else if (line.find("winter") != string::npos) season = "winter";

    // COLOR
    size_t colorPos = line.find(", ", line.find("color"));
    size_t colorStart = line.rfind(" ", colorPos - 1);
    color = line.substr(colorStart + 1, colorPos - colorStart - 1);

    // WEIGHT
    size_t weightPos = line.find(" pounds");
    size_t weightStart = line.rfind(" ", weightPos - 1);
    weight = stoi(line.substr(weightStart + 1, weightPos - weightStart - 1));

    // ORIGIN
    size_t fromPos = line.find("from ");
    origin = line.substr(fromPos + 5);

    // Create object
    Animal* animal = nullptr;
    if (species == "hyena") animal = new Hyena(age, sex, color, weight, origin);
    else if (species == "lion") animal = new Lion(age, sex, color, weight, origin);
    else if (species == "tiger") animal = new Tiger(age, sex, color, weight, origin);
    else animal = new Bear(age, sex, color, weight, origin);

    // Set attributes
    animal->setName(getName(species));
    animal->setUniqueID(genUniqueID(species));
    animal->setBirthDate(genBirthDay(age, season));
    animal->setArrivalDate("2024-03-05");

    return animal;
}

// ---------- MAIN ----------
int main() {
    try {
        loadNames();

        ifstream file("arrivingAnimals.txt");
        if (!file) throw runtime_error("Error opening arrivingAnimals.txt");

        vector<Animal*> hyenas, lions, tigers, bears;

        string line;
        while (getline(file, line)) {
            Animal* a = parseAnimal(line);

            if (a->getSpecies() == "hyena") hyenas.push_back(a);
            else if (a->getSpecies() == "lion") lions.push_back(a);
            else if (a->getSpecies() == "tiger") tigers.push_back(a);
            else bears.push_back(a);
        }

        ofstream out("zooPopulation.txt");

        auto printGroup = [&](string title, vector<Animal*>& group) {
            out << title << " Habitat:\n";
            for (auto a : group) a->print(out);
            out << endl;
        };

        printGroup("Hyena", hyenas);
        printGroup("Lion", lions);
        printGroup("Tiger", tigers);
        printGroup("Bear", bears);

        cout << "zooPopulation.txt generated successfully!\n";

    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}