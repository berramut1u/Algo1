#include "tweet.h"
#include <vector>
#include <string>

int binarySearch(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy)
{
    int left = 0;
    int right = tweets.size() - 1;

    //continue searching while there is a valid range
    while (left <= right) {
        int mid = left + (right - left) / 2;

        long long midValue;

        //determine which attribute to compare
        if (sortBy == "favoriteCount") {
            midValue = tweets[mid].favoriteCount;
        } else if (sortBy == "retweetCount") {
            midValue = tweets[mid].retweetCount;
        } else if (sortBy == "tweetID") {
            midValue = tweets[mid].tweetID;
        } else {
            return -1; //if sortBy is not recognized, return -1 (error)
        }

        if (midValue == key) {
            return mid; //found the tweet
        } else if (midValue < key) {
            left = mid + 1; //search the right half
        } else {
            right = mid - 1; //search the left half
        }
    }

    return -1; //key not found
}

int countAboveThreshold(const std::vector<Tweet>& tweets, const std::string& metric, int threshold) 
{
    int count = 0;

    //iterate through each Tweet in the vector
    for (const auto& tweet : tweets) {
        //check if the Tweet's metric exceeds the threshold
        if ((metric == "favoriteCount" && tweet.favoriteCount > threshold) ||
            (metric == "retweetCount" && tweet.retweetCount > threshold)) {
            count++; //increment the count if the condition is met
        }
    }

    return count;
}
