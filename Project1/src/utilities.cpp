#include "tweet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<Tweet> readTweetsFromFile(const std::string& filename) 
{
    std::vector<Tweet> tweets;
    std::ifstream inputFile(filename);

    //checking if file opened correctly
    if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return tweets; 
    }

    std::string line;
    std::getline(inputFile, line); //skip the header line

    //reading each line
    while (std::getline(inputFile, line)) {
        std::stringstream lineStream(line);
        std::string tweetID, retweetCount, favoriteCount;
        
        //extracting attributes from the line
        std::getline(lineStream, tweetID, ',');
        std::getline(lineStream, retweetCount, ',');
        std::getline(lineStream, favoriteCount, ',');

        Tweet tweet;
        tweet.tweetID = std::stoi(tweetID); //converting attributes from string to integer
        tweet.retweetCount = std::stoi(retweetCount);
        tweet.favoriteCount = std::stoi(favoriteCount);

        tweets.push_back(tweet); //adding the new Tweet object to vector
    }

    inputFile.close();
    return tweets;    
}

void writeTweetsToFile(const std::string& filename, const std::vector<Tweet>& tweets) 
{
    std::ofstream outputFile(filename);
    
    //checking if file opened correctly
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    outputFile << "tweetID,retweet_count,favorite_count\n";  // write the header

    //loop over each tweet in the vector and write its data to the file
    for (const auto& tweet : tweets) {
        outputFile << tweet.tweetID << "," << tweet.retweetCount << "," << tweet.favoriteCount << "\n";
    }

    outputFile.close();
}