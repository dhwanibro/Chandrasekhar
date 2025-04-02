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

void MainMenu::run() {
    int choice;
    do {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice) {
            case 1: sortSubMenu(); break;
            case 2: analyzeSubMenu(); break;
            case 3: visualizeSubMenu(); break;
            case 4: displayAllPlanets(); break;
            case 5: searchPlanet(); break;
            case 6: break; // Exit
            default: std::cout << "Invalid choice\n";
        }
    } while (choice != 6);
}

void MainMenu::displayMainMenu() {
    std::cout << "\n==== Exoplanet Catalog ====\n"
              << "1. Sort and Export Data\n"
              << "2. Analyze Orbital Periods\n"
              << "3. Visualize Data\n"
              << "4. Display All Planets\n"
              << "5. Search Planet\n"
              << "6. Exit\n"
              << "=========================\n"
              << "Enter choice: ";
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

void MainMenu::analyzeSubMenu() {
    auto stats = catalog.analyzePeriods();
    std::cout << "\n=== Orbital Period Stats ===\n"
              << std::fixed << std::setprecision(2)
              << "Mean:   " << std::setw(8) << stats.mean << " days\n"
              << "Median: " << std::setw(8) << stats.median << " days\n"
              << "Min:    " << std::setw(8) << stats.min << " days\n"
              << "Max:    " << std::setw(8) << stats.max << " days\n";
}

void MainMenu::visualizeSubMenu() {
    int choice;
    do {
        std::cout << "\n=== Visualizations ===\n"
                  << "1. Top Planets by Radius\n"
                  << "2. Temperature Histogram\n"
                  << "3. Back\n"
                  << "======================\n"
                  << "Enter choice: ";
        
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                int n;
                std::cout << "Number of planets to show: ";
                std::cin >> n;
                catalog.printTopNByRadius(n);
                break;
            }
            case 2: {
                int bins;
                std::cout << "Number of histogram bins: ";
                std::cin >> bins;
                catalog.printTemperatureHistogram(bins);
                break;
            }
        }
    } while (choice != 3);
}

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