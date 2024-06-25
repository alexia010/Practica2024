# Practica2024
DNS look-up using sockets

**Plan de implementare a aplicatiei:**

- Realizarea conexiunii client-server 
  - Implementarea unui server UDP simplu care să primeasca si sa proceseze cereri de la clienti 
  - Dezvoltarea unui client UDP care sa poata trimite cereri catre server
  
- Crearea pachetului de tip DNS     
  - Definirea structurii pachetului DNS conform specificatiilor *RFC 1035*
  - Implementarea functionalitatilor pentru a construi si analiza pachete DNS pentru extragerea informatiilor necesare din cererile si raspunsurile DNS
    
-   Interogarea serverului DNS
    - Implementarea logicii necesare pentru a trimite cereri DNS catre serverele DNS configurate în fișierul */etc/resolv.conf*
  
- Filtrarea si procesarea rezultatelor  
    - Dezvoltarea logicii pentru filtrarea rezultatelor în funcție de tipul cererii DNS (A, AAAA, CNAME etc.)
    - Gestionarea cazurilor de eroare si a raspunsurilor negative ( ex: domeniul introdus nu a fost gasit)
- Creare unui logger la nivelul apilicatie server
  
## 17.06.2024

modify README


## 18.06.2024

Realizarea primei parti a conexiunii client-server pentru aplicatia de DNS lookup. Am implementat serverul astfel incat sa poata accepta si gestiona multiple cereri de la clienti. Am folosit file descriptori si un vector de socket-uri pentru clienti, impreuna cu un socket de ascultare configurat in mod non-blocking pentru a permite gestionareamai multor conexiuni simultane. 

Pentru a gestiona eficient multiple conexiuni simultane, am configurat socket-ul de ascultare in mod non-blocking folosind functia fcntl, ceea ce permite serverului sa continue sa proceseze alte conexiuni si cereri chiar daca unele operatii de I/O nu sunt imediat disponibile. Am utilizat fd_set si functia select pentru a monitoriza file descriptorii activi. Atunci cand un nou client se conecteaza, socket-ul acestuia este adaugat la fd_set si in vectorul de socketuri pentru a fi luat in considerare .

Totodata, am configurat spatiul de lucru in Visual Studio Code, creand fisierele tasks.json si launch.json pentru automatizarea procesului de build si debugging, si un Makefile la nivel de proiect.

## 19.06.2024

Am reconsiderat implementarea realizata in ziua anterioara, renuntand la file descriptori si vectorul de socketuri din cadrul aplicatiei server, realizand doar o conexiune simpla UDP intre client si server 

*Surse de ocumentare:*

>[ sursa_1](https://mislove.org/teaching/cs4700/spring11/handouts/project1-primer.pdf) -  formatul packetului DNS 
 
 >[ sursa_2 ](https://www.baeldung.com/linux/etc-resolv-conf-file) - rolul si configurarea fisierului /etc/resolv.conf în sistemele Linux. Acesta contine informatii despre serverele DNS utilizate pentru traducerea numelor de domenii in adrese IP. Prin intermediul directivei nameserver, fisierul specifica adresele IP ale serverelor DNS pe care sistemul le utilizeaza.

 ## 20.06.2024

 Realizarea claselor care definesc pachetul DNS si componentele sale.Am definit clase pentru header-ul DNS, intrebari (queries), inregistrari de raspuns (answers),autoritati si pentru inregistrari suplimentare.Totodata, am extras si adresele IP ale serverelor DNS folosite local.

 ## 21.06.2024

 Adaugarea functionalitatilor necesare în cadrul claselor care definesc pachetul DNS. Definirea unui enum pentru diferitele tipuri de înregistrări DNS, cum ar fi A, NS, CNAME, SOA, PTR și MX.

 ## 25.06.2024

Am renuntat la utilizarea serverelor DNS locale, stocate in cadrul fisierului /etc/resolv.conf, optand pentru serverele DNS open source, ale caror adrese IP si nume sunt stocate in fisierul *dns_servers.txt*. Am finalizat implementarea trimiterii cererii DNS de la client la server si, ulterior, de la server la serverul DNS specificat. Raspunsul de la serverul DNS este apoi transmis inapoi la serverul c++ si, in final, la client.