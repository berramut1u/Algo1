#include "methods.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>  

int main() 
{
    //load items from a CSV file, change the input file for different sizes
    std::vector<Item> items = readItemsFromFile("data/items_s.csv");

  
    //benchmarking and sorting by age (ascending using countingSort)
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Item> sortedByAge = countingSort(items, "age", true);  //sort by age in ascending order
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start; 
    std::cout << "Time taken for countingSort by age: " << duration.count() << " ms" << std::endl;
    
    //save the sorted items to a CSV file
    //writeItemsToFile("data/items_l_sorted_by_age.csv", sortedByAge);


    //benchmarking and calculating rarity scores
    start = std::chrono::high_resolution_clock::now();
    calculateRarityScores(sortedByAge, 50);  //calculate rarity scores with an age window of 50
    end = std::chrono::high_resolution_clock::now();
    duration = end - start; 
    std::cout << "Time taken to calculate rarity scores: " << duration.count() << " ms" << std::endl;

    //save items with calculated rarity scores to a CSV file
    //writeItemsToFile("data/items_l_rarity.csv", sortedByAge);


    //benchmarking and sorting by rarity score (descending using heapSort)
    start = std::chrono::high_resolution_clock::now();
    std::vector<Item> sortedByRarity = heapSortByRarity(sortedByAge, true);  //sort by rarity score in descending order
    end = std::chrono::high_resolution_clock::now();
    duration = end - start; 
    std::cout << "Time taken for heapSort by rarity score (descending): " << duration.count() << " ms" << std::endl;

    //save the sorted items by rarity score to a CSV file
    //writeItemsToFile("data/items_l_sorted_by_rarity.csv", sortedByRarity);


    //benchmarking and sorting by rarity score (ascending using heapSort)
    start = std::chrono::high_resolution_clock::now();
    sortedByRarity = heapSortByRarity(sortedByAge, false);  //sort by rarity score in ascending order
    end = std::chrono::high_resolution_clock::now();
    duration = end - start; 
    std::cout << "Time taken for heapSort by rarity score (ascending): " << duration.count() << " ms" << std::endl;

    //save the sorted items by rarity score in ascending order to a CSV file
    //writeItemsToFile("data/items_l_sorted_by_rarity_ascending.csv", sortedByRarity);

    return 0;
}
