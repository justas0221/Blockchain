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

const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185bf, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

std::string hash_function(std::string input);
std::vector<std::bitset<8>> text_to_binary(std::string text);
std::vector<unsigned long> binary_to_hex(std::vector<std::bitset<8>> binary);
void initialize(uint32_t state[8]);
void padding(const uint8_t* input, size_t input_length, uint8_t** padded_message, size_t* padded_length);
void process_block(uint32_t state[8], const uint8_t block[BLOCK_SIZE]);

int main()
{
    const char* input = "";
    size_t input_length = std::strlen(input);

    uint8_t* padded_input;
    size_t padded_length;

    // Step 1: Initialize hash state
    uint32_t state[8];
    initialize(state);

    // Step 2: Pad the input message
    padding(reinterpret_cast<const uint8_t*>(input), input_length, &padded_input, &padded_length);

    // Step 3: Process each 64-byte block of the padded message
    for (size_t i = 0; i < padded_length; i += BLOCK_SIZE) {
        process_block(state, padded_input + i);
    }

    // Step 4: Final hash output
    std::cout << "Hash: ";
    for (int i = 0; i < 8; ++i) {
        std::cout << std::hex << std::setw(8) << std::setfill('0') << state[i];
    }
    std::cout << std::dec << std::endl;

    // Free the allocated memory for padded_input
    delete[] padded_input;

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

void process_block(uint32_t state[8], const uint8_t block[BLOCK_SIZE])
{
    // Message schedule array
    uint32_t W[64];

    // Prepare the message schedule
    for (int i = 0; i < 16; ++i)
    {
        W[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) | (block[i * 4 + 2] << 8) | (block[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i)
    {
        uint32_t s0 = (W[i - 15] >> 6 | W[i - 15] << (32 - 6)) ^ 
                    (W[i - 15] >> 12 | W[i - 15] << (32 - 12)) ^ 
                    (W[i - 15] >> 5);

        uint32_t s1 = (W[i - 2] >> 14 | W[i - 2] << (32 - 14)) ^ 
                    (W[i - 2] >> 23 | W[i - 2] << (32 - 23)) ^ 
                    (W[i - 2] >> 9);

        W[i] = W[i - 16] + s0 + W[i - 7] + s1;
    }

    // Initialize working variables
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

    // Compression function main loop
    for (int i = 0; i < 64; ++i)
    {
        uint32_t S1 = (e >> 6 | e << (32 - 6)) ^ 
                      (e >> 11 | e << (32 - 11)) ^ 
                      (e >> 25 | e << (32 - 25));
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t temp1 = h + S1 + ch + K[i] + W[i];
        uint32_t S0 = (a >> 2 | a << (32 - 2)) ^ 
                      (a >> 13 | a << (32 - 13)) ^ 
                      (a >> 22 | a << (32 - 22));
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t temp2 = S0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}