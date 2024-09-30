# Blockchain

## Pseudo-kodas

### Main funkcija:

Pradžia
Nustatyti įvestį
Kviesti hash_function(input) ⟶ (gauti įvesties eilutės maišos vertę)
Atspausdinti "Maiša: [hash_function rezultatas]"
Pabaiga

### Funkcija hash_function(input)

Inicijuoti state[8] ⟶ (inicijuoti būseną pradinėmis konstantomis)
Kviesti padding(input) ⟶ (gauti užpildytą pranešimą)
Ciklas per kiekvieną 64 baitų bloką iš užpildyto pranešimo:
Kviesti process_block(state, block) ⟶ (atnaujinti būseną kiekvienam blokui)
Konvertuoti galutinę state[8] reikšmę į šešioliktainį skaičių
Grąžinti šešioliktainę eilutę kaip galutinį maišos rezultatą

### Funkcija initialize(state)

Nustatyti state[0...7] į iš anksto nustatytas konstantas
Grąžinti

### Funkcija padding(input)

Apskaičiuoti visą ilgį, reikalingą užpildymui ⟶ (įtraukti 1 bitą + užpildymą + 8 baitus ilgiui)
Paskirti atmintį padded_input
Kopijuoti pradinį input į padded_input
Pridėti 0x80 bitą (1, po kurio eina 7 nuliai) po įvesties
Pridėti nulius, kad ilgis būtų suderintas su 512 bitų daugikliu
Pridėti pradinio input ilgį (bitais) kaip 64 bitų sveikąjį skaičių
Grąžinti padded_input ir padded_length

### Funkcija process_block(state, block)

Paruošti pranešimo masyvą W[64]:
Pirmieji 16 žodžių (1 žodis - 4 ASCII simboliai) iš block
Kiti 48 žodžiai gaunami naudojant ankstesnius žodžius
Inicijuoti kintamuosius a, b, c, d, e, f, g, h iš state[0...7]
Ciklas 64 kartus:
Apskaičiuoti S1, ch, temp1, S0, maj, temp2
Atnaujinti a, b, c, d, e, f, g, h
Atnaujinti state[0...7] su a, b, c, d, e, f, g, h
Grąžinti