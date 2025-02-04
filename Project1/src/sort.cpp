#include "tweet.h"

//bubble sort
void bubbleSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    //exit if the vector is empty or contains only one element
    if (tweets.size() < 2) return;

    for (size_t i = 0; i < tweets.size() - 1; i++) {
        for (size_t j = 0; j < tweets.size() - i - 1; j++) {
            bool condition = false;

            //checking by sorting criteria and if its ascending or descending
            if (sortBy == "favoriteCount") {
                condition = (ascending) ? (tweets[j].favoriteCount > tweets[j + 1].favoriteCount) : (tweets[j].favoriteCount < tweets[j + 1].favoriteCount);
            } else if (sortBy == "retweetCount") {
                condition = (ascending) ? (tweets[j].retweetCount > tweets[j + 1].retweetCount) : (tweets[j].retweetCount < tweets[j + 1].retweetCount);
            } else if (sortBy == "tweetID") {
                condition = (ascending) ? (tweets[j].tweetID > tweets[j + 1].tweetID) : (tweets[j].tweetID < tweets[j + 1].tweetID);
            }   

            if (condition) {
                std::swap(tweets[j], tweets[j + 1]);
            }
        }
    }
}

//insertion Sort
void insertionSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    //exit if the vector is empty or contains only one element
    if (tweets.size() < 2) return;

    //iterate through the vector starting from the second element
    for (size_t i = 1; i < tweets.size(); i++) {
        Tweet key = tweets[i]; //store the current Tweet to be positioned
        int j = i - 1; //index for comparing with the sorted part of the array

        bool condition = false;

        //compare the key with elements in the sorted portion of the vector
        while (j >= 0) {
            //checking for ascending or descending first then the sort attribute
            if (ascending) {
                if (sortBy == "favoriteCount") {
                    condition = tweets[j].favoriteCount > key.favoriteCount;
                } else if (sortBy == "retweetCount") {
                    condition = tweets[j].retweetCount > key.retweetCount;
                } else if (sortBy == "tweetID") {
                    condition = tweets[j].tweetID > key.tweetID;
                }
            } else {
                if (sortBy == "favoriteCount") {
                    condition = tweets[j].favoriteCount < key.favoriteCount;
                } else if (sortBy == "retweetCount") {
                    condition = tweets[j].retweetCount < key.retweetCount;
                } else if (sortBy == "tweetID") {
                    condition = tweets[j].tweetID < key.tweetID;
                }
            }
            if (!condition) break;

            //shifting the current Tweet one to right
            tweets[j + 1] = tweets[j];
            j--; //move the next element to left
        }

        //insert the key Tweet into the correct position
        tweets[j + 1] = key;
    }
}

//merge sort helper function 
void merge(std::vector<Tweet>& tweets, int left, int mid, int right, const std::string& sortBy, bool ascending) 
{
    int n1 = mid - left + 1; //size of the left subarray
    int n2 = right - mid;    //size of the right subarray

    //temporary vectors for the left and right subarrays
    std::vector<Tweet> leftArr(n1), rightArr(n2);

    //copy data to the left subarray
    for (int i = 0; i < n1; i++) 
        leftArr[i] = tweets[left + i];
    //copy data to the right subarray
    for (int i = 0; i < n2; i++) 
        rightArr[i] = tweets[mid + 1 + i];

    //initialize pointers for left, right, and merged subarrays
    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        bool condition = false;

        if (sortBy == "favoriteCount") {
            condition = (ascending) ? (leftArr[i].favoriteCount <= rightArr[j].favoriteCount) : (leftArr[i].favoriteCount >= rightArr[j].favoriteCount);
        } else if (sortBy == "retweetCount") {
            condition = (ascending) ? (leftArr[i].retweetCount <= rightArr[j].retweetCount) : (leftArr[i].retweetCount >= rightArr[j].retweetCount);
        } else if (sortBy == "tweetID") {
            condition = (ascending) ? (leftArr[i].tweetID <= rightArr[j].tweetID) : (leftArr[i].tweetID >= rightArr[j].tweetID);
        }

        //place the smaller (or larger, depending on the order) element back into the original array
        if (condition) {
            tweets[k] = leftArr[i]; //add from left array if condition is true
            i++; //move to the next element in the left array
        } else {
            tweets[k] = rightArr[j]; //add from the right array if false
            j++; //move to the next element in the right array
        }
        k++;
    }

    //copy any remaining elements from the left array
    while (i < n1) {
        tweets[k] = leftArr[i];
        i++;
        k++;
    }

    //copy any remaining elements from the right array
    while (j < n2) {
        tweets[k] = rightArr[j];
        j++;
        k++;
    }
}

//merge Sort
void mergeSort(std::vector<Tweet>& tweets, int left, int right, const std::string& sortBy, bool ascending) 
{
    //exit if the vector is empty or contains only one element
    if (tweets.size() < 2) return;

    //check if there is more than one element in the subarray
    if (left < right) {
        //calculate the mid index
        int mid = left + (right - left) / 2;

        //recursively sort the first half
        mergeSort(tweets, left, mid, sortBy, ascending);
        //recursively sort the second half
        mergeSort(tweets, mid + 1, right, sortBy, ascending);

        //merge the two sorted halves
        merge(tweets, left, mid, right, sortBy, ascending);
    }
}
