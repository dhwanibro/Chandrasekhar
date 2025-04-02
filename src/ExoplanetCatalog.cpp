#include "../include/ExoplanetCatalog.hpp"
#include "../include/fileio.hpp"
#include "../include/algorithms/sorting.hpp"
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

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

ExoplanetCatalog::Stats ExoplanetCatalog::analyzePeriods() const {
    Stats stats;
    if (planets.empty()) return stats;
    
    DynamicArray<double> periods;
    for (const auto& planet : planets) {
        if (!std::isnan(planet.koi_period)) {
            periods.push_back(planet.koi_period);
        }
    }
    
    if (periods.empty()) return stats;
    
    algo::quick_sort(periods.begin(), periods.end());
    
    stats.min = periods[0];
    stats.max = periods[periods.size() - 1];
    
    stats.mean = std::accumulate(periods.begin(), periods.end(), 0.0) / periods.size();
    
    size_t mid = periods.size() / 2;
    stats.median = (periods.size() % 2 == 0) ?
        (periods[mid-1] + periods[mid]) / 2 :
        periods[mid];
    
    return stats;
}

void ExoplanetCatalog::printTemperatureHistogram(int bins) const {
    if (planets.empty()) {
        std::cout << "No data available for histogram.\n";
        return;
    }

    double min_temp = std::numeric_limits<double>::max();
    double max_temp = std::numeric_limits<double>::lowest();
    size_t valid_count = 0;

    for (const auto& planet : planets) {
        if (!std::isnan(planet.koi_teq)) {
            min_temp = std::min(min_temp, planet.koi_teq);
            max_temp = std::max(max_temp, planet.koi_teq);
            valid_count++;
        }
    }

    if (valid_count == 0) {
        std::cout << "No valid temperature data available.\n";
        return;
    }

    double bin_size = (max_temp - min_temp) / bins;
 DynamicArray<int> counts;
    counts.resize(bins);  // Or initialize properly based on your DynamicArray implementation
    for (int i = 0; i < bins; i++) {
        counts.push_back(0);
    }

    for (const auto& planet : planets) {
        if (!std::isnan(planet.koi_teq)) {
            int bin = static_cast<int>((planet.koi_teq - min_temp) / bin_size);
            if (bin == bins) bin--;
            counts[bin]++;
        }
    }

    std::cout << "Temperature Distribution (K):\n";
    for (int i = 0; i < bins; ++i) {
        double lower = min_temp + i * bin_size;
        double upper = lower + bin_size;
        std::cout << std::fixed << std::setprecision(1) 
                  << "[" << lower << " - " << upper << ") : " 
                  << std::string(counts[i] * 50 / valid_count, '#') 
                  << " (" << counts[i] << ")\n";
    }
}

void ExoplanetCatalog::printTopNByRadius(int n) const {
    if (planets.empty()) {
        std::cout << "No planets available.\n";
        return;
    }

    DynamicArray<Exoplanet> sorted = planets;
    
    algo::quick_sort(sorted.begin(), sorted.end(), 
        [](const Exoplanet& a, const Exoplanet& b) {
            return a.koi_prad > b.koi_prad;
        });

    std::cout << "\nTop " << n << " Largest Exoplanets:\n";
    std::cout << std::setw(20) << "Name" 
              << std::setw(15) << "Radius (Earth)" 
              << std::setw(15) << "Temperature (K)\n";
    
    int count = 0;
    for (const auto& planet : sorted) {
        if (count >= n) break;
        if (!std::isnan(planet.koi_prad)) {
            std::cout << std::setw(20) << planet.kepoi_name
                      << std::setw(15) << planet.koi_prad
                      << std::setw(15) << planet.koi_teq << "\n";
            count++;
        }
    }
}