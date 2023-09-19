# webserv

[HTTP Server: Everything you need to know to Build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[server web en C from scratch](servWebC/server%20web%20en%20C%20from%20scratch%20f3a66cec98944c1289af54934492bb0a.md)

[HyperText Transfer Protocol (HTTP)](https://http.dev/)

## To do:

- [ ]  Parsing du fichier de config
- [ ]  Gestion des requetes avec les sockets
- [ ]  Parsing des requetes
- [ ]  Creation de reponse
- [ ]  Gestion des CGI

## Creation du coeur du programme

Etre capable de recevoir et de l’analyser pour ensuite renvoyer la bonne reponse.

### Fonctions utilisees

```cpp
//convertion local vers reseau (vers big-endian)
short htons(short value);
long htonl(long value);
//convertion inverse (de big-endian)
short ntohs(short value);
long ntohl(long value);

int socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//AF_INET  pour ipv4, SOCK_STREAM pour TCP et IPPROTO_TCP pour TCP
//retourne -1 lors d'une erreur
int close(int socket) //pour fermer le socker qui est un simple fd
```

![epollSchema](servWeb/epollSchema.png)

[C++ Confused by epoll and socket fd on Linux systems and async threads](https://stackoverflow.com/questions/66916835/c-confused-by-epoll-and-socket-fd-on-linux-systems-and-async-threads)

Creation d’une classe tcp_server, celle-ci permet de creer un server tcp sur un port donne.

Cette classe contiendra le fd du server tcp et les fd des differents clients.

On pourra avec cela utiliser epoll pour gerer differents server en meme temps et identifier de quel server provient le fd actif.
