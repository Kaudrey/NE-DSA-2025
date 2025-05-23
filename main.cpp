// main.cpp
#include <iostream>
#include <limits>
#include "road.cpp"
using namespace std;


void addCity();
void viewCities();
void updateCity();
void searchCityByIndex();

void addRoad();
void addBudget();
void displayRoads();
void displayAdjacencyMatrices();

void convertToNewFormat() {
    // Convert cities.txt
    ifstream cityFile("cities.txt");
    if (cityFile.is_open()) {
        vector<City> cities = loadCities();
        saveCities(cities); // This will save in the new format
        cityFile.close();
    }

    // Convert roads.txt
    ifstream roadFile("roads.txt");
    if (roadFile.is_open()) {
        vector<Road> roads = loadRoads();
        saveRoads(roads); // This will save in the new format
        roadFile.close();
    }
}

int main() {
    // Convert existing data to new format at startup
    convertToNewFormat();
    
    int choice;
    do {
        cout << "\n--- Transport Infrastructure Menu ---\n";
        cout << "1. Add new city(cities)\n";
        cout << "2. Add roads between cities\n";
        cout << "3. Add budget for roads\n";
        cout << "4. Edit city\n";
        cout << "5. Search for a city using its index\n";
        cout << "6. Display cities\n";
        cout << "7. Display roads\n";
        cout << "8. Display adjacency matrices\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        
        // Robust input validation
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 9: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        // Clear any remaining input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: addCity(); break;
            case 2: addRoad(); break;
            case 3: addBudget(); break;
            case 4: updateCity(); break;
            case 5: searchCityByIndex(); break;
            case 6: viewCities(); break;
            case 7: displayRoads(); break;
            case 8: displayAdjacencyMatrices(); break;
            case 9: cout << "Exiting...\n"; break;
            default: cout << "Invalid option! Please enter a number between 1 and 9.\n";
        }
    } while (choice != 9);

    return 0;
}