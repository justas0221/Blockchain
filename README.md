# Blockchain

## V0.4

Ši versija yra pradinė mano sukurtos blokų grandinės realizacija. Norint ja naudotis reikia įsidiegti gcc kompiliatorių ir clonint'i mano repozitoriją iš Github'o. Tuomet per CMD naviguoti iki failo į kurį klonavote repozitoriją, build'inti programą naudojant komandą "g++ -o blockchain blockchain.cpp sha256.cpp" ir paleisti ją naudojant komandą "./blockchain". Tuomet iš atsitiktinai sugeneruotų transakcijų bus atsitiktinai išrinkta 10 ir jos bus pridedamos į naują bloką, jeigu siuntėjų sąskaitų likučiai bus pakankami, jog galėtume įvykdyti tas transakcijas. Po to faile "blockchain.json" galite matyti kiekvieną bloką ir jo sandarą, faile "users.txt" - visų naudotojų duomenis ir atnaujinamus balansus, faile "transactions.txt" - transakcijas, kurios dar yra neįvykdytos.