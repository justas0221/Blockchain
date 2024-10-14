#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <ios>
#include <chrono>
#include <random>
#include <vector>
#include <numeric>
#include "sha256.h"

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

std::string hash_function(const std::string& input);
void initialize(uint32_t state[8]);
void padding(const uint8_t* input, size_t input_length, uint8_t** padded_message, size_t* padded_length);
void process_block(uint32_t state[8], const uint8_t block[BLOCK_SIZE]);
void read_file(char *argv, std::string &input);
void manual_input(std::string &input);
void time_tracking_my_hash();
void time_tracking_sha256();
void read_file_time(std::string& input, int line_count);
void generate_strings();
void collision_search(std::string file_name);
std::string hex_to_binary(const std::string& hex);
void hex_percentage_difference();
void binary_percentage_difference();
std::string generate_salt();
void add_salt_to_file(std::string file_name);

// Function which does the hashing and returns the final output as a string
std::string hash_function(const std::string& input)
{
    uint32_t state[8];
    initialize(state);

    uint8_t* padded_input;
    size_t padded_length;
    
    // Pad the input
    padding(reinterpret_cast<const uint8_t*>(input.c_str()), input.size(), &padded_input, &padded_length);

    // Process each 64-byte block of the padded message
    for (size_t i = 0; i < padded_length; i += BLOCK_SIZE)
    {
        process_block(state, padded_input + i);
    }

    // Convert the final hash state to a hex string
    std::stringstream hash_output;
    for (int i = 0; i < 8; ++i)
    {
        hash_output << std::hex << std::setw(8) << std::setfill('0') << state[i];
    }

    delete[] padded_input;
    return hash_output.str();
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

void read_file(char *argv, std::string& input)
{

    std::ifstream input_file;
    input_file.open(argv, std::ios_base::binary);

    if (input_file)
    {
        std::stringstream ss;
        ss << input_file.rdbuf();
        input = ss.str();
    }
    else
    {
        std::cerr << "Failed to open file: " << argv << std::endl;
        exit(0);
    }

    input_file.close();
}

void manual_input(std::string &input)
{
    std::cout << "Ivestis: ";
    std::getline(std::cin, input);
}

void time_tracking_my_hash()
{
    const int repetitions = 5;

    for (int i = 1; i < 1025; i *= 2)
    {
        std::string input;
        std::string hash;
        read_file_time(input, i);

        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < repetitions; j++)
        {
            hash = hash_function(input);
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start) / repetitions;

        std::cout << "Hash: " << hash << " Time: " << duration.count() << " microseconds" << std::endl;
    }
}

void time_tracking_sha256()
{
    const int repetitions = 5;

    for (int i = 1; i < 1025; i *= 2)
    {
        std::string input;
        std::string hash;
        read_file_time(input, i);

        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < repetitions; j++)
        {
            hash = sha256(input);
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start) / repetitions;

        std::cout << "Hash: " << hash << " Time: " << duration.count() << " microseconds" << std::endl;
    }
}

void read_file_time(std::string& input, int line_count)
{
    std::ifstream input_file("test_files/konstitucija.txt");
    if (input_file)
    {
        std::stringstream ss;
        for (int i = 0; i < line_count; i++)
        {
            std::getline(input_file, input);
            ss << input;
        }
        input = ss.str();
    }
    else
    {
        std::cerr << "Failed to open file: konstitucija.txt" << std::endl;
        exit(0);
    }

    input_file.close();
}

void generate_strings()
{
    std::ofstream output("test_files/100k_same.txt", std::ios::app);
    const std::string CHARACTERS = "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<> char_distribution(0, CHARACTERS.size() - 1);
    std::uniform_int_distribution<> index_distribution(0, 999);

    for (int i = 0; i < 25000; ++i)
    {
        std::string random_string1;
        std::string random_string2;
        int random_index = index_distribution(generator);

        for (int j = 0; j < 1000; ++j)
        {
            char random_symbol = CHARACTERS[char_distribution(generator)];
            random_string1 += random_symbol;
            // random_string2 += random_symbol;
        }

        // do
        // {
        //     random_string1[random_index] = CHARACTERS[char_distribution(generator)];
        //     random_string2[random_index] = CHARACTERS[char_distribution(generator)];
        // } while (random_string1[random_index] == random_string2[random_index]);

        output << random_string1 << "," << random_string1 << std::endl;
    }

    output.close();
}

