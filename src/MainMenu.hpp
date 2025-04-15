#pragma once
#include "../include/ExoplanetCatalog.hpp"
#include <string>

class MainMenu {
private:
    ExoplanetCatalog catalog;
    const std::string dataFile = "cumulative_cleaned1.csv"; // Preloaded data file
    
    void displayMainMenu();
    void sortSubMenu();
    void analyzeSubMenu();
    void visualizeSubMenu();
    void searchPlanet();
    void displayAllPlanets();
    void findTopExtremes(); 
    void saveSortedData(const std::string& sortType);
    void displayHabitablePlanets();
    void displayPlanetExtremes();
    void extremeFinderSubMenu();
    
    public:
    MainMenu(); // Constructor to load data automatically
    void run();
};