#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>

std::string hash_function(std::string input);
std::vector<std::bitset<8>> text_to_binary(std::string text);
std::vector<unsigned long> binary_to_hex(std::vector<std::bitset<8>> binary);

int main()
{
    std::string input = "As esu Justas Kuzma", output;

    std::vector<std::bitset<8>> input_in_binary = text_to_binary(input);
    std::vector<unsigned long> input_in_hex = binary_to_hex(input_in_binary);

    return 0;
}

// Function which does the hashing and returns the output
std::string hash_function(std::string input)
{
    std::string output;



    return output;
}

/* Function which takes a string and returns a vector
   where each element is a single character of the string turned into a binary expression */
std::vector<std::bitset<8>> text_to_binary(std::string text)
{
    std::vector<std::bitset<8>> text_in_binary;

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        text_in_binary.push_back(std::bitset<8>(text[i]));
    }

    return text_in_binary;
}

/* Function which takes a vector of binary expressions and
   returns a vector where each element is turned from binary to unsigned long */
std::vector<unsigned long> binary_to_hex(std::vector<std::bitset<8>> binary)
{
    std::vector<unsigned long> hex;

    for (auto &i : binary)
    {
        hex.push_back(i.to_ulong());
    }

    return hex;
}