#include "hash.h"
#include "blockchain.h"

int main()
{
    // Load the blockchain from a file at the start
    const std::string blockchainFile = "blockchain.json";
    Blockchain blockchain(blockchainFile);

    const std::string transactionFile = "transactions.txt";
    const std::string userFile = "users.txt";
    int difficulty = 4;

    // Add a new block by mining
    blockchain.mineAndAddBlock(transactionFile, userFile, difficulty);

    // Save the updated blockchain back to the file
    blockchain.saveToFile(blockchainFile);

    return 0;
}