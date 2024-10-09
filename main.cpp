#include "my_hash.h"

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

    return 0;
}