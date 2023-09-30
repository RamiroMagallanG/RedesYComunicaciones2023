/*
    Author: Ramiro Hernan Magallán Gonzalez.
    Purpose: Create the basis of a server.

    Description: Only test the handling of
        sockets in of sockets in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include "functionsServer.h"

int main(){
    int socket_servidor = iniciarServidor(AF_INET);
    
    menuServidor(socket_servidor);

    return 0;
}