void collision_search(std::string file_name)
{
    std::ifstream file(file_name);
    int collision_count = 0;
    std::string string1, string2, line;

    while (!file.eof())
    {
        std::getline(file, line);
        std::stringstream ss(line);

        std::getline(ss, string1, ',');
        std::getline(ss, string2);

        if (hash_function(string1) == hash_function(string2))
        {
            collision_count++;
        }
    }

    std::cout << "Collision count: " << collision_count << std::endl;

    file.close();
}

std::string hex_to_binary(const std::string& hex) {
    std::string binary_data;
    binary_data.reserve(hex.size() * 4);

    for (size_t i = 0; i < hex.size(); ++i) {
        unsigned int hex_digit = std::stoi(hex.substr(i, 1), nullptr, 16);
        binary_data += std::bitset<4>(hex_digit).to_string();
    }
    return binary_data;
}

void hex_percentage_difference()
{
    std::ifstream file("test_files/100k_one_difference.txt");
    double min_difference = 100, max_difference = 0, avg_difference, percentage;
    std::string line, string1, string2, hash1, hash2;
    int matches_count = 0;
    std::vector<double> percentages;

    while (!file.eof())
    {
        std::getline(file, line);
        std::stringstream ss(line);

        std::getline(ss, string1, ',');
        std::getline(ss, string2);

        hash1 = hash_function(string1);
        hash2 = hash_function(string2);
        
        for (int i = 0; i < 64; ++i)
        {
            if (hash1[i] == hash2[i])
            {
                matches_count++;
            }
        }

        percentage = 100 - (double(matches_count / 64.0) * 100);
        if (percentage > max_difference)
        {
            max_difference = percentage;
        }
        if (percentage < min_difference)
        {
            min_difference = percentage;
        }
        percentages.push_back(percentage);
        matches_count = 0;
    }

    avg_difference = std::accumulate(percentages.begin(), percentages.end(), 0) / 100000.0;

    std::cout << "Average difference: " << avg_difference << "%" << std::endl;
    std::cout << "Min: " << min_difference << "%" << " Max: " << max_difference << "%" << std::endl;

    file.close();
}

void binary_percentage_difference()
{
    std::ifstream file("test_files/100k_one_difference.txt");
    double min_difference = 100, max_difference = 0, avg_difference, percentage;
    std::string line, string1, string2, hash1, hash2;
    int matches_count = 0;
    std::vector<double> percentages;

    while (!file.eof())
    {
        std::getline(file, line);
        std::stringstream ss(line);

        std::getline(ss, string1, ',');
        std::getline(ss, string2);

        hash1 = hash_function(string1);
        hash2 = hash_function(string2);

        std::string binary_hash1 = hex_to_binary(hash1);
        std::string binary_hash2 = hex_to_binary(hash2);
        
        for (int i = 0; i < 256; ++i)
        {
            if (binary_hash1[i] == binary_hash2[i])
            {
                matches_count++;
            }
        }

        percentage = 100 - (double(matches_count / 256.0) * 100);
        if (percentage > max_difference)
        {
            max_difference = percentage;
        }
        if (percentage < min_difference)
        {
            min_difference = percentage;
        }
        percentages.push_back(percentage);
        matches_count = 0;
    }

    avg_difference = std::accumulate(percentages.begin(), percentages.end(), 0) / 100000.0;

    std::cout << "Average difference: " << avg_difference << "%" << std::endl;
    std::cout << "Min: " << min_difference << "%" << " Max: " << max_difference << "%" << std::endl;

    file.close();
}

std::string generate_salt()
{
    const std::string CHARACTERS = "abcdefghijklmnopqrstuvwxyz";
    std::string salt;
    const int salt_length = 5;
    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<> char_distribution(0, CHARACTERS.size() - 1);
    for (int i = 0; i < salt_length; ++i)
    {
        char random_symbol = CHARACTERS[char_distribution(generator)];
        salt += random_symbol;
    }

    return salt;
}

void add_salt_to_file(std::string file_name)
{
    std::ifstream input_file(file_name);
    size_t pos = file_name.find(".txt");
    
    if (pos != std::string::npos)
    {
        file_name.erase(pos, 4);
    }
    std::ofstream output_file(file_name + "_salt.txt");
    std::string line;

    while(std::getline(input_file, line))
    {
        std::stringstream ss(line);
        std::string first, second, random_salt1, random_salt2;
        
        if (std::getline(ss, first, ',') && std::getline(ss, second))
        {
            do
            {
                random_salt1 = generate_salt();
                random_salt2 = generate_salt();
            } while (random_salt1 == random_salt2);
            
            first += generate_salt();
            second += generate_salt();
            output_file << first << "," << second << std::endl;
        }
    }

    input_file.close();
    output_file.close();
}