#pragma once
#include "Exoplanet.hpp"
#include <string>

// Forward declaration to resolve circular dependency
class ExoplanetCatalog;

class FileIO {
public:
    static bool parseCSV(const std::string& filename, ExoplanetCatalog& catalog);
    static bool writeCSV(const std::string& filename, const DynamicArray<Exoplanet>& planets);
};