# Chandrasekhar
🔗 About the Name: Subrahmanyan Chandrasekhar revolutionized our understanding of stellar evolution. This project aims to apply a similarly rigorous approach to uncovering hidden insights in the cosmos.


# Exoplanet Catalog System

A C++ application for managing and analyzing exoplanet data using custom data structures.

## Features

- **Custom Data Structures**
  - `DynamicArray`: Template-based resizable array
  - `HashTable`: Chaining hash table with prime-number sizing
  - `MaxHeap`: Priority queue implementation

- **Core Functionality**
  - CSV data import/export
  - Multiple sorting algorithms
  - Statistical analysis
  - Habitable planet detection
  - Extreme value identification

## Installation


git clone https://github.com/yourusername/Chandrasekhar.git
cd Chandrasekhar
make

## Usage

./bin/exoplanet_analyzer


**Main Menu Options:**

1. Sort and Export Data
2. Find Extreme Planets
3. Display All Planets
4. Search Planet
5. Show Habitable Planets
6. Show Planet Type Distribution
7. Exit


## Data Structures

### DynamicArray

// Example usage:
DynamicArray<Exoplanet> planets;
planets.push_back(planet);


### HashTable

// Example usage:
HashTable<std::string, int> typeCounts(17);
typeCounts.insert("Gas Giant", 42);


### MaxHeap

// Example usage:
MaxHeap<Exoplanet> largestPlanets;
largestPlanets.push(planet);


## Examples

### Finding Top 5 Largest Planets

Menu > 2 (Find Extreme Planets) > 1 (Largest Radii) > 5

Output:

Top 5 Largest Exoplanets:
Kepler-39 b          15.2 R⊕     1450 K
HD 100546 b          12.8 R⊕     2100 K


### Displaying Habitable Planets

Menu > 5 (Show Habitable Planets)

Output:

List of Habitable Planets:
Kepler-186 f         225 K
Kepler-442 b         233 K


## File Structure


bin/exoplanet-catalog/
├── include/          # Header files
│   ├── datastructs/  # Custom data structures
│   └── algorithms/   # Sorting implementations
├── src/              # Source files
├── file.csv          # Sample CSV data
└── Makefile          # Build configuration


## Contributing

1. Fork the project
2. Create your feature branch (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -m 'Add new feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Open a Pull Request

## License

MIT License - See [LICENSE](LICENSE) for details.
```
