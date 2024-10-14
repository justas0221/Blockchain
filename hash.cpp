#include "hash.h"

int main(int argc, char **argv)
{
    std::string input;
    if (argc == 2)
    {
        read_file(argv[1], input);
    }
    else
    {
        manual_input(input);
    }

    // Call hash function
    std::string hash = hash_function(input);
    
    // Output the result
    std::cout << "Hash: " << hash << std::endl;

    collision_search("test_files/100k_one_difference.txt");
    hex_percentage_difference();
    binary_percentage_difference();
    time_tracking_my_hash();
    time_tracking_sha256();

    return 0;
}