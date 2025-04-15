#include "MainMenu.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

MainMenu::MainMenu() {
    try {
        catalog.loadData(dataFile);
        catalog.buildIndices();
        std::cout << "Successfully loaded " << dataFile << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

void MainMenu::displayMainMenu() {
    std::cout << "\n==== Exoplanet Catalog ====\n"
              << "1. Sort and Export Data\n"
              << "2. Find Extreme Planets\n"
              << "3. Display All Planets\n"
              << "4. Search Planet\n"
              << "5. Show Habitable Planets\n"
              << "6. Show Planet Type Distribution\n"
              << "7. Exit\n"
              << "=========================\n"
              << "Enter choice: ";
}

void MainMenu::run() {
    int choice;
    do {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1: sortSubMenu(); break;
            case 2: extremeFinderSubMenu(); break;
            case 3: displayAllPlanets(); break;
            case 4: searchPlanet(); break;
            case 5: displayHabitablePlanets(); break;
            case 6: catalog.printPlanetTypeAnalysis(); break;
            case 7: break; // Exit
            default: std::cout << "Invalid choice\n";
        }
    } while (choice != 7);
}

void MainMenu::sortSubMenu() {
    int choice;
    do {
        std::cout << "\n=== Sort and Export ===\n"
                  << "1. By Period (saves to CSV)\n"
                  << "2. By Radius (saves to CSV)\n"
                  << "3. By Temperature (saves to CSV)\n"
                  << "4. Back\n"
                  << "======================\n"
                  << "Enter choice: ";

        std::cin >> choice;

        switch(choice) {
            case 1:
                catalog.sortByPeriod();
                catalog.saveResults("sorted_by_period.csv");
                std::cout << "Saved to sorted_by_period.csv\n";
                break;
            case 2:
                catalog.sortByRadius();
                catalog.saveResults("sorted_by_radius.csv");
                std::cout << "Saved to sorted_by_radius.csv\n";
                break;
            case 3:
                catalog.sortByTemperature();
                catalog.saveResults("sorted_by_temp.csv");
                std::cout << "Saved to sorted_by_temp.csv\n";
                break;
        }
    } while (choice != 4);
}

// void MainMenu::searchPlanet() {
//     std::string name;
//     std::cout << "Enter planet KOI or Kepler name: ";
//     std::getline(std::cin, name);

//     const auto& planets = catalog.getPlanets();
//     bool found = false;

//     for (const auto& planet : planets) {
//         if (planet.kepoi_name == name || planet.kepler_name == name) {
//             std::cout << "\n=== Planet Found ===\n"
//                       << "KOI Name: " << planet.kepoi_name << "\n"
//                       << "Kepler Name: " << (planet.kepler_name.empty() ? "N/A" : planet.kepler_name) << "\n"
//                       << "Period: " << planet.koi_period << " days\n"
//                       << "Radius: " << planet.koi_prad << " Earth radii\n"
//                       << "Temp: " << planet.koi_teq << " K\n"
//                       << "Status: " << planet.koi_disposition << "\n";
//             found = true;
//             break;
//         }
//     }

//     if (!found) {
//         std::cout << "Planet not found. Try exact KOI name (e.g., K00752.02)\n";
//     }
// }

void MainMenu::searchPlanet() {
    std::string name;
    std::cout << "Enter planet KOI or Kepler name: ";
    std::getline(std::cin, name);

    const auto& planets = catalog.getPlanets();
    bool found = false;

    for (const auto& planet : planets) {
        if (planet.kepoi_name == name || planet.kepler_name == name) {
            std::cout << "\n=== Planet Found ===\n"
                      << "KOI Name: " << planet.kepoi_name << "\n"
                      << "Kepler Name: " << (planet.kepler_name.empty() ? "N/A" : planet.kepler_name) << "\n"
                      << "Period: " << planet.koi_period << " days\n"
                      << "Radius: " << planet.koi_prad << " Earth radii\n"
                      << "Temp: " << planet.koi_teq << " K\n"
                      << "Status: " << planet.koi_disposition << "\n";

            // Add sub-menu
            int choice;
            do {
                std::cout << "\n1. Check your weight on this planet\n"
                     << "2. Check escape velocity\n"
                     << "3. Back to main menu\n"
                     << "Enter choice: ";
                std::cin >> choice;
                std::cin.ignore();

                if (choice == 1) {
                    catalog.printGravityAndWeightForPlanet(planet);
                } else if (choice == 2) {
                    catalog.printEscapeVelocityForPlanet(planet);
                } else if (choice != 3) {
                    std::cout << "Invalid choice\n";
                }
            } while (choice != 3);


            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Planet not found. Try exact KOI name (e.g., K00752.02)\n";
    }
}


void MainMenu::displayAllPlanets() {
    const auto& planets = catalog.getPlanets();
    std::cout << "\n=== All Planets (" << planets.size() << ") ===\n";

    for (const auto& planet : planets) {
        std::cout << planet.kepoi_name << ": " 
                  << "P=" << planet.koi_period << "d, "
                  << "R=" << planet.koi_prad << "R⊕, "
                  << "T=" << planet.koi_teq << "K\n";
    }
}

void MainMenu::displayHabitablePlanets() {
    auto habitable = catalog.findHabitablePlanets();
    std::cout << "\n=== Habitable Planets (" << habitable.size() << ") ===\n";

    if (habitable.empty()) {
        std::cout << "No potentially habitable planets found.\n";
        return;
    }

    for (const auto& planet : habitable) {
        std::cout << planet.kepoi_name << ": "
                  << "P=" << planet.koi_period << "d, "
                  << "R=" << planet.koi_prad << "R⊕, "
                  << "T=" << planet.koi_teq << "K\n";
    }
}

void MainMenu::extremeFinderSubMenu() {
    int choice;
    size_t n = 5;
    std::string type;
    
    std::cout << "How many extremes to show (default 5)? ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty()) {
        try {
            n = std::stoul(input);
        } catch (...) {
            std::cout << "Invalid input, using default 5\n";
        }
    }

    do {
        std::cout << "\n=== Find Extreme Planets ===\n"
                  << "1. Largest Radii\n"
                  << "2. Smallest Radii\n"
                  << "3. Highest Temperatures\n"
                  << "4. Lowest Temperatures\n"
                  << "5. Longest Orbital Periods\n"
                  << "6. Shortest Orbital Periods\n"
                  << "7. Highest Insolation\n"
                  << "8. Lowest Insolation\n"
                  << "9. Back\n"
                  << "======================\n"
                  << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        bool findMax;
        std::string property;
        
        switch(choice) {
            case 1: property = "radius"; findMax = true; break;
            case 2: property = "radius"; findMax = false; break;
            case 3: property = "temperature"; findMax = true; break;
            case 4: property = "temperature"; findMax = false; break;
            case 5: property = "period"; findMax = true; break;
            case 6: property = "period"; findMax = false; break;
            case 7: property = "insolation"; findMax = true; break;
            case 8: property = "insolation"; findMax = false; break;
            case 9: return;
            default: 
                std::cout << "Invalid choice\n";
                continue;
        }
        
        catalog.findTopExtremes(n, property, findMax);
    } while (true);
}