# Blockchain

## Pseudo-kodas

### Main funkcija:

Pradžia<br />
Nustatyti įvestį<br />
Kviesti hash_function(input) ⟶ (gauti įvesties eilutės maišos vertę)<br />
Atspausdinti "Maiša: [hash_function rezultatas]"<br />
Pabaiga<br />

### Funkcija hash_function(input)

Inicijuoti state[8] ⟶ (inicijuoti būseną pradinėmis konstantomis)<br />
Kviesti padding(input) ⟶ (gauti užpildytą pranešimą)<br />
Ciklas per kiekvieną 64 baitų bloką iš užpildyto pranešimo:<br />
Kviesti process_block(state, block) ⟶ (atnaujinti būseną kiekvienam blokui)<br />
Konvertuoti galutinę state[8] reikšmę į šešioliktainį skaičių<br />
Grąžinti šešioliktainę eilutę kaip galutinį maišos rezultatą<br />

### Funkcija initialize(state)

Nustatyti state[0...7] į iš anksto nustatytas konstantas<br />
Grąžinti<br />

### Funkcija padding(input)

Apskaičiuoti visą ilgį, reikalingą užpildymui ⟶ (įtraukti 1 bitą + užpildymą + 8 baitus ilgiui)<br />
Paskirti atmintį padded_input<br />
Kopijuoti pradinį input į padded_input<br />
Pridėti 0x80 bitą (1, po kurio eina 7 nuliai) po įvesties<br />
Pridėti nulius, kad ilgis būtų suderintas su 512 bitų daugikliu<br />
Pridėti pradinio input ilgį (bitais) kaip 64 bitų sveikąjį skaičių<br />
Grąžinti padded_input ir padded_length<br />

### Funkcija process_block(state, block)

Paruošti pranešimo masyvą W[64]:<br />
Pirmieji 16 žodžių (1 žodis - 4 ASCII simboliai) iš block<br />
Kiti 48 žodžiai gaunami naudojant ankstesnius žodžius<br />
Inicijuoti kintamuosius a, b, c, d, e, f, g, h iš state[0...7]<br />
Ciklas 64 kartus:<br />
Apskaičiuoti S1, ch, temp1, S0, maj, temp2<br />
Atnaujinti a, b, c, d, e, f, g, h<br />
Atnaujinti state[0...7] su a, b, c, d, e, f, g, h<br />
Grąžinti<br />