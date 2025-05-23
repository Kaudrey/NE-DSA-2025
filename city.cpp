#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

struct City {
    string index, name;
};

vector<City> loadCities() {
    vector<City> cities;
    ifstream file("cities.txt");
    if (!file.is_open()) return cities;

    string line;
    // Skip header and underline
    getline(file, line); // Header
    getline(file, line); // Underline

    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string index, name;
        ss >> index >> name;
        
        // Handle multi-word city names
        string rest;
        while (ss >> rest) {
            name += " " + rest;
        }
        
        cities.push_back({index, name});
    }
    return cities;
}

void saveCities(const vector<City>& cities) {
    ofstream file("cities.txt");
    
    // Write header with underline
    file << "Index  City Name\n";
    file << "-----  ---------\n";
    
    // Write data with proper alignment
    for (const auto& c : cities) {
        file << left << setw(6) << c.index << setw(30) << c.name << "\n";
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
    
    // Input validation for number of cities
    while (true) {
        cout << "Enter number of new cities to add: ";
        if (cin >> count && count > 0) {
            break;
        }
        cout << "Invalid input. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(); // Clear newline character

    for (int i = 0; i < count; i++) {
        string name;
        bool validName = false;
        
        while (!validName) {
            cout << "Enter City " << i + 1 << " Name: ";
            getline(cin, name);
            
            // Trim whitespace
            name.erase(0, name.find_first_not_of(" \t\r\n"));
            name.erase(name.find_last_not_of(" \t\r\n") + 1);
            
            if (name.empty()) {
                cout << "City name cannot be empty. Please try again.\n";
                continue;
            }
            
            // Check if name contains only valid characters
            validName = all_of(name.begin(), name.end(), [](char c) {
                return isalnum(c) || c == ' ' || c == '-' || c == '_';
            });
            
            if (!validName) {
                cout << "City name contains invalid characters. Use only letters, numbers, spaces, hyphens, and underscores.\n";
                continue;
            }
            
            if (cityExists(cities, name)) {
                cout << "City already exists! Please enter a different name.\n";
                validName = false;
            }
        }

        int index = cities.size() + 1;
        cities.push_back({to_string(index), name});
    }
    saveCities(cities);
    cout << "Cities added successfully!\n";
}

void searchCityByIndex() {
    vector<City> cities = loadCities();
    string index;
    bool found = false;
    
    while (!found) {
        cout << "Enter City Index to search: ";
        if (!(cin >> index)) {
            cout << "Invalid input. Please enter a valid index.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        // Validate index format
        if (!all_of(index.begin(), index.end(), ::isdigit)) {
            cout << "Invalid index format. Please enter a numeric index.\n";
            continue;
        }

        for (const auto& c : cities) {
            if (c.index == index) {
                cout << "City found: " << c.name << "\n";
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "City with index " << index << " not found. Try again? (y/n): ";
            char retry;
            cin >> retry;
            if (tolower(retry) != 'y') {
                break;
            }
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void updateCity() {
    vector<City> cities = loadCities();
    string index;
    bool validInput = false;
    
    while (!validInput) {
        cout << "Enter City Index to update: ";
        if (!(cin >> index)) {
            cout << "Invalid input. Please enter a valid index.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        // Validate index format
        if (!all_of(index.begin(), index.end(), ::isdigit)) {
            cout << "Invalid index format. Please enter a numeric index.\n";
            continue;
        }

        bool cityFound = false;
        for (auto& c : cities) {
            if (c.index == index) {
                cityFound = true;
                string oldName = c.name;
                cout << "Current Name: " << oldName << "\n";
                
                string newName;
                bool validName = false;
                while (!validName) {
                    cout << "Enter new name: ";
                    getline(cin >> ws, newName);
                    
                    // Trim whitespace
                    newName.erase(0, newName.find_first_not_of(" \t\r\n"));
                    newName.erase(newName.find_last_not_of(" \t\r\n") + 1);
                    
                    if (newName.empty()) {
                        cout << "City name cannot be empty. Please try again.\n";
                        continue;
                    }
                    
                    // Check if name contains only valid characters
                    validName = all_of(newName.begin(), newName.end(), [](char c) {
                        return isalnum(c) || c == ' ' || c == '-' || c == '_';
                    });
                    
                    if (!validName) {
                        cout << "City name contains invalid characters. Use only letters, numbers, spaces, hyphens, and underscores.\n";
                        continue;
                    }
                    
                    if (newName != oldName && cityExists(cities, newName)) {
                        cout << "City name already exists! Please enter a different name.\n";
                        validName = false;
                    }
                }
                
                c.name = newName;
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
                validInput = true;
                break;
            }
        }

        if (!cityFound) {
            cout << "City with index " << index << " not found. Try again? (y/n): ";
            char retry;
            cin >> retry;
            if (tolower(retry) != 'y') {
                break;
            }
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
