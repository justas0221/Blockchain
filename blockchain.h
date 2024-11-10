#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>
#include <set>
#include <unordered_set>
#include <omp.h>
#include <atomic>
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
    Block(const std::string& prev_hash, const std::string& version, int difficulty, bool printInfo = true)
        : prev_block_hash(prev_hash), timestamp(std::time(nullptr)), nonce(0), difficulty_target(difficulty), version(version)
    {
        if (printInfo)
        {
            std::cout << "-------------" << std::endl;
            std::cout << "Block Created: " << std::endl;
            std::cout << "Previous Hash: " << prev_hash << std::endl;
            std::cout << "Version      : " << version << std::endl;
            std::cout << "Timestamp    : " << std::ctime(&timestamp) << std::endl;
            std::cout << "-------------" << std::endl;
        }
    }

    // Constructor with all of the members
    Block(const std::string& prev_hash, const std::string& version, int difficulty, std::time_t timestamp, const std::string& merkleRoot, bool printInfo = true)
        : prev_block_hash(prev_hash), timestamp(timestamp), nonce(0), difficulty_target(difficulty), version(version), merkle_root(merkleRoot)
    {
        if (printInfo)
        {
            std::cout << "-------------" << std::endl;
            std::cout << "Block Created: " << std::endl;
            std::cout << "Previous Hash: " << prev_hash << std::endl;
            std::cout << "Version      : " << version << std::endl;
            std::cout << "Timestamp    : " << std::ctime(&timestamp);
            std::cout << "-------------" << std::endl;
        }
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

    bool mineBlock(int difficulty)
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
                return true;
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
        blockJson["timestamp"] = convertTimestampToDate(timestamp);
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
    std::atomic<bool> blockMined{false};
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
        std::vector<Transaction> selectedTransactions = selectRandomTransactionsFromFile(filename, 100);
        
        // Determine the previous block's hash; if no blocks exist, use a default value for genesis
        std::string prevHash = blocks.empty() ? "0" : blocks.back().computeBlockHash();
        
        // Create a new block, ensuring all necessary parameters are passed
        std::string version = "1.0";
        std::time_t timestamp = std::time(nullptr);
        std::string merkleRoot = "";
        Block newBlock(prevHash, version, difficulty, timestamp, merkleRoot);

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
        blockMined = false;
        const int candidateBlockCount = 5;
        const int transactionSampleSize = 100;
        const int maxAttempts = 100000;
        const int maxTimeSeconds = 5;

        int attemptLimit = maxAttempts;
        int timeLimit = maxTimeSeconds;

        // Check if there are transactions in the file
        if (std::filesystem::file_size(transactionFile) == 0)
        {
            std::cout << "No transactions left to mine." << std::endl;
            return;  // Skip the mining process if there are no transactions left
        }

        // Continue with mining if there are transactions left
        while (!blockMined)
        {
            std::vector<Block> candidateBlocks;
            std::vector<std::vector<Transaction>> allProcessedTransactions(candidateBlockCount);

            // Only create candidate blocks if there are transactions to process
            std::ifstream infile(transactionFile);
            std::string line;
            if (!std::getline(infile, line))
            {
                std::cout << "No transactions available to process." << std::endl;
                return;  // If no transaction is found in the file, exit mining
            }
            infile.close();

            // Now start creating candidate blocks
            for (int i = 0; i < candidateBlockCount; ++i)
            {
                std::unordered_map<std::string, double> balanceMap;
                std::ifstream userFileIn(userFile);
                std::string userLine;
                while (std::getline(userFileIn, userLine))
                {
                    User user = User::fromCSV(userLine);
                    balanceMap[user.getPublicKey()] = user.getBalance();
                }
                userFileIn.close();

                Block candidate = createBlockFromSampledTransactions(transactionFile, transactionSampleSize);

                std::vector<Transaction> validTransactions;
                std::vector<Transaction> processedTransactions;

                // Validate transactions
                for (const auto& transaction : candidate.getTransactions())
                {
                    processedTransactions.push_back(transaction);

                    if (validateTransaction(transaction, balanceMap))
                    {
                        balanceMap[transaction.getSenderKey()] -= transaction.getAmount();
                        balanceMap[transaction.getRecipientKey()] += transaction.getAmount();
                        validTransactions.push_back(transaction);
                    }
                }

                candidate.clearTransactions();
                for (const auto& tx : validTransactions)
                {
                    candidate.addTransaction(tx);
                }

                candidateBlocks.push_back(candidate);
                allProcessedTransactions[i] = processedTransactions;
            }

            // Parallelize the mining process
            #pragma omp parallel for
            for (int i = 0; i < candidateBlockCount; ++i)
            {
                if (blockMined.load(std::memory_order_acquire))
                {
                    return;  // Exit mining if a block is already mined
                }

                Block& candidate = candidateBlocks[i];
                auto start = std::chrono::steady_clock::now();
                int attempts = 0;

                while (attempts < attemptLimit &&
                    std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < timeLimit &&
                    !blockMined.load(std::memory_order_acquire))
                {
                    if (candidate.mineBlock(difficulty))
                    {
                        bool expected = false;
                        if (blockMined.compare_exchange_strong(expected, true, std::memory_order_acq_rel))
                        {
                            #pragma omp critical
                            {
                                // Update user balances and save mined block
                                updateBalances(candidate, userFile);

                                std::vector<Transaction> processedTransactions;
                                for (int j = 0; j < candidateBlockCount; ++j)
                                {
                                    processedTransactions.insert(processedTransactions.end(),
                                                                allProcessedTransactions[j].begin(),
                                                                allProcessedTransactions[j].end());
                                }
                                removeTransactions(processedTransactions, transactionFile);

                                blocks.push_back(candidate);
                                std::cout << "Block successfully mined and added to blockchain!" << std::endl;
                            }
                        }
                        break;  // Exit this mining loop if a block is mined
                    }
                    ++attempts;
                }
            }

            if (blockMined)
            {
                break;  // Exit the loop if a block has been mined
            }

            // If no block was mined, increase limits and retry
            attemptLimit *= 2;
            timeLimit *= 2;
            std::cout << "Increasing mining limits. New attempt limit: " << attemptLimit << ", new time limit: " << timeLimit << " seconds." << std::endl;
        }
    }

    bool validateTransaction(const Transaction& transaction, const std::unordered_map<std::string, double>& balanceMap)
    {
        const std::string& sender = transaction.getSenderKey();
        const std::string& recipient = transaction.getRecipientKey();
        double amount = transaction.getAmount();

        // Check if sender has enough balance
        if (balanceMap.at(sender) < amount)
        {
            std::cout << "Transaction " << transaction.getID() << " denied: Insufficient balance for user " << sender << std::endl;
            return false;
        }

        // Construct a string of transaction details to verify the transaction ID (without timestamp)
        std::string transactionData = sender + recipient + std::to_string(amount);

        // Verify the transaction ID by hashing transactionData
        std::string calculatedHash = hashFunction(transactionData);
        if (calculatedHash != transaction.getID())
        {
            std::cout << "Transaction " << transaction.getID() << " denied: Hash mismatch. Calculated hash: " 
                    << calculatedHash << ", Expected hash: " << transaction.getID() << std::endl;
            return false;
        }

        // Transaction is valid
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

            // Pass `false` to avoid printing block info when loading from file
            Block newBlock(prevHash, version, difficultyTarget, timestamp, merkleRoot, false);

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

    // Print a block by index
    void printBlockByIndex(int index) const
    {
        if (index >= 0 && index < blocks.size())
        {
            blocks[index].printBlock();
        }
        else
        {
            std::cout << "Block index out of range." << std::endl;
        }
    }

    // Print a transaction by block and transaction indices
    void printTransactionByIndices(int blockIndex, int transactionIndex) const
    {
        if (blockIndex >= 0 && blockIndex < blocks.size())
        {
            const Block& block = blocks[blockIndex];
            if (transactionIndex >= 0 && transactionIndex < block.getTransactions().size())
            {
                block.getTransactions()[transactionIndex].printTransaction();
            }
            else
            {
                std::cout << "Transaction index out of range in the specified block." << std::endl;
            }
        }
        else
        {
            std::cout << "Block index out of range." << std::endl;
        }
    }
};

int countLines(const std::string& filename)
{
    std::ifstream file(filename);
    return std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
}

void generateUsers()
{
    std::ifstream file("users.txt");
    std::ofstream outputFile("users.txt", std::ios::app);
    
    if (file.is_open() && outputFile.is_open())
    {
        int lastLine = countLines("users.txt");
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(100.0, 1000000.0);
        
        for (int i = 0; i < 1000; i++)
        {
            std::string name = "user" + std::to_string(lastLine + 1);
            std::string publicKey = hashFunction(name);
            double balance = dis(gen);
            
            outputFile << name << "," << publicKey << "," << balance << std::endl;
            lastLine++;
        }
        
        file.close();
        outputFile.close();
        std::cout << "1000 users generated" << std::endl;
    }
    else
    {
        std::cout << "Unable to open file." << std::endl;
    }
}

void generateTransactions()
{
    int numUsers = countLines("users.txt");

    std::ofstream transactionFile("transactions.txt", std::ios::app);
    if (!transactionFile)
    {
        std::cerr << "Error opening transactions.txt" << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> amountDist(1.0, 50000.0);
    std::uniform_int_distribution<> lineDist(1, numUsers);

    for (int i = 0; i < 1000; ++i)
    {
        std::string senderKey, receiverKey;
        double amount = amountDist(gen);

        std::ifstream userFile("users.txt");
        if (!userFile)
        {
            std::cerr << "Error opening users.txt" << std::endl;
            return;
        }

        int senderLine = lineDist(gen);
        int receiverLine = lineDist(gen);

        while (senderLine == receiverLine)
        {
            receiverLine = lineDist(gen);
        }

        for (int j = 1; j <= senderLine; ++j)
        {
            std::getline(userFile, senderKey);
        }

        userFile.clear();
        userFile.seekg(0, std::ios::beg);
        for (int j = 1; j <= receiverLine; ++j)
        {
            std::getline(userFile, receiverKey);
        }

        userFile.close();

        senderKey = senderKey.substr(senderKey.find(',') + 1, senderKey.rfind(',') - senderKey.find(',') - 1);
        receiverKey = receiverKey.substr(receiverKey.find(',') + 1, receiverKey.rfind(',') - receiverKey.find(',') - 1);

        std::string transactionData = senderKey + receiverKey + std::to_string(amount);
        std::string transactionID = hashFunction(transactionData);

        transactionFile << transactionID << "," << senderKey << "," << receiverKey << "," << std::fixed << std::setprecision(6) << amount << std::endl;
    }

    transactionFile.close();
    std::cout << "1000 transactions generated" << std::endl;
}

#endif