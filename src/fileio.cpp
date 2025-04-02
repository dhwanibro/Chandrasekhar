#include "../include/Exoplanet.hpp"
#include "../include/ExoplanetCatalog.hpp"
#include "../include/fileio.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>

bool FileIO::parseCSV(const std::string& filename, ExoplanetCatalog& catalog) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    // Skip header lines
    if (!std::getline(file, line)) return false; // Skip header
    if (!std::getline(file, line)) return false; // Skip titles

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Exoplanet planet = {};

        try {
            // Read and assign fields with proper error checking
            if (!std::getline(ss, token, ',')) continue;
            planet.rowid = std::stoi(token);

            if (!std::getline(ss, token, ',')) continue;
            planet.kepid = std::stoi(token);

            if (!std::getline(ss, token, ',')) continue;
            planet.kepoi_name = token;

            if (!std::getline(ss, token, ',')) continue;
            planet.kepler_name = token;

            if (!std::getline(ss, token, ',')) continue;
            planet.koi_disposition = token;

            if (!std::getline(ss, token, ',')) continue;
            planet.koi_pdisposition = token;

            if (!std::getline(ss, token, ',')) continue;
            planet.koi_score = token.empty() ? std::nan("") : std::stod(token);

            // Read flags
            for (int i = 0; i < 4; i++) {
                if (!std::getline(ss, token, ',')) break;
                int val = token.empty() ? 0 : std::stoi(token);
                switch (i) {
                    case 0: planet.koi_fpflag_nt = val; break;
                    case 1: planet.koi_fpflag_ss = val; break;
                    case 2: planet.koi_fpflag_co = val; break;
                    case 3: planet.koi_fpflag_ec = val; break;
                }
            }

            // Read remaining fields with proper error handling
            #define READ_DOUBLE_FIELD(field) \
                if (!std::getline(ss, token, ',')) continue; \
                field = token.empty() ? std::nan("") : std::stod(token);

            READ_DOUBLE_FIELD(planet.koi_period);
            
            // Skip unused columns
            for (int i = 0; i < 5; i++) std::getline(ss, token, ',');
            
            READ_DOUBLE_FIELD(planet.koi_impact);
            for (int i = 0; i < 2; i++) std::getline(ss, token, ',');
            READ_DOUBLE_FIELD(planet.koi_duration);
            for (int i = 0; i < 2; i++) std::getline(ss, token, ',');
            READ_DOUBLE_FIELD(planet.koi_depth);
            for (int i = 0; i < 2; i++) std::getline(ss, token, ',');
            READ_DOUBLE_FIELD(planet.koi_prad);
            for (int i = 0; i < 2; i++) std::getline(ss, token, ',');
            READ_DOUBLE_FIELD(planet.koi_teq);
            READ_DOUBLE_FIELD(planet.koi_insol);

            // Skip remaining columns
            for (int i = 0; i < 17; i++) std::getline(ss, token, ',');

            catalog.addPlanet(planet);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line: " << line << "\nError: " << e.what() << std::endl;
            continue;
        }
    }

    file.close();
    return true;
}

bool FileIO::writeCSV(const std::string& filename, const DynamicArray<Exoplanet>& planets) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return false;
    }

    // Write header
    file << "rowid,kepid,kepoi_name,kepler_name,koi_disposition,koi_pdisposition,"
         << "koi_score,koi_fpflag_nt,koi_fpflag_ss,koi_fpflag_co,koi_fpflag_ec,"
         << "koi_period,koi_impact,koi_duration,koi_depth,koi_prad,koi_teq,koi_insol\n";

    // Write data
    for (const auto& planet : planets) {
        file << planet.rowid << ","
             << planet.kepid << ","
             << planet.kepoi_name << ","
             << planet.kepler_name << ","
             << planet.koi_disposition << ","
             << planet.koi_pdisposition << ","
             << planet.koi_score << ","
             << planet.koi_fpflag_nt << ","
             << planet.koi_fpflag_ss << ","
             << planet.koi_fpflag_co << ","
             << planet.koi_fpflag_ec << ","
             << planet.koi_period << ","
             << planet.koi_impact << ","
             << planet.koi_duration << ","
             << planet.koi_depth << ","
             << planet.koi_prad << ","
             << planet.koi_teq << ","
             << planet.koi_insol << "\n";
    }

    file.close();
    return true;
}