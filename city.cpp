#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct City {
    string index, name;
};

vector<City> loadCities() {
    vector<City> cities;
    ifstream file("cities.txt");
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string index, name;
        getline(ss, index, ',');
        getline(ss, name, ',');
        cities.push_back({index, name});
    }
    return cities;
}

void saveCities(const vector<City>& cities) {
    ofstream file("cities.txt");
    file << "Index,City_Name\n";
    for (const auto& c : cities) {
        file << c.index << "," << c.name << "\n";
    }
    file.close();
}

bool cityExists(const vector<City>& cities, const string& name) {
    return any_of(cities.begin(), cities.end(), [&](const City& c) {
        return c.name == name;
    });
}

void viewCities() {
    auto cities = loadCities();
    if (cities.empty()) {
        cout << "No cities found.\n";
        return;
    }

    cout << left << setw(10) << "Index" << setw(20) << "City_Name\n";
    cout << string(30, '-') << "\n";
    for (const auto& c : cities) {
        cout << left << setw(10) << c.index << setw(20) << c.name << "\n";
    }
}

void addCity() {
    vector<City> cities = loadCities();
    int count;
    cout << "Enter number of new cities to add: ";
    cin >> count;
    cin.ignore(); // Clear newline character

    for (int i = 0; i < count; i++) {
        string name;
        cout << "Enter City " << i + 1 << " Name: ";
        getline(cin, name);

        if (!cityExists(cities, name)) {
            int index = cities.size() + 1;
            cities.push_back({to_string(index), name});
        } else {
            cout << "City already exists!\n";
        }
    }
    saveCities(cities);
    cout << "Cities added successfully!\n";
}
void searchCityByIndex() {
    vector<City> cities = loadCities();
    string index;
    cout << "Enter City Index to search: ";
    cin >> index;

    for (const auto& c : cities) {
        if (c.index == index) {
            cout << "City found: " << c.name << "\n";
            return;
        }
    }

    cout << "City with index " << index << " not found.\n";
}

void updateCity() {
    vector<City> cities = loadCities();
    string index;
    cout << "Enter City Index to update: ";
    cin >> index;
    cin.ignore();

    for (auto& c : cities) {
        if (c.index == index) {
            string oldName = c.name;
            cout << "Current Name: " << oldName << "\n";
            cout << "Enter new name: ";
            getline(cin, c.name);

            saveCities(cities);

            // Update roads.txt with the new city name
            ifstream roadFile("roads.txt");
            ofstream tempFile("temp_roads.txt");

            string line;
            getline(roadFile, line); // header
            tempFile << "City1,City2,Budget\n";

            while (getline(roadFile, line)) {
                stringstream ss(line);
                string city1, city2, budget;
                getline(ss, city1, ',');
                getline(ss, city2, ',');
                getline(ss, budget, ',');

                if (city1 == oldName) city1 = c.name;
                if (city2 == oldName) city2 = c.name;

                tempFile << city1 << "," << city2 << "," << budget << "\n";
            }

            roadFile.close();
            tempFile.close();

            remove("roads.txt");
            rename("temp_roads.txt", "roads.txt");

            cout << "City and related roads updated successfully!\n";
            return;
        }
    }

    cout << "City with index " << index << " not found.\n";
}


void deleteCity() {
    vector<City> cities = loadCities();
    string index;
    cout << "Enter City Index to delete: ";
    cin >> index;

    auto it = remove_if(cities.begin(), cities.end(), [&](const City& c) {
        return c.index == index;
    });

    if (it == cities.end()) {
        cout << "City with index " << index << " not found.\n";
        return;
    }

    string deletedCityName;
    for (const auto& c : cities) {
        if (c.index == index) {
            deletedCityName = c.name;
            break;
        }
    }

    cities.erase(it, cities.end());

    // Reassign new indexes after deletion
    for (int i = 0; i < cities.size(); ++i) {
        cities[i].index = to_string(i + 1);
    }

    saveCities(cities);

    // Clean up roads associated with the deleted city
    ifstream roadFile("roads.txt");
    ofstream tempFile("temp_roads.txt");

    string line;
    getline(roadFile, line); // header
    tempFile << "City1,City2,Budget\n";

    while (getline(roadFile, line)) {
        stringstream ss(line);
        string city1, city2, budget;
        getline(ss, city1, ',');
        getline(ss, city2, ',');
        getline(ss, budget, ',');

        if (city1 != deletedCityName && city2 != deletedCityName) {
            tempFile << city1 << "," << city2 << "," << budget << "\n";
        }
    }

    roadFile.close();
    tempFile.close();

    remove("roads.txt");
    rename("temp_roads.txt", "roads.txt");

    cout << "City and all associated roads deleted successfully!\n";
}
