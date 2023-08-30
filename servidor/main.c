/*
    Author: Ramiro Hernan Magallán Gonzalez.
    Purpose: Create the basis of a server.

    Description: Only test the handling of
        sockets in of sockets in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PUERTO 62626

int main(){

    // socket server
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    //Direccion del servidor
    struct sockaddr_in servidor;
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PUERTO);
    servidor.sin_addr.s_addr = INADDR_ANY;

    bind(socket_descriptor, (struct sockaddr*) &servidor, sizeof(servidor));

    listen(socket_descriptor, 1);

    int socket_cliente = accept(socket_descriptor, NULL, NULL);

    close(socket_descriptor);

    return 0;
}
