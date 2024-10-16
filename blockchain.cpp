#include "hash.h"
#include "blockchain.h"

int main()
{
    std::ifstream users("users.txt");
    std::ofstream transactions("transactions.txt");
    std::string characters = "0123456789abcdef";

    std::random_device rd;  // Non-deterministic random number generator
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> users_dis(1, 1000);
    std::uniform_real_distribution<double> amount_dis(0.000001, 2000000.0);
    std::string line;

    for (int i = 0; i < 10000; ++i)
    {
        int random_user1;
        int random_user2;
        double amount = amount_dis(gen);
        do
        {
            random_user1 = users_dis(gen);
            random_user2 = users_dis(gen);
        } while (random_user1 == random_user2);

        User sender, recipient;

        line = get_specific_line(users, random_user1);
        sender = read_user_data(line);
        line = get_specific_line(users, random_user2);
        recipient = read_user_data(line);

        transactions << hash_function(sender.public_key + recipient.public_key + std::to_string(amount)) << ','
        << sender.public_key << ',' << recipient.public_key << ',' << std::fixed << std::setprecision(6) << amount << std::endl;
    }

    users.close();
    transactions.close();

    return 0;
}