# Encryption-decryption-and-pattern-recognition-in-C.

1) Functia xorshift genereaza numere intregi fara semn aleatoare, utilizand operatii pe biti.
2) Functia openLin are ca scop liniarizarea unei imagini de tip BMP, astfel : se copiaza mai intai header-ul
intr-o variabila de tip unsigned char alocata dinamic, imagedata; se calculeaza padding-ul; apoi citim
pixelii si ii pastram in variabila de tip pixel alocata dinamic. Returnam printr-o structura nr, informatiile
despre imaginea data : header-ul si pixelii.
3) Functia closeLin are ca scop salvarea in memoria externa a unei imagini stocate in forma liniarizata in
memoria interna. Functioneaza astfel : primeste informatiile despre imagine si le stocheaza sub forma
matriceala.
4) Functia criptare genereaza o secventa de numere intregi fara semn aleatoare,prin intermediul functiei
xorshift, apoi genereaza o permutare aleatoare prin intermediul algoritmului lui Durstenfeld si
numerelor generate anterior. Se permuta pixelii imaginii, iar prin intermediul operatiilor pe biti se
amesteca pixelii, generandu-se o noua imagine.
5) Functia decriptare genereaza o secventa de numere, apoi o permutare aleatoare si se aplica asupra
fiecarui pixel din imaginea criptata o relatie, ce consta in operatii pe biti. Imaginea decriptata se obtine
permutand invers pixelii imaginii intermediare.
6) Functia chi_test are ca scop analiza uniformitatii distributiei valorilor pixelilor. Folosim 3 vectori , unul
pentru fiecare pixel, pentru a retine frecventa fiecarei valori.
7) Functia template_matching consta in gasirea unor parti dintr-o imagine care se aseamana cu un
sablon dat. Prin intermediul functiei matrice incarcam un fisier de tip BMP si il transformam intr-o
matrice de tip pixel. O alta functie ajutatoare este corelatie, care are ca rol calculul corelatiei dintre un
sablon si o imagine.
8) Functia Sort sorteaza detectiilor in ordinea descrescatoare a corelatiei. 
