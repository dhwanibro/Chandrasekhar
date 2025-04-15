#pragma once
#include "Exoplanet.hpp"
#include "datastructs/DynamicArray.hpp"
#include "datastructs/BinarySearchTree.hpp"
#include "datastructs/HashTable.hpp"
#include <string>


struct PlanetComparator {
    std::string prop;
    PlanetComparator(const std::string& p) : prop(p) {}
    bool operator()(const Exoplanet& a, const Exoplanet& b) const;
};

class ExoplanetCatalog {
private:
    DynamicArray<Exoplanet> planets;
    BinarySearchTree<double, Exoplanet*> tempTree;
    HashTable<std::string, Exoplanet*> nameIndex;
      
public:
    // Core functionality
    void loadData(const std::string& filename);
    void buildIndices();
    
    // Sorting
    void sortByPeriod();
    void sortByRadius();
    void sortByTemperature();
    
    // Analysis
    struct Stats {
        double mean;
        double median;
        double min;
        double max;
    };
    void findTopExtremes(size_t n, const std::string& property, bool findMax);
    Stats analyzePeriods() const;

    void printGravityAndWeightForPlanet(const Exoplanet& planet) const;
    
    Exoplanet* findPlanetByName(const std::string& name) const;

    // Visualization
    void printTopNByRadius(int n) const;
    void addPlanet(const Exoplanet& planet);
    HashTable<std::string, int> analyzePlanetTypes() const;
    void printPlanetTypeAnalysis() const;
    
    void printEscapeVelocityForPlanet(const Exoplanet& planet) const;

    // Habitable planets functionality
    void printHabitablePlanets() const;
   DynamicArray<Exoplanet> findHabitablePlanets() const;
    
    // Data export
    bool saveResults(const std::string& filename) const;
    
    // Data access
    const DynamicArray<Exoplanet>& getPlanets() const;
};
