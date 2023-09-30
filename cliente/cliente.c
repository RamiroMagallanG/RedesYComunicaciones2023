/*
    Author: Ramiro Hernan Magallán Gonzalez.
    Purpose: Create the basis of a client.

    Description: Only test the handling of
        sockets in of sockets in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include "functionsClient.h"

int main(){
    int socket = iniciarCliente(AF_INET);

    interaccionConServidor(socket);

    return 0;
}
