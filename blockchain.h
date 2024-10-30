#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>
#include <set>
#include <unordered_set>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string convertTimestampToDate(std::time_t timestamp)
{
    std::tm* tm = std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

class User
{
private:
    std::string name;
    std::string public_key;
    double balance;

public:
    // Constructor
    User(const std::string& name, const std::string& public_key, double balance) 
        : name(name), public_key(public_key), balance(balance) {}
    
    // Getters
    std::string getName() const { return name; }
    std::string getPublicKey() const { return public_key; }
    double getBalance() const { return balance; }
    
    // Setter for balance (with validation)
    void setBalance(double new_balance)
    {
        if (new_balance < 0)
        {
            throw std::invalid_argument("Balance cannot be negative");
        }
        balance = new_balance;
    }

    void updateBalance(double amount)
    {
        if (balance + amount < 0)
        {
            throw std::invalid_argument("Balance cannot go negative.");
        }
        balance += amount;
    }

    json toJson() const
    {
        return
        {
            {"name", name},
            {"public_key", public_key},
            {"balance", balance}
        };
    }

    static User fromJson(const json& j)
    {
        try
        {
            if (!j.contains("name") || !j.contains("public_key") || !j.contains("balance"))
            {
                throw std::invalid_argument("Missing required fields in JSON");
            }
            return User(
                j["name"].get<std::string>(),
                j["public_key"].get<std::string>(),
                j["balance"].get<double>()
            );
        } catch (const json::exception& e)
        {
            throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
        }
    }

    static User fromCSV(const std::string& line)
    {
        std::stringstream ss(line);
        std::string name, public_key, balance_str;
        std::getline(ss, name, ',');
        std::getline(ss, public_key, ',');
        std::getline(ss, balance_str, ',');
        double balance = std::stod(balance_str);

        return User(name, public_key, balance);
    }
};

class Transaction
{
private:
    std::string transaction_id;
    std::string sender;
    std::string recipient;
    double amount;
public:
    // Constructor
    Transaction(const std::string& transaction_id, const std::string& sender, const std::string& recipient, double amount) 
        : transaction_id(transaction_id), sender(sender), recipient(recipient), amount(amount) {}
    
    // Getters
    std::string getID() const { return transaction_id; }
    std::string getSenderKey() const { return sender; }
    std::string getRecipientKey() const { return recipient; }
    double getAmount() const { return amount; }

    json toJson() const
    {
        return
        {
            {"transaction_id", transaction_id},
            {"sender", sender},
            {"recipient", recipient},
            {"amount", amount}
        };
    }

    static Transaction fromJson(const json& j)
    {
        try
        {
            if (!j.contains("transaction_id") || !j.contains("sender") || !j.contains("recipient") || !j.contains("amount"))
            {
                throw std::invalid_argument("Missing required fields in JSON");
            }
            return Transaction(
                j["transaction_id"].get<std::string>(),
                j["sender"].get<std::string>(),
                j["recipient"].get<std::string>(),
                j["amount"].get<double>()
            );
        } catch (const json::exception& e)
        {
            throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
        }
    }

    static Transaction fromCSV(const std::string& line)
    {
        std::stringstream ss(line);
        std::string transaction_id, sender, recipient, amount_str;
        std::getline(ss, transaction_id, ',');
        std::getline(ss, sender, ',');
        std::getline(ss, recipient, ',');
        std::getline(ss, amount_str, ',');
        double amount = std::stod(amount_str);

        return Transaction(transaction_id, sender, recipient, amount);
    }

    void printTransaction() const
    {
        std::cout << "---------------------------------" << std::endl;
        std::cout << "Transaction ID : " << transaction_id << std::endl;
        std::cout << "Sender         : " << sender << std::endl;
        std::cout << "Recipient      : " << recipient << std::endl;
        std::cout << "Amount         : " << std::fixed << std::setprecision(2) << amount << " coins" << std::endl;
        std::cout << "---------------------------------" << std::endl;
    }
};

class Block
{
private:
    std::string prev_block_hash;
    std::string version;
    std::string merkle_root;
    std::time_t timestamp;
    int nonce;
    int difficulty_target;
    std::vector<Transaction> transactions;

public:
    // Constructor with only mandatory members
    Block(const std::string& prev_hash, const std::string& version, int difficulty)
        : prev_block_hash(prev_hash), timestamp(std::time(nullptr)), nonce(0), difficulty_target(difficulty), version(version)
        {
            std::cout << "-------------" << std::endl;
            std::cout << "Block Created: " << std::endl;
            std::cout << "Previous Hash: " << prev_hash << std::endl;
            std::cout << "Version      : " << version << std::endl;
            std::cout << "Timestamp    : " << std::ctime(&timestamp) << std::endl;
            std::cout << "-------------" << std::endl;
        }

    // Constructor with all of the members
    Block(const std::string& prev_hash, const std::string& version, int difficulty, std::time_t timestamp, const std::string& merkleRoot)
        : prev_block_hash(prev_hash), timestamp(timestamp), nonce(0), difficulty_target(difficulty), version(version), merkle_root(merkleRoot)
        {
            std::cout << "-------------" << std::endl;
            std::cout << "Block Created: " << std::endl;
            std::cout << "Previous Hash: " << prev_hash << std::endl;
            std::cout << "Version      : " << version << std::endl;
            std::cout << "Timestamp    : " << std::ctime(&timestamp);
            std::cout << "Merkle Root  : " << merkleRoot << std::endl;
            std::cout << "-------------" << std::endl;
        }

    // Getter methods
    std::string getPreviousHash() const
    {
        return prev_block_hash;
    }

    std::string getVersion() const
    {
        return version;
    }

    std::string getMerkleRoot() const
    {
        return merkle_root;
    }

    std::time_t getTimestamp() const
    {
        return timestamp;
    }

    int getNonce() const
    {
        return nonce;
    }

    int getDifficultyTarget() const
    {
        return difficulty_target;
    }

    // Add transaction to the block
    void addTransaction(const Transaction& transaction)
    {
        transactions.push_back(transaction);
        std::cout << "Transaction " << transaction.getID() << " added to Block with Previous Hash: " 
                  << prev_block_hash << std::endl;
        merkle_root = computeMerkleRoot();
    }

    // Getter for transactions
    const std::vector<Transaction>& getTransactions() const
    {
        return transactions;
    }

    // Computation of the Merkle root
    std::string computeMerkleRoot()
    {
        if (transactions.empty())
            return "";

        std::vector<std::string> hashes;
       
        for (const auto& transaction : transactions)
        {
            hashes.push_back(hashFunction(transaction.getID()));
        }

        while (hashes.size() > 1)
        {
            std::vector<std::string> new_hashes;

            for (size_t i = 0; i < hashes.size(); i += 2)
            {
                if (i + 1 < hashes.size())
                {
                    new_hashes.push_back(hashFunction(hashes[i] + hashes[i + 1]));
                }
                else
                {
                    new_hashes.push_back(hashFunction(hashes[i] + hashes[i]));
                }
            }

            hashes = new_hashes;
        }

        return hashes.empty() ? "" : hashes.front();
    }

    std::string computeBlockHash()
    {
        // Concatenate the block header details
        std::string blockData = prev_block_hash + merkle_root + std::to_string(timestamp) + std::to_string(nonce);
        return hashFunction(blockData);
    }

    Transaction getTransaction(size_t index) const
    {
        if (index < transactions.size())
            return transactions[index];
        throw std::out_of_range("Transaction index out of range");
    }

    void mineBlock(int difficulty)
    {
        std::string target(difficulty, '0');
        nonce = 0;

        while (true) {
            std::string blockData = prev_block_hash + merkle_root + std::to_string(timestamp) + std::to_string(nonce);
            std::string hash = hashFunction(blockData);

            // Check if the hash meets the difficulty target
            if (hash.substr(0, difficulty) == target)
            {
                std::cout << "Block mined! Nonce: " << nonce << ", Hash: " << hash << std::endl;
                break;
            }
            nonce++;
        }
    }

    void printBlock() const
    {
        std::cout << "=================================" << std::endl;
        std::cout << "Block Version       : " << version << std::endl;
        std::cout << "Previous Block Hash : " << prev_block_hash << std::endl;
        std::cout << "Merkle Root         : " << merkle_root << std::endl;
        std::cout << "Timestamp           : " << std::ctime(&timestamp);
        std::cout << "Nonce               : " << nonce << std::endl;
        std::cout << "Difficulty Target   : " << difficulty_target << std::endl;
        std::cout << "Transactions        : " << transactions.size() << " transactions" << std::endl;
        std::cout << "=================================" << std::endl;

        // Print each transaction in the block
        for (const auto& transaction : transactions)
        {
            transaction.printTransaction();
        }
    }

    json toJson() const
    {
        json blockJson;
        blockJson["prev_block_hash"] = prev_block_hash;
        blockJson["version"] = version;
        blockJson["merkle_root"] = merkle_root;
        blockJson["timestamp"] = convertTimestampToDate(timestamp); // Call const function
        blockJson["nonce"] = nonce;
        blockJson["difficulty_target"] = difficulty_target;

        // Convert each transaction to JSON
        json transactionsJson = json::array();
        for (const auto& transaction : transactions)
        {
            transactionsJson.push_back(transaction.toJson());
        }
        blockJson["transactions"] = transactionsJson;

        return blockJson;
    }

    static Block fromJson(const json& j)
    {
        Block block(j["prev_block_hash"], j["version"], j["difficulty_target"]);
        block.timestamp = j["timestamp"];
        block.nonce = j["nonce"];
        block.merkle_root = j["merkle_root"];

        // Rebuild transactions from JSON array
        for (const auto& txJson : j["transactions"])
        {
            block.transactions.push_back(Transaction::fromJson(txJson));
        }

        return block;
    }

    void clearTransactions()
    {
        transactions.clear();
        merkle_root = computeMerkleRoot();  // Recompute the Merkle root for the updated transactions
    }
};

class Blockchain
{
private:
    std::vector<Block> blocks;                    // The main blockchain
    std::vector<Transaction> pendingTransactions; // List of unconfirmed transactions
public:
    Blockchain()
    {
        // Create and add a genesis block
        Block genesisBlock("0", "1.0", 4); // Previous hash "0" (no previous block), version "1.0", difficulty level of 4
        blocks.push_back(genesisBlock);
    }

    Blockchain(const std::string& filename)
    {
        // Attempt to load blocks from the file
        loadFromFile(filename);

        // Check if blockchain is empty after loading; if so, add a genesis block
        if (blocks.empty()) 
        {
            // Create and add a genesis block
            Block genesisBlock("0", "1.0", 4); // Previous hash "0" (no previous block), version "1.0", difficulty of 4
            blocks.push_back(genesisBlock);
        }
    }

    std::vector<Transaction> selectRandomTransactionsFromFile(const std::string& filename, int sampleSize)
    {
        std::ifstream file(filename);
        std::string line;
        std::vector<Transaction> sampleTransactions;
        
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file " + filename);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        int lineNumber = 0;

        // Reservoir sampling
        while (std::getline(file, line))
        {
            Transaction transaction = Transaction::fromCSV(line);

            if (lineNumber < sampleSize)
            {
                // Fill initial reservoir with transactions
                sampleTransactions.push_back(transaction);
            }
            else
            {
                // Replace elements with decreasing probability
                std::uniform_int_distribution<> distr(0, lineNumber);
                int j = distr(gen);
                if (j < sampleSize)
                {
                    sampleTransactions[j] = transaction;
                }
            }
            ++lineNumber;
        }

        file.close();
        return sampleTransactions;
    }

    Block createBlockFromSampledTransactions(const std::string& filename, int difficulty)
    {
        std::vector<Transaction> selectedTransactions = selectRandomTransactionsFromFile(filename, 10);
        
        // Determine the previous block's hash; if no blocks exist, use a default value for genesis
        std::string prevHash = blocks.empty() ? "0" : blocks.back().computeBlockHash();
        
        // Create a new block, ensuring all necessary parameters are passed
        std::string version = "1.0"; // Example version
        std::time_t timestamp = std::time(nullptr); // Get the current time for the block's timestamp
        std::string merkleRoot = ""; // Compute this based on transactions later if needed
        Block newBlock(prevHash, version, difficulty, timestamp, merkleRoot); // Pass all necessary parameters

        // Add sampled transactions to the new block
        for (const auto& tx : selectedTransactions)
        {
            newBlock.addTransaction(tx);
        }

        return newBlock;
    }

    User getUserFromFile(const std::string& public_key, const std::string& filename)
    {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string name, key;
            double balance;

            std::getline(ss, name, ',');
            std::getline(ss, key, ',');
            ss >> balance;

            if (key == public_key) {
                return User(name, key, balance);
            }
        }
        throw std::runtime_error("User not found");
    }

    void updateBalances(const Block& block, const std::string& filename)
    {
        std::unordered_map<std::string, double> balanceChanges;

        // Calculate balance changes from transactions in the block
        for (const auto& transaction : block.getTransactions())
        {
            balanceChanges[transaction.getSenderKey()] -= transaction.getAmount();
            balanceChanges[transaction.getRecipientKey()] += transaction.getAmount();
        }

        // Read all users, update their balances, and write back to the file
        std::ifstream infile(filename);
        std::vector<std::string> users;
        std::string line;

        while (std::getline(infile, line))
        {
            std::stringstream ss(line);
            std::string name, key;
            double balance;
            std::getline(ss, name, ',');
            std::getline(ss, key, ',');
            ss >> balance;

            // Update balance if there are changes
            if (balanceChanges.find(key) != balanceChanges.end())
            {
                balance += balanceChanges[key];
            }

            users.push_back(name + "," + key + "," + std::to_string(balance));
        }
        infile.close();

        // Write back updated user data to the file
        std::ofstream outfile(filename);
        for (const auto& userLine : users)
        {
            outfile << userLine << std::endl;
        }
        outfile.close();
    }

    void removeTransactions(const std::vector<Transaction>& addedTransactions, const std::string& transactionFile)
    {
        // Read all transactions from the file
        std::ifstream inFile(transactionFile);
        std::vector<std::string> allTransactions;
        std::string line;

        while (std::getline(inFile, line))
        {
            allTransactions.push_back(line);
        }
        inFile.close();

        // Collect transaction IDs of added transactions for easy lookup
        std::unordered_set<std::string> addedTransactionIDs;
        for (const auto& tx : addedTransactions)
        {
            addedTransactionIDs.insert(tx.getID());
        }

        // Filter out transactions that were added
        std::vector<std::string> remainingTransactions;
        for (const auto& transactionLine : allTransactions)
        {
            Transaction tempTransaction = Transaction::fromCSV(transactionLine);

            // Only keep transactions that were not added to the block
            if (addedTransactionIDs.find(tempTransaction.getID()) == addedTransactionIDs.end())
            {
                remainingTransactions.push_back(transactionLine);
            }
        }

        // Write remaining transactions back to the file
        std::ofstream outFile(transactionFile, std::ios::trunc); // Truncate file to rewrite
        for (const auto& transactionLine : remainingTransactions)
        {
            outFile << transactionLine << "\n";
        }
        outFile.close();
    }

    void mineAndAddBlock(const std::string& transactionFile, const std::string& userFile, int difficulty)
    {
        // Create a temporary balance map
        std::unordered_map<std::string, double> balanceMap;

        // Load user balances into balanceMap from userFile
        std::ifstream infile(userFile);
        std::string line;
        while (std::getline(infile, line))
        {
            User user = User::fromCSV(line);
            balanceMap[user.getPublicKey()] = user.getBalance();
        }
        infile.close();

        // Create a new block with sampled transactions
        Block newBlock = createBlockFromSampledTransactions(transactionFile, difficulty);

        // Validate each transaction in the block
        std::vector<Transaction> validTransactions;
        for (const auto& transaction : newBlock.getTransactions())
        {
            const std::string& sender = transaction.getSenderKey();
            const std::string& recipient = transaction.getRecipientKey();
            double amount = transaction.getAmount();

            // Check if sender has enough balance in the temporary balance map
            if (balanceMap[sender] >= amount)
            {
                // Update the temporary balances
                balanceMap[sender] -= amount;
                balanceMap[recipient] += amount;

                // Add valid transaction to the block
                validTransactions.push_back(transaction);
            }
            else
            {
                std::cout << "Transaction " << transaction.getID() << " denied: Insufficient balance for user " 
                        << sender << std::endl;
            }
        }

        // Clear existing transactions and add only the validated ones
        newBlock.clearTransactions();
        for (const auto& tx : validTransactions)
        {
            newBlock.addTransaction(tx);
        }

        // Mine the block
        newBlock.mineBlock(difficulty);

        // Update balances in userFile based on valid transactions
        updateBalances(newBlock, userFile);
        removeTransactions(validTransactions, transactionFile);

        // Add the mined block to the blockchain
        blocks.push_back(newBlock);
    }

    bool validateAndAddTransaction(const Transaction& transaction, const std::string& usersFile)
    {
        User sender = getUserFromFile(transaction.getSenderKey(), usersFile);
        User recipient = getUserFromFile(transaction.getRecipientKey(), usersFile);
        
        // Check if the sender has enough balance
        if (sender.getBalance() < transaction.getAmount())
        {
            std::cout << "Transaction denied: Insufficient balance for user " << sender.getName() << std::endl;
            return false;
        }

        // Update the sender and recipient's balances temporarily in memory
        sender.updateBalance(-transaction.getAmount());
        recipient.updateBalance(transaction.getAmount());

        // Add the transaction to pending transactions
        pendingTransactions.push_back(transaction);
        std::cout << "Transaction added: " << transaction.getID() << std::endl;
        return true;
    }

    void loadFromFile(const std::string& filename)
    {
        std::ifstream inputFile(filename);
        if (!inputFile.is_open())
        {
            std::cerr << "Failed to open the file: " << filename << std::endl;
            return;
        }

        json blockchainJson;

        // Check if the file is empty
        if (inputFile.peek() == std::ifstream::traits_type::eof())
        {
            std::cerr << "The file is empty: " << filename << std::endl;
            inputFile.close();
            return;
        }

        try {
            inputFile >> blockchainJson; // Deserialize JSON data
        } catch (const std::exception& e) {
            std::cerr << "Error reading JSON data: " << e.what() << std::endl;
            inputFile.close();
            return;
        }

        // Check if the loaded JSON is an array (which we expect)
        if (!blockchainJson.is_array())
        {
            std::cerr << "Invalid JSON format: expected an array." << std::endl;
            inputFile.close();
            return;
        }

        for (const auto& blockJson : blockchainJson)
        {
            // Additional checks can be added here for blockJson fields if necessary

            std::string prevHash = blockJson["prev_block_hash"];
            std::string version = blockJson["version"];
            std::string timestampStr = blockJson["timestamp"]; // Read the timestamp as a string

            std::tm tm = {};
            std::istringstream ss(timestampStr);

            // Parse the timestamp string
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            std::time_t timestamp = std::mktime(&tm); // Convert to time_t

            if (ss.fail())
            {
                std::cerr << "Failed to parse timestamp: " << timestampStr << std::endl;
                continue; // Skip this block or handle error as needed
            }

            int nonce = blockJson["nonce"];
            int difficultyTarget = blockJson["difficulty_target"];
            std::string merkleRoot = blockJson["merkle_root"];

            Block newBlock(prevHash, version, difficultyTarget, timestamp, merkleRoot);

            // Load transactions
            for (const auto& txJson : blockJson["transactions"])
            {
                newBlock.addTransaction(Transaction::fromJson(txJson));
            }

            blocks.push_back(newBlock);
        }

        inputFile.close();
    }

    void saveToFile(const std::string& filename) const
    {
        nlohmann::json blockchainJson;

        for (const Block& block : blocks)
        {
            nlohmann::json blockJson;
            
            // Convert timestamp to a human-readable format
            std::string formattedTimestamp = convertTimestampToDate(block.getTimestamp());

            blockJson["version"] = block.getVersion();
            blockJson["difficulty_target"] = block.getDifficultyTarget();
            blockJson["merkle_root"] = block.getMerkleRoot();
            blockJson["nonce"] = block.getNonce();
            blockJson["prev_block_hash"] = block.getPreviousHash();
            
            // Use formatted date string instead of Unix timestamp
            blockJson["timestamp"] = formattedTimestamp;

            nlohmann::json transactionsJson = nlohmann::json::array();
            for (const Transaction& transaction : block.getTransactions()) {
                nlohmann::json transactionJson;
                transactionJson["transaction_id"] = transaction.getID();
                transactionJson["sender"] = transaction.getSenderKey();
                transactionJson["recipient"] = transaction.getRecipientKey();
                transactionJson["amount"] = transaction.getAmount();
                transactionsJson.push_back(transactionJson);
            }
            
            blockJson["transactions"] = transactionsJson;
            blockchainJson.push_back(blockJson);
        }

        // Save the JSON data to file
        std::ofstream file(filename);
        file << std::setw(4) << blockchainJson << std::endl;
        file.close();
    }
};

#endif