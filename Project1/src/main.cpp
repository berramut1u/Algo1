#include "tweet.h"
#include <chrono>
#include <iomanip>

//function to measure execution time
template<typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() {
    //read tweets from file (change the path to test each file)
    std::string inputFile = "data/permutations/tweets.csv"; 

    std::vector<Tweet> tweets = readTweetsFromFile(inputFile);
    
    if (tweets.empty()) {
        std::cerr << "No tweets were read from the file." << std::endl;
        return 1;
    }

    //creating copies for different sorts
    std::vector<Tweet> bubbleSortTweets = tweets;
    std::vector<Tweet> insertionSortTweets = tweets;
    std::vector<Tweet> mergeSortTweets = tweets;

    //test sorting by favorites (ascending)
    std::cout << "\n=== Sorting Performance Comparison (by retweet count) ===" << std::endl;
    
    //bubble Sort
    double bubbleTime = measureTime([&]() {
        bubbleSort(bubbleSortTweets, "retweetCount", true);
    });
    std::cout << "Bubble Sort time: " << bubbleTime << " seconds" << std::endl;

    //insertion Sort
    double insertionTime = measureTime([&]() {
        insertionSort(insertionSortTweets, "retweetCount", true);
    });
    std::cout << "Insertion Sort time: " << insertionTime << " seconds" << std::endl;

    //merge Sort
    double mergeTime = measureTime([&]() {
        mergeSort(mergeSortTweets, 0, mergeSortTweets.size() - 1, "retweetCount", true);
    });
    std::cout << "Merge Sort time: " << mergeTime << " seconds" << std::endl;

    //test binary search
    std::cout << "\n=== Binary Search Test ===" << std::endl;
    int searchindex = 1773335;  //search for tweets with 1773335 index
    double binarySearchTime = measureTime([&]() {
    int index = binarySearch(mergeSortTweets, searchindex, "tweetID");
    
    if (index != -1) {
        std::cout << "Found tweet with " << searchindex << " id. " << index << std::endl;
    } else {
        std::cout << "No tweet found with exactly " << searchindex << " id." << std::endl;
    }
    });
    std::cout << "Binary Search time: " << binarySearchTime << " seconds" << std::endl;


    //test count above threshold
    std::cout << "\n=== Threshold Analysis ===" << std::endl;
    int threshold = 250;
    double countFavoritesTime = measureTime([&]() {
    int countFavorites = countAboveThreshold(tweets, "favoriteCount", threshold);
    std::cout << "Tweets with more than " << threshold << " favorites: " << countFavorites << std::endl;
    });
    std::cout << "Count Above Threshold time: " << countFavoritesTime << " seconds" << std::endl;

    return 0;
}