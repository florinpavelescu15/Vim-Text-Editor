Pavelescu Florin, grupa 314CC

Am rezolvat tema în aproximativ o săptămână și jumătate.
Am folosit o parte din funcțiile pe care le-am implementat
pentru *liste* și *stive* în laboratoarele 4 și 5.

# IMPLEMENTARE
Am utilizat o *listă dublu înlănțuită cu santinelă* pentru 
a reține textul asupra căruia trebuie efectuate modificările 
și două *stive* pentru a reține istoricul comenzilor (una pentru `undo` și 
alta pentru `redo`).
Am implementat funcții pentru găsirea unui cuvânt într-un text, 
`backspace`, `delete`, ștergerea unui cuvânt, înlocuirea unui cuvânt, 
salvarea textului în fișierul de ieșire etc.
Am citit textul și comenzile, cu ajutorul unor bucle `while`,
respectând formatul fișierelor de intrare și, folosind funcțiile
definite anterior, am efectuat operațiile, pe rând, asupra textului 
reținut în lista dublu înlănțuită, ținând cont de rigorile impuse în enunț. 
