#pragma once
#include "Exoplanet.hpp"
#include "datastructs/DynamicArray.hpp"
#include "datastructs/BinarySearchTree.hpp"
#include <string>

class ExoplanetCatalog {
private:
    DynamicArray<Exoplanet> planets;
    BinarySearchTree<double, Exoplanet*> tempTree;
    
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
    
    Stats analyzePeriods() const;
    
    // Visualization
    void printTopNByRadius(int n) const;
    void printTemperatureHistogram(int bins) const;
    void addPlanet(const Exoplanet& planet);

    
    // Data export
    bool saveResults(const std::string& filename) const;
    
    // Data access
    const DynamicArray<Exoplanet>& getPlanets() const;
};