#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>

struct User
{
    std::string name, public_key;
    double balance;
};

struct Transaction
{
    std::string transaction_id, sender, recipient;
    double amount;
};

User read_user_data(std::string line)
{
    User user;
    std::stringstream ss(line);
    std::getline(ss, user.name, ',');
    std::getline(ss, user.public_key, ',');
    std::string balance_str;
    std::getline(ss, balance_str, ',');
    user.balance = std::stod(balance_str);

    return user;
}

#endif