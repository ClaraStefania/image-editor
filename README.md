// Copyright Stefania-Clara Diaconescu 313CAb 2023-2024

    Problema are ca scop realizarea unui editor de text fara interfata
grafica, prin intermediul caruia sa se realizeze operatii asupra imaginilor
formatate PPM sau PGM.

    In 'main()' se aloca spatiu pentru structura 'imag' si se citeste primul
cuvant al comenzii, urmand sa intre in while cat timp acesta este diferit de
'EXIT'.

#### LOAD

    In cazul in care cuvantul este 'LOAD', se citeste numele fisierului, se
apeleaza functia 'incarcare()' si se deschide fisierul. Daca acesta nu exista,
se afiseaza mesajul corespunzator si se elibereaza matricea, daca este vreuna
incarcata. Altfel, se verifica daca este vreo matrice incarcata si se
elibereaza. Se citeste tipul imaginii si se sare peste comentarii. Daca
imaginea este color sau grayscale, se citeste valoarea maxima si se realizeaza
citirea specifica tipului. Daca imaginea e alb-negru/grayscale, valoarea este
salvata in variabila rezervata culorii rosii. Intreaga suprafata va fi
selectata.

#### SELECT

    In cazul in care cuvantul este 'SELECT', se citeste urmatorul cuvant.

    Daca acesta este 'ALL', se apeleaza functia 'selectare_intreg()'. Daca nu
este incarcata nicio imagine, se afiseaza mesajul si se iese din functie.
Altfel, sunt modificate cordonatele astfel incat imaginea sa fie selectata in
intregime.

    In cazul in care este un numar, prima coordonata ia valoarea numarului
citit si se apeleaza functia 'selectare_imag()'. Se citeste restul comenzii.
Daca nu este incarcata nicio imagine, se afiseaza mesajul si se iese din
functie. Altfel, se verifica daca se mai pot citi inca 3 numere sau daca in
comanda se afla si alte cuvinte/numere. Se verifica daca sunt valide
coordonatele introduse. Daca x1 > x2 sau y1 > y2 se apeleaza functia
'interschimbare()' care schimba valorile intre ele. La final coordonatele sunt
modificate.

#### SAVE

    In cazul in care cuvantul este 'SAVE', se citeste numele fisierului si se
apeleaza functia 'salvare()'. Daca nu este incarcata nicio imagine, se afiseaza
mesajul si se iese din functie. Se verifica daca matricea trebuie salvata in
format ascii sau binar prin numararea caracterelor ramase pana la finalul
comenzii. 

    Daca trebuie salvata in format ASCII, se modifica tipul matricei daca este
necesar, se scriu in fisier tipul si valoarea maxima si se apeleaza functia
'afisare_ascii()'. Daca matricea e color, se afiseaza valorile pentru fiecare
culoare. Altfel, se afiseaza doar prima valoare.

     Daca trebuie salvata in format binar, are loc acelasi procedeu, doar ca la
final se apeleaza functia 'afisare_binar()'.

#### CROP

    In cazul in care cuvantul este 'CROP', se apeleaza functia 'decupare()'.
Daca nu este incarcata nicio imagine, se afiseaza mesajul si se iese din
functie. Se aloca memorie pentru matricea auxiliara in care sunt salvate
valorile din regiunea selectata si se modifica dimensiunile. Se elibereaza
memoria alocata pentru matricea initiala si se aloca din nou cu noile
dimensiuni. Valorile sunt trecute din auxiliar in matrice si se elibereaza
memoria alocata auxiliarului. Coordonatele sunt modificate, astfel incat
intreaga imagine sa fie selectata.

#### EQUALIZE

    In cazul in care cuvantul este 'EQUALIZE', se apeleaza functia
'egalizare()'. Daca nu este incarcata nicio imagine, se afiseaza mesajul si se
iese din functie. Se verifica daca imaginea este color sau nu si se
initializeaza cu 0 vectorul de frecventa. Se calculeaza frecventa fiecarei
valori si se afla noile valori ale pixelilor cu ajutorul algoritmului. Se
inlocuiesc noile valori in matrice si se elibereaza memoria alocata pentru
vectorii de frecventa si cel al functiei.

#### APPLY

    In cazul in care cuvantul este 'APPLY', se apeleaza functia 'aplicare()'.
Daca nu este incarcata nicio imagine, se afiseaza mesajul si se iese din
functie. Se verifica validitatea comenzii si se citeste parametrul. Daca
matricea nu este color, se afiseaza mesajul si se iese din functie.

    In functie de parametrul citit, se apeleaza functiile 'edge()', 'sharpen()'
'gaussian_blur()' sau 'blur()' care initializeaza matricea 'efect' cu valorile
specifice filtrului.

    Fiecare dintre aceste functii apeleaza 'aplicare_parametru()', care
calculeaza noua matrice dupa aplicarea nucleului. In variabilele x1, x2, y1
si y2 se salveaza coordonatele selectate. Daca sunt egale cu 0 sau cu
dimensiunile, se adauga/scade 1. Se aloca memorie pentru matricea auxiliara.
Pentru fiecare pixel din regiunea selectata, se calculeaza suma produselor
dintre valorile pixelilor din blocul 3 x 3 si valorile corespunzatoare
ale pixelilor din nucleu. Aceasta se reduce la intervalul [0, 255] si se
rotunjeste. La final se inlocuiesc valorile in matrice si se elibereaza memoria
ocupata de auxiliar.

#### HISTOGRAM

    In cazul in care cuvantul este 'HISTOGRAM', se apeleaza functia
'histograma()'. Daca nu este incarcata nicio imagine, se afiseaza mesajul si
se iese din functie. Se verifica daca se pot citi 2 numere sau daca in comanda
se afla si alte cuvinte/numere. Se verifica daca imaginea este color sau 
alb-negru/grayscale si sunt initializati cu 0 vectorii pentru frecventa si
pentru numarul de stele al fiecarui bin.

    Se calculeaza frecventa fiecarei valori. Cele 256 de valori se impart in 
grupuri de 256/y bin-uri, unde y este numarul maxim de bin-uri. Se calculeaza
sumele frecventelor valorilor din fiecare bin si se afla maximul acestora.
Pentru fiecare bin se calculeaza numarul de stele folosindu-se formula si se
afiseaza histograma. La final se elibereaza memoria alocata.

#### ROTATE

    In cazul in care cuvantul este 'ROTATE', se citeste unghiul cu care va fi
rotita imaginea si se apeleaza functia 'rotire()'. Daca nu este incarcata nicio
imagine, se afiseaza mesajul si se iese din functie. Se verifica daca unghiul
este valid si daca, in cazul in care nu este selectata toata imaginea, selectia
este patrat. Daca aceste conditii nu sunt indeplinite, se afiseaa un mesaj si
se iese din functie.

    Functia 'rot_90()' roteste o matrice alb-negru/grayscale cu 90 de grade
spre stanga. In variabilele x1, x2, y1 si y2 se salveaza coordonatele
selectate. Daca matricea este selectata in intregime, 'ok' devine 1. Se aloca
memorie pentru o matrice auxiliara in care se salveaza transpusa matricei
initiale. Se inverseaza liniile cu ajutorul functiei 'interschimbare()'. Daca
imaginea este selectata in intregime, se elibereaza memoria si se aloca
matricea cu dimensiunile inversate. Se salveaza noile valori si se elibereaza
memoria alocata pentru auxiliar.

    Functia 'rot_90_col()' urmeaza acelasi procedeu, doar ca se aplica pentru
toate cele 3 valori ale pixelilor.

    In functie de valoarea unghiului, se apeleaza functiile o data, de 2 sau
de 3 ori.

#### EXIT

     In cazul in care cuvantul este 'EXIT', se iese din while. Daca este vreo
matrice incarcata, se elibereaza. Altfel, se afiseaza un mesaj. La final se
elibereaza memoria alocata structurii.
