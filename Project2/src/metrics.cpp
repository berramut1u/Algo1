#include "methods.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int getMax(std::vector<Item>& items, const std::string& attribute)
{
    //throw error if vector is empty
    if (items.empty()) {
        throw std::invalid_argument("Items vector is empty.");
    }

    int max = 0;

    //iterate through items for chosen attribute to find the max value
    for(const auto& item : items){
        int value = 0;
        if(attribute == "age"){
            value = item.age;
        } else if(attribute == "type"){
            value = item.type;
        } else if(attribute == "origin"){
            value = item.origin;
        } else {
            throw std::invalid_argument("Unsupported attribute: " + attribute);
        }
        max = std::max(max, value);
    }
    return max;
}

// min = age - ageWindow
// max = age + ageWindow
// rarityScore = (1 - probability) * (1 + item.age/ageMax)
void calculateRarityScores(std::vector<Item>& items, int ageWindow)
{
    int ageMax = getMax(items, "age"); //find maximum age 

    //iterate through items 
    for(auto& item : items){
        int totalCount = 0; //total number of items in ageWindow
        int similarCount = 0; //number of items that have the same attributes

        int min = item.age - ageWindow;
        int max = item.age + ageWindow;

        // iterate through items to count how many are in within the age window
        for(const auto& other : items){
            if(other.age >= min && other.age<= max){
                totalCount++; //increment count for every item in ageWindow

                //checking if the item has all the attributes same
                if (other.type == item.type && other.origin == item.origin) {
                    similarCount++; //increment count for the same items
                }
            }
        }

        //calculate P
        double P =(totalCount > 0) ? static_cast<double>(similarCount) / totalCount : 0.0;

        //finally, rarityScore calculation according to equation
        item.rarityScore = (1.0 - P) * (1.0 + static_cast<double>(item.age) / ageMax);
    }
}
