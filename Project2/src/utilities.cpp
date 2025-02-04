#include "methods.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<Item> readItemsFromFile(const std::string& filename) 
{
    std::vector<Item> items;
    std::ifstream inputFile(filename);

    //checking if file opened correctly
    if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return items; 
    }

    std::string line;
    std::getline(inputFile, line); //skip the header line

    //reading each line
    while (std::getline(inputFile, line)) {
        std::stringstream lineStream(line);
        std::string age, type, origin, rarity;
        
        //extracting attributes from the line
        std::getline(lineStream, age, ',');
        std::getline(lineStream, type, ',');
        std::getline(lineStream, origin, ',');
        std::getline(lineStream, rarity, ',');


        Item item;
        item.age = std::stoi(age); //converting attributes from string to integer
        item.type = std::stoi(type);
        item.origin = std::stoi(origin);
        item.rarityScore = std::stoi(rarity);

        items.push_back(item); //adding the new item object to vector
    }
    
    inputFile.close();
    return items;    
}

void writeItemsToFile(const std::string& filename, const std::vector<Item>& items) 
{
    std::ofstream outputFile(filename);
    
    //checking if file opened correctly
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    outputFile << "age,type,origin,rarity\n";  // write the header

    //loop over each item in the vector and write its data to the file
    for (const auto& item : items) {
        outputFile << item.age << "," << item.type << "," << item.origin << "," << item.rarityScore << "\n";
    }

    outputFile.close();
}
