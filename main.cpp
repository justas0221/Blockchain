#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <iomanip>

#define BLOCK_SIZE 64

// Initial constants (Cube roots of first eight prime numbers)
const uint32_t H[8] =
{
    0x429A2FC8, 0x717744FC, 0xB6184A88, 0xE99E4B5D,
    0x395C0A74, 0x59E4FC7E, 0x924D0A1D, 0xAB0F92F7
};

std::string hash_function(std::string input);
std::vector<std::bitset<8>> text_to_binary(std::string text);
std::vector<unsigned long> binary_to_hex(std::vector<std::bitset<8>> binary);
void initialize(uint32_t state[8]);
void padding(const uint8_t* message, size_t length, uint8_t** paddedMessage, size_t* paddedLength);

int main()
{
    const char* input = "As esu Justas Kuzma";
    size_t input_length = std::strlen(input);

    uint8_t* padded_input;
    size_t padded_length;


    // Free the allocated memory for padded input
    delete[] padded_input;


    uint32_t state[8];
    uint8_t hash[32];

    return 0;
}

// Function which does the hashing and returns the output
std::string hash_function(std::string input)
{
    std::string output;
    std::vector<std::bitset<8>> input_in_binary = text_to_binary(input);

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

// Initialize the state with the initial constants
void initialize(uint32_t state[8])
{
    for (int i = 0; i < 8; i++)
    {
        state[i] = H[i];
    }
}

void padding(const uint8_t* input, size_t input_length, uint8_t** padded_input, size_t* padded_length)
{
    // Calculate the padding length
    size_t total_length = input_length + 1 + 8;
    size_t remaining_length = total_length % BLOCK_SIZE;
    size_t pad_length = (BLOCK_SIZE - remaining_length) % BLOCK_SIZE;

    *padded_length = input_length + 1 + pad_length + 8;
    *padded_input = new uint8_t[*padded_length];

    // Copy the original message
    std::memcpy(*padded_input, input, input_length);

    // Append the '1' bit followed by 7 zero bits
    (*padded_input)[input_length] = 0x80;
    std::memset(*padded_input + input_length + 1, 0, pad_length);

    // Append the length in bits as a 64-bit integer
    uint64_t bit_length = static_cast<uint64_t>(input_length) * 8;
    for (int i = 0; i < 8; ++i)
    {
        (*padded_input)[*padded_length - 1 - i] = bit_length & 0xFF;
        bit_length >>= 8;
    }
}
