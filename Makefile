# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -Isrc
LDFLAGS := 
DEBUG_FLAGS := -g -O0
RELEASE_FLAGS := -O3

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Target executable
TARGET := $(BIN_DIR)/exoplanet_analyzer

# Build mode (debug or release)
BUILD_MODE ?= debug

# Set appropriate flags based on build mode
ifeq ($(BUILD_MODE),debug)
    CXXFLAGS += $(DEBUG_FLAGS)
else ifeq ($(BUILD_MODE),release)
    CXXFLAGS += $(RELEASE_FLAGS)
else
    $(error Invalid BUILD_MODE. Use 'debug' or 'release')
endif

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Main target linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Pattern rule for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Header dependencies
$(BUILD_DIR)/MainMenu.o: $(SRC_DIR)/MainMenu.hpp $(INCLUDE_DIR)/ExoplanetCatalog.hpp
$(BUILD_DIR)/ExoplanetCatalog.o: $(INCLUDE_DIR)/ExoplanetCatalog.hpp $(INCLUDE_DIR)/Exoplanet.hpp
$(BUILD_DIR)/fileio.o: $(INCLUDE_DIR)/fileio.hpp $(INCLUDE_DIR)/Exoplanet.hpp
$(BUILD_DIR)/main.o: $(SRC_DIR)/MainMenu.hpp

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the program
run: all
	@./$(TARGET)

# Phony targets
.PHONY: all clean run directories