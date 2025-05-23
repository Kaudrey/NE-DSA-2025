// main.cpp
#include <iostream>
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

int main() {
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
        cin >> choice;

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
            default: cout << "Invalid option!\n";
        }
    } while (choice != 9);

    return 0;
}