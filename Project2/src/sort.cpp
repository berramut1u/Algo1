#include "methods.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<Item> countingSort(std::vector<Item>& items, const std::string& attribute, bool ascending)
{
    if (items.empty()) return items; //if the theres nothing to sort, return the input as it is

    //if the attribute is wrong throw error
    if (attribute != "age" && attribute != "type" && attribute != "origin") {
        throw std::invalid_argument("Unsupported attribute: " + attribute);
    }

    int max = getMax(items, attribute); //find the maximum value of the chosen attribute

    std::vector<int> count(max + 1, 0); //defining a counting array with the size 1 greater then max value so it can store everything from 0 to max

    //counting the occurance of the chosen attribute 
    for (const auto& item : items) {
        int value = 0;
        if (attribute == "age") {
            value = item.age;
        } else if (attribute == "type"){
            value = item.type;
        } else if (attribute == "origin"){
            value = item.origin;
        }  
        if (value < 0 || value > max) { //checking bounds 
        throw std::out_of_range("Attribute value out of range.");
        }
        count[value]++;
    }

    //calculating the cumulative sum for ascending
    if (ascending) {
        for (size_t i = 1; i < count.size(); i++) {
            count[i] += count[i - 1];
        }
    } else {
        //for descending order, reverse cumulative sums
        for (int i = count.size() - 2; i >= 0; i--) {
            count[i] += count[i + 1];
        }
    }

    std::vector<Item> sortedItems(items.size()); //creating a vector for sorted items 
    for (int i = items.size() - 1; i >= 0; i--) {
        int value = 0;
        if (attribute == "age") {
            value = items[i].age;
        } else if (attribute == "type"){
            value = items[i].type;
        } else if (attribute == "origin"){
            value = items[i].origin;
        }
        int position = count[value] - 1; //determine the correct position in the sorted array
        sortedItems[position] = items[i]; //place the item in the correct position
        count[value]--; //decrement the count for this value
    }

    return sortedItems;
}


// Function to heapify a subtree rooted with node i in the array of items
void heapify(std::vector<Item>& items, int n, int i, bool descending) {
    int root = i;          //root node index
    int left = 2 * i + 1;  //left child index
    int right = 2 * i + 2; //right child index

    //check the left child
    if (left < n) {
        if (descending) {
            //for descending order(min-heap), check if left child is smaller than root
            if (items[left].rarityScore < items[root].rarityScore) {
                root = left; //update root to left
            }
        } else {
            //for ascending order(max-heap), check if left child is greater than root
            if (items[left].rarityScore > items[root].rarityScore) {
                root = left; //update root to left
            }
        }
    }

    //check the right child
    if (right < n) {
        if (descending) {
            //for descending order(min-heap), check if right child is smaller than current root
            if (items[right].rarityScore < items[root].rarityScore) {
                root = right; //update root to right
            }
        } else {
            //for ascending order(max-heap), check if right child is greater than current root
            if (items[right].rarityScore > items[root].rarityScore) {
                root = right; //update root to right
            }
        }
    }

    //if root is not the original root, swap and recurse
    if (root != i) {
        std::swap(items[i], items[root]); //swap the root with the largest/smallest child
        heapify(items, n, root, descending); //recursively heapify the affected subtree
    }
}
 

// Function to perform heap sort on rarityScore scores of items
std::vector<Item> heapSortByRarity(std::vector<Item>& items, bool descending) {
    int n = items.size();

    //build the heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(items, n, i, descending);  //build the heap from bottom-up
    }

    //extract elements one by one from the heap
    for (int i = n - 1; i > 0; i--) {
        std::swap(items[0], items[i]);  //move the root to the end
        heapify(items, i, 0, descending);  //restore heap property on the reduced heap
    }

    return items; 
}

