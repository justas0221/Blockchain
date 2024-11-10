# Blockchain

## V0.5

Ši programa yra paprasta blokų grandinės (angl. blockchain) implementacija, leidžianti naudotojui generuoti ir įrašyti blokus, juos peržiūrėti bei apdoroti transakcijas per CLI (komandinės eilutės sąsają).

### Reikalavimai

g++ kompiliatorius: Įsitikinkite, kad jūsų sistemoje yra įdiegtas g++ (GCC) kompiliatorius.
Repozitorija: Atsisiųskite arba nusiklonuokite šį projektą į savo aplinką:
```
git clone <repozitorijos-nuoroda>
```
C++17 arba naujesnė C++ versija.

### Paleidimo instrukcijos

1. Kodo kompiliavimas
Atidarykite terminalą ir eikite į nusiklonuotą projekto aplanką. Tuomet vykdykite šią komandą, kad sukurtumėte vykdomąjį failą:

```
g++ -std=c++17 main.cpp -o blockchain_program
```

2. Programos paleidimas
Kai kompiliacija baigta, paleiskite programą naudodami šią komandą:

```
./blockchain_program
```

3. Failų paruošimas
Programa naudoja šiuos failus:

blockchain.json: blockchain'o failas, kuriame saugomi blokų duomenys.
transactions.txt: transakcijų failas.
users.txt: naudotojų failas.

Užtikrinkite, kad šie failai egzistuoja tame pačiame aplanke kaip ir vykdomasis failas, arba programa juos sukurs pirmo paleidimo metu.

### Programos naudojimas

Paleidus programą, main funkcija vykdys šiuos veiksmus:

Blockchain'o nuskaitymas: programa įkels esamą blokų grandinę iš blockchain.json failo.
Transakcijų apdorojimas ir blokų kasimas: jei transactions.txt faile yra transakcijų, programa bandys sugeneruoti naują bloką ir pridėti jį prie grandinės (naudojant nustatytą sunkumo lygį).

Pasirinkimų meniu: jei nėra transakcijų, naudotojas pamatys pasirinkimų meniu:
1. Generuoti naujas transakcijas ir naudotojus: sukuria naujus naudotojus ir transakcijas.
2. Spausdinti bloką: leidžia pasirinkti bloką pagal jo indeksą ir atspausdinti informaciją apie jį.
3. Spausdinti transakciją: leidžia pasirinkti bloką ir konkrečią transakciją jame, kad peržiūrėtumėte informaciją.
4. Baigti programą: išjungia programą.

### Pavyzdys

```
No transactions left to mine.
Would you like to:
1. Generate more transactions and users
2. Print a block
3. Print a transaction
4. Exit
Enter your choice (1-4):
Įvedę skaičių nuo 1 iki 4, galite pasirinkti atitinkamą veiksmą, kurį norite atlikti.
```

### Sunkumo lygio nustatymas

Programa naudoja difficulty kintamąjį (numatytoji reikšmė yra 4), kad nustatytų, kiek nulių turi būti „hash“ pradžioje. Šią reikšmę galite keisti kodo pradžioje, kad padidintumėte ar sumažintumėte kasimo sudėtingumą.

### Programos sustabdymas

Jei norite išeiti iš programos, pasirinkite „4“ pasirinkimų meniu arba paspauskite Ctrl + C bet kuriuo metu terminale.

## Programos ypatybės

