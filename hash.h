#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <cctype>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }


    // IMPORTANT INFO
    // ASCII value of 'a' = 97
    // ASCII value of '0' = 48



    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // array to store up tp fice base-36 int values
        HASH_INDEX_T w[5] = {0};

        // start from end of sring, 6 char at a time
        int chunkIndex = 4;
        int len = static_cast<int>(k.length());

        for (int i = len; i > 0 && chunkIndex >= 0; i -= 6, --chunkIndex)
        {
            HASH_INDEX_T value = 0;

            // compute starting index of this 6-character chunk
            int start = std::max(0, i - 6);

            // process each character in the chunk from left to right
            for (int j = start; j < i; ++j)
            {
                // shift previous digits left by multiplying by 36
                value *= 36;

                // convert current char to base-36 value and add
                char c = std::tolower(k[j]);

                if (c >= 'a' && c <= 'z') {
                    value += c - 'a'; // 0–25
                }
                else if (c >= '0' && c <= '9') {
                    value += c - '0' + 26; // 26–35
                }
            }

            // store the result in the corresponding slot
            w[chunkIndex] = value;
        }

        // compute final hash by summing products with r values
        HASH_INDEX_T h = 0;
        for (int i = 0; i < 5; ++i)
        {
            h += rValues[i] * w[i];
        }

        return h;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // turn lowercase
        letter = std::tolower(letter);

        // if lowercase letter a to z
        if(letter >= 'a' && letter <= 'z') {
            return static_cast<HASH_INDEX_T>(letter - 'a'); // turns into 0-25
        }

        // if digit
        if(letter >= '0' && letter <= '9') {
            return static_cast<HASH_INDEX_T>(letter - '0' + 26); // 26–35
        }

        // invalid char
        return 0;

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
