#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype> // for tolower()
#include "city.cpp"

using namespace std;

struct Road {
    string from, to;
    double budget;
};

const int MAX_CITIES = 100;
int roadMatrix[MAX_CITIES][MAX_CITIES] = {0};
float budgetMatrix[MAX_CITIES][MAX_CITIES] = {0.0f};

// Case-insensitive string comparison
bool iequals(const string& a, const string& b) {
    return equal(a.begin(), a.end(), b.begin(), b.end(),
        [](char a, char b) { return tolower(a) == tolower(b); });
}

// Find city index with case-insensitive comparison
int getCityIndex(const vector<City>& cities, const string& name) {
    for (int i = 0; i < cities.size(); ++i) {
        if (iequals(cities[i].name, name)) {
            return i + 1; // 1-based index
        }
    }
    return -1;
}

vector<Road> loadRoads() {
    vector<Road> roads;
    ifstream file("roads.txt");
    if (!file.is_open()) return roads;

    // Reset matrices
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            roadMatrix[i][j] = 0;
            budgetMatrix[i][j] = 0.0f;
        }
    }

    string line;
    getline(file, line); // Skip header

    vector<City> cities = loadCities();

    while (getline(file, line)) {
        stringstream ss(line);
        string indexStr, roadName, budgetStr;

        getline(ss, indexStr, ',');
        getline(ss, roadName, ',');
        getline(ss, budgetStr, ',');

        size_t dashPos = roadName.find('-');
        if (dashPos == string::npos) continue;

        Road r;
        r.from = roadName.substr(0, dashPos);
        r.to = roadName.substr(dashPos + 1);
        r.budget = stof(budgetStr);

        // Update matrices
        int fromIdx = getCityIndex(cities, r.from);
        int toIdx = getCityIndex(cities, r.to);
        
        if (fromIdx != -1 && toIdx != -1) {
            roadMatrix[fromIdx][toIdx] = 1;
            roadMatrix[toIdx][fromIdx] = 1;
            budgetMatrix[fromIdx][toIdx] = r.budget;
            budgetMatrix[toIdx][fromIdx] = r.budget;
        }

        roads.push_back(r);
    }
    return roads;
}

void saveRoads(const vector<Road>& roads) {
    ofstream file("roads.txt");
    file << "nbr,Road,Budget\n";
    for (int i = 0; i < roads.size(); ++i) {
        file << i + 1 << "," 
             << roads[i].from << "-" << roads[i].to << "," 
             << fixed << setprecision(2) << roads[i].budget << "\n";
    }
}

void addRoad() {
    vector<City> cities = loadCities();
    string from, to;
    
    cout << "Enter name for first city: ";
    getline(cin >> ws, from); // ws to skip whitespace
    cout << "Enter name for second city: ";
    getline(cin, to);

    if (!cityExists(cities, from) || !cityExists(cities, to)) {
        cout << "One or both cities not found.\n";
        return;
    }

    if (iequals(from, to)) {
        cout << "Cannot connect a city to itself.\n";
        return;
    }

    vector<Road> roads = loadRoads();
    int fromIdx = getCityIndex(cities, from);
    int toIdx = getCityIndex(cities, to);

    // Check if road exists
    for (const auto& r : roads) {
        if ((iequals(r.from, from) && iequals(r.to, to)) || 
            (iequals(r.from, to) && iequals(r.to, from))) {
            cout << "Road already exists.\n";
            return;
        }
    }

    // Add to matrices and vector
    roadMatrix[fromIdx][toIdx] = roadMatrix[toIdx][fromIdx] = 1;
    roads.push_back({from, to, 0.0f});
    saveRoads(roads);

    cout << "Road added between " << from << " and " << to << ".\n";
}

void addBudget() {
    vector<City> cities = loadCities();
    vector<Road> roads = loadRoads();
    
    if (roads.empty()) {
        cout << "No roads exist. Add roads first.\n";
        return;
    }

    string from, to;
    cout << "Enter first city name: ";
    getline(cin >> ws, from);
    cout << "Enter second city name: ";
    getline(cin, to);

    int fromIdx = getCityIndex(cities, from);
    int toIdx = getCityIndex(cities, to);

    if (fromIdx == -1 || toIdx == -1) {
        cout << "One or both cities not found.\n";
        return;
    }

    bool found = false;
    for (auto& r : roads) {
        if ((iequals(r.from, from) && iequals(r.to, to)) || 
            (iequals(r.from, to) && iequals(r.to, from))) {
            
            double budget;
            cout << "Enter budget amount (RWF): ";
            while (!(cin >> budget)) {
                cout << "Invalid input. Enter a number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            r.budget = budget;
            budgetMatrix[fromIdx][toIdx] = budgetMatrix[toIdx][fromIdx] = budget;
            found = true;
            break;
        }
    }

    if (found) {
        saveRoads(roads);
        cout << "Budget updated successfully.\n";
    } else {
        cout << "No road found between these cities.\n";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void displayRoads() {
    vector<City> cities = loadCities();
    vector<Road> roads = loadRoads();

    if (roads.empty()) {
        cout << "\nNo roads found in the system. Please add some roads first.\n";
        return;
    }

    // Update matrices just in case
    int size = cities.size();
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            roadMatrix[i][j] = 0;
            budgetMatrix[i][j] = 0.0f;
        }
    }

    for (const auto& r : roads) {
        int fromIdx = getCityIndex(cities, r.from);
        int toIdx = getCityIndex(cities, r.to);
        if (fromIdx != -1 && toIdx != -1) {
            roadMatrix[fromIdx][toIdx] = 1;
            roadMatrix[toIdx][fromIdx] = 1;
            budgetMatrix[fromIdx][toIdx] = budgetMatrix[toIdx][fromIdx] = r.budget;
        }
    }

    // Show list of cities
    cout << "\nList of Cities:\n";
    for (int i = 0; i < cities.size(); i++) {
        cout << i + 1 << ". " << cities[i].name << "\n";
    }

    // Show matrix
    cout << "\nAdjacency Matrix (1=Road exists, 0=None):\n";
    cout << "     ";
    for (int i = 1; i <= size && i <= 15; i++) {
        cout << setw(3) << i;
    }
    cout << "\n";

    for (int i = 1; i <= size && i <= 15; i++) {
        cout << setw(3) << i << ": ";
        for (int j = 1; j <= size && j <= 15; j++) {
            cout << setw(3) << roadMatrix[i][j];
        }
        cout << "\n";
    }
}


void displayAdjacencyMatrices() {
    vector<City> cities = loadCities();
    if (cities.empty()) {
        cout << "No cities loaded.\n";
        return;
    }

    // Display cities first
    cout << "List of Cities:\n";
    for (int i = 0; i < (int)cities.size() && i < 15; i++) {
        cout << setw(3) << (i + 1) << ". " << cities[i].name << "\n";
    }

    vector<Road> roads = loadRoads(); // Load roads to populate matrices

    int size = cities.size();

    if (roads.empty()) {
        cout << "No roads loaded.\n";
        return;
    }
    
    cout << "\nRoad Connections Matrix:\n";
    cout << "     ";
    for (int i = 1; i <= size && i <= 15; i++) {
        cout << setw(3) << i;
    }
    cout << "\n";
    
    for (int i = 1; i <= size && i <= 15; i++) {
        cout << setw(3) << i << ": ";
        for (int j = 1; j <= size && j <= 15; j++) {
            cout << setw(3) << roadMatrix[i][j];
        }
        cout << "\n";
    }
    
    cout << "\nBudget Matrix (RWF):\n";
    cout << "     ";
    for (int i = 1; i <= size && i <= 15; i++) {
        cout << setw(10) << i;
    }
    cout << "\n";
    
    for (int i = 1; i <= size && i <= 15; i++) {
        cout << setw(3) << i << ": ";
        for (int j = 1; j <= size && j <= 15; j++) {
            cout << setw(10) << fixed << setprecision(2) << budgetMatrix[i][j];
        }
        cout << "\n";
    }
}
