# Practica2024
DNS look-up using sockets

# 17.06.2024

modify README

# 18.09.2024

Realizarea primei parti a conexiunii client-server pentru aplicatia de DNS lookup. Am implementat serverul astfel incat sa poata accepta si gestiona multiple cereri de la clienti. Am folosit file descriptori si un vector de socket-uri pentru clienti, impreuna cu un socket de ascultare configurat in mod non-blocking pentru a permite gestionareamai multor conexiuni simultane. 

Pentru a gestiona eficient multiple conexiuni simultane, am configurat socket-ul de ascultare in mod non-blocking folosind functia fcntl, ceea ce permite serverului sa continue sa proceseze alte conexiuni si cereri chiar daca unele operatii de I/O nu sunt imediat disponibile. Am utilizat fd_set si functia select pentru a monitoriza file descriptorii activi. Atunci cand un nou client se conecteaza, socket-ul acestuia este adaugat la fd_set si in vectorul de socketuri pentru a fi luat in considerare .

Totodata, am configurat spatiul de lucru in Visual Studio Code, creand fisierele tasks.json si launch.json pentru automatizarea procesului de build si debugging, si un Makefile la nivel de proiect.
