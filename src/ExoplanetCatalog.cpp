#include "../include/ExoplanetCatalog.hpp"
#include "../include/fileio.hpp"
#include "../include/algorithms/sorting.hpp"
#include "../include/datastructs/HashTable.hpp"
#include "../include/datastructs/MaxHeap.hpp"
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

void ExoplanetCatalog::addPlanet(const Exoplanet& planet) {
    planets.push_back(planet);
}

void ExoplanetCatalog::loadData(const std::string& filename) {
    if (!FileIO::parseCSV(filename, *this)) {
        throw std::runtime_error("Failed to load data from " + filename);
    }
}

const DynamicArray<Exoplanet>& ExoplanetCatalog::getPlanets() const {
    return planets;
}

bool ExoplanetCatalog::saveResults(const std::string& filename) const {
    return FileIO::writeCSV(filename, planets);
}

void ExoplanetCatalog::buildIndices() {
    tempTree.clear();
    for (const auto& planet : planets) {
        if (!std::isnan(planet.koi_teq)) {
            tempTree.insert(planet.koi_teq, const_cast<Exoplanet*>(&planet));
        }
    }
}

void ExoplanetCatalog::sortByPeriod() {
    algo::quick_sort(planets.begin(), planets.end(), 
        [](const Exoplanet& a, const Exoplanet& b) {
            return a.koi_period < b.koi_period;
        });
}

void ExoplanetCatalog::sortByRadius() {
    algo::quick_sort(planets.begin(), planets.end(),
        [](const Exoplanet& a, const Exoplanet& b) {
            return a.koi_prad < b.koi_prad;
        });
}

void ExoplanetCatalog::sortByTemperature() {
    algo::quick_sort(planets.begin(), planets.end(),
        [](const Exoplanet& a, const Exoplanet& b) {
            return a.koi_teq < b.koi_teq;
        });
}

double calculateMin(const DynamicArray<double>& data) {
    if (data.size() == 0) return NAN; // Handle empty case
    double min_val = data[0];
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
    }
    return min_val;
}

double calculateMax(const DynamicArray<double>& data) {
    if (data.size() == 0) return NAN; // Handle empty case
    double max_val = data[0];
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }
    return max_val;
}

double calculateMean(const DynamicArray<double>& data) {
    if (data.size() == 0) return NAN; // Handle empty case
    double sum = 0.0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];
    }
    return sum / data.size();
}

double calculateMedian(DynamicArray<double>& data) {
    if (data.size() == 0) return NAN; // Handle empty case
    // Sorting data manually using quicksort
    algo::quick_sort(data.begin(), data.end());
    
    size_t mid = data.size() / 2;
    if (data.size() % 2 == 0) {
        return (data[mid - 1] + data[mid]) / 2.0;
    } else {
        return data[mid];
    }
}

HashTable<std::string, int> ExoplanetCatalog::analyzePlanetTypes() const {
    HashTable<std::string, int> typeCounts(17); // Prime number for better distribution
    
    for (const auto& planet : planets) {
        if (std::isnan(planet.koi_prad) || std::isnan(planet.koi_teq)) continue;
        
        std::string type;
        double radius = planet.koi_prad;
        double temp = planet.koi_teq;
        
        // Radius classification
        if (radius < 1.0) type = "Sub-Earth";
        else if (radius < 1.5) type = "Earth-like";
        else if (radius < 2.0) type = "Super-Earth";
        else if (radius < 6.0) type = "Mini-Neptune";
        else type = "Gas Giant";
        
        // Temperature classification
        if (temp < 200) type += " (Frozen)";
        else if (temp < 300) type += " (Temperate)";
        else if (temp < 500) type += " (Warm)";
        else if (temp < 1000) type += " (Hot)";
        else type += " (Scorching)";
        
        // Count using HashTable
        if (typeCounts.contains(type)) {
            typeCounts[type]++;
        } else {
            typeCounts.insert(type, 1);
        }
    }
    
    return typeCounts;
}

void ExoplanetCatalog::printPlanetTypeAnalysis() const {
    auto typeCounts = analyzePlanetTypes();
    
    std::cout << "\nPlanet Type Distribution:\n";
    std::cout << "-------------------------\n";
    
    // Create a DynamicArray to store and sort the types
    DynamicArray<std::pair<std::string, int>> sortedTypes;
    
    // Collect all types
    for (size_t i = 0; i < typeCounts.bucketCount(); ++i) {
        auto* entry = typeCounts.getBucket(i);
        while (entry) {
            sortedTypes.push_back(std::make_pair(entry->key, entry->value));
            entry = entry->next;
        }
    }
    
    // Sort by count (descending) using your quick_sort
    algo::quick_sort(sortedTypes.begin(), sortedTypes.end(),
        [](const auto& a, const auto& b) { return b.second < a.second; });
    
    // Print results
    for (size_t i = 0; i < sortedTypes.size(); ++i) {
        const auto& pair = sortedTypes[i];
        std::cout << std::setw(25) << pair.first 
                  << ": " << pair.second << " planets\n";
    }
}

DynamicArray<Exoplanet> ExoplanetCatalog::findHabitablePlanets() const {
    DynamicArray<Exoplanet> habitable;

    for (const auto& planet : planets) {
        if (planet.koi_disposition == "CONFIRMED" &&
            planet.koi_prad >= 0.8 && planet.koi_prad <= 1.5 &&
            planet.koi_teq >= 200 && planet.koi_teq <= 300) {
            habitable.push_back(planet);
        }
    }

    return habitable;
}

void ExoplanetCatalog::printHabitablePlanets() const {
    DynamicArray<Exoplanet> habitablePlanets = findHabitablePlanets();

    if (habitablePlanets.empty()) {
        std::cout << "No habitable planets found.\n";
        return;
    }

    std::cout << "\nList of Habitable Planets:\n";
    std::cout << std::setw(25) << "Planet Name"
              << std::setw(20) << "Equilibrium Temp (K)" << "\n";

    for (size_t i = 0; i < habitablePlanets.size(); ++i) {
        const auto& planet = habitablePlanets[i];
        std::cout << std::setw(25) << planet.kepoi_name
                  << std::setw(20) << planet.koi_teq << "\n";
    }
}
// Implement the comparator first
bool PlanetComparator::operator()(const Exoplanet& a, const Exoplanet& b) const {
    if (prop == "radius") return a.koi_prad < b.koi_prad;
    if (prop == "temperature") return a.koi_teq < b.koi_teq;
    if (prop == "period") return a.koi_period < b.koi_period;
    if (prop == "insolation") return a.koi_insol < b.koi_insol;
    return false;
}

void ExoplanetCatalog::findTopExtremes(size_t n, const std::string& property, bool findMax) {
    // Define comparator as lambda
    auto compare = [property, findMax](const Exoplanet& a, const Exoplanet& b) {
        double a_val = 0, b_val = 0;
        
        if (property == "radius") {
            a_val = a.koi_prad;
            b_val = b.koi_prad;
        }
        else if (property == "temperature") {
            a_val = a.koi_teq;
            b_val = b.koi_teq;
        }
        else if (property == "period") {
            a_val = a.koi_period;
            b_val = b.koi_period;
        }
        else if (property == "insolation") {
            a_val = a.koi_insol;
            b_val = b.koi_insol;
        }
        
        return findMax ? (a_val < b_val) : (a_val > b_val);
    };

    // Create heap with the comparator
    MaxHeap<Exoplanet, decltype(compare)> heap(compare);
    
    // Filter and add to heap
    for (size_t i = 0; i < planets.size(); ++i) {
        const auto& planet = planets[i];
        if (property == "radius" && !std::isnan(planet.koi_prad)) {
            heap.push(planet);
        }
        else if (property == "temperature" && !std::isnan(planet.koi_teq)) {
            heap.push(planet);
        }
        else if (property == "period" && !std::isnan(planet.koi_period)) {
            heap.push(planet);
        }
        else if (property == "insolation" && !std::isnan(planet.koi_insol)) {
            heap.push(planet);
        }
    }

    // Display results
    std::cout << "\nTop " << n << " " << (findMax ? "maximum" : "minimum") 
              << " planets by " << property << ":\n";
    std::cout << std::setw(25) << "Name" 
              << std::setw(15) << property 
              << std::setw(15) << "Temp (K)" << "\n";
    
    for (size_t i = 0; i < n && !heap.empty(); ++i) {
        const auto& planet = heap.top();
        std::cout << std::setw(25) << planet.kepoi_name
                  << std::setw(15);
        
        if (property == "radius") std::cout << planet.koi_prad;
        else if (property == "temperature") std::cout << planet.koi_teq;
        else if (property == "period") std::cout << planet.koi_period;
        else if (property == "insolation") std::cout << planet.koi_insol;
        
        std::cout << std::setw(15) << planet.koi_teq << "\n";
        heap.pop();
    }
}

void ExoplanetCatalog::printGravityAndWeightForPlanet(const Exoplanet& planet) const {
    if (std::isnan(planet.koi_prad)) {
        std::cout << "Radius data not available for this planet.\n";
        return;
    }

    double gravity = std::pow(planet.koi_prad, 1.7); // relative to Earth
    double userWeight;
    std::cout << "Enter your Earth weight (kg): ";
    std::cin >> userWeight;

    double weightOnPlanet = userWeight * gravity;

    std::cout << "\n=== Surface Gravity Info ===\n"
              << "Planet: " << planet.kepoi_name << "\n"
              << "Estimated Gravity: " << std::fixed << std::setprecision(2) << gravity << " g (Earth)\n"
              << "Your Weight: " << std::fixed << std::setprecision(2) << weightOnPlanet << " kg\n";
}

void ExoplanetCatalog::printEscapeVelocityForPlanet(const Exoplanet& planet) const {
    if (std::isnan(planet.koi_prad)) {
        std::cout << "Radius data not available for this planet.\n";
        return;
    }

    double escapeVelocity = 11.2 * std::pow(planet.koi_prad, 1.35);

    std::cout << "\n=== Escape Velocity Info ===\n"
              << "Planet: " << planet.kepoi_name << "\n"
              << "Estimated Escape Velocity: " 
              << escapeVelocity << " km/s\n"
              << "So if you were to throw a ball with velocity " << escapeVelocity<< "km/s, it would enter space!" <<"\n"
              << std::fixed << std::setprecision(2);
}