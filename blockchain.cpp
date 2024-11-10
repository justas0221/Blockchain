#include "hash.h"
#include "blockchain.h"
#include <iostream>
#include <string>

int main()
{
    // Load the blockchain from a file at the start
    const std::string blockchainFile = "blockchain.json";
    Blockchain blockchain(blockchainFile);

    const std::string transactionFile = "transactions.txt";
    const std::string userFile = "users.txt";
    int difficulty = 4;

    while (true)
    {
        // Check if there are transactions left to process
        if (std::filesystem::file_size(transactionFile) > 0)
        {
            blockchain.mineAndAddBlock(transactionFile, userFile, difficulty);
            blockchain.saveToFile(blockchainFile);
        }
        else
        {
            std::cout << "No transactions left to mine." << std::endl;
            std::cout << "Would you like to:\n"
                      << "1. Generate more transactions and users\n"
                      << "2. Print a block\n"
                      << "3. Print a transaction\n"
                      << "4. Exit\n"
                      << "Enter your choice (1-4): ";

            int choice;
            std::cin >> choice;

            if (choice == 1)
            {
                // Call functions to generate new transactions and users
                generateTransactions();
                generateUsers();
                std::cout << "Generated new transactions and users." << std::endl;
            }
            else if (choice == 2)
            {
                std::cout << "Enter block index to print: ";
                int blockIndex;
                std::cin >> blockIndex;
                blockchain.printBlockByIndex(blockIndex);
            }
            else if (choice == 3)
            {
                std::cout << "Enter block index: ";
                int blockIndex;
                std::cin >> blockIndex;

                std::cout << "Enter transaction index within block: ";
                int transactionIndex;
                std::cin >> transactionIndex;

                blockchain.printTransactionByIndices(blockIndex, transactionIndex);
            }
            else if (choice == 4)
            {
                std::cout << "Exiting program." << std::endl;
                break;
            }
            else
            {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }
    }

    return 0;
}
