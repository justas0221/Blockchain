#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

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
    
    // Setteris for balance (with validation)
    void setBalance(double new_balance)
    {
        if (new_balance < 0)
        {
            throw std::invalid_argument("Balance cannot be negative");
        }
        balance = new_balance;
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
    // Constructor
    Block(const std::string& prev_hash, const std::string& version, int difficulty)
        : prev_block_hash(prev_hash), timestamp(std::time(nullptr)), nonce(0), difficulty_target(difficulty), version(version) {}

    // Add transaction to the block
    void addTransaction(const Transaction& transaction)
    {
        transactions.push_back(transaction);
        merkle_root = computeMerkleRoot();
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
};

#endif