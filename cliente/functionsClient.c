#include "functionsClient.h"



//----->>>>>Funciones relacionadas con la conexion del socket<<<<<-----

void obtenerDireccionServidor(char * direccion){
    char direccionServidor[25];
    
    printf("Introduzca la direccion del servidor (default %s):", DIRECCION_DEFAULT);
    
    fgets(direccionServidor, sizeof(direccionServidor), stdin);
    
    if (direccionServidor[0] == '\n'){
        strcpy(direccionServidor, DIRECCION_DEFAULT);
    }

    strcpy(direccion, direccionServidor);
}

int obtenerPuertoDeConexion(){
    char auxPuerto[10] = "";

    printf("Introduzca el puerto de la conexion (default %hd):", PUERTO_DEAFULT);
    fgets(auxPuerto, sizeof(auxPuerto), stdin);

    if (auxPuerto[0] == '\n'){
        return PUERTO_DEAFULT;
    } else {
        return atoi(auxPuerto);
    }
}

struct sockaddr_in configurarConexion(short tipo_conexion, int puerto, char direccion[]){
    struct sockaddr_in servidor;
 
    servidor.sin_family = tipo_conexion;
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = inet_addr(direccion);

    return servidor;
}

int crearSocket(short tipo_conexion){
    int socket_descriptor = socket(tipo_conexion, SOCK_STREAM, 0);

    if(socket_descriptor < 0){
        perror("--->>>Error al crear socket");
        cerrarConexion(socket_descriptor);
        exit(1);
    }
    return socket_descriptor;
}

void conectarSocket(int socketDescriptor, struct sockaddr_in servidor){
    if(connect(socketDescriptor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0){
        perror("--->>>Error al conectar socket");
        cerrarConexion(socketDescriptor);
        exit(1);
    }
}

int iniciarCliente(short tipo_conexion){
    char direccionServidor[15];

    obtenerDireccionServidor(direccionServidor);
    
    int puerto = obtenerPuertoDeConexion();

    struct sockaddr_in servidor = configurarConexion(tipo_conexion, puerto, direccionServidor);

    int socket_descriptor = crearSocket(tipo_conexion);

    conectarSocket(socket_descriptor, servidor);

    return socket_descriptor;
}

void cerrarConexion(int socketDescriptor){
    if(close(socketDescriptor) < 0){
        perror("--->>>Error al cerrar socket");
        exit(1);
    }
}



//----->>>>>Funciones de Comunicación<<<<<-----

void desbloquearProgramaServidor(int socketDescriptor){
    size_t bytes = send(socketDescriptor, CODIGO_MENSAJE_RECIBIDO, 
        sizeof(CODIGO_MENSAJE_RECIBIDO), 0);

    if(bytes < 0){
        perror("--->>>Error al enviar mensaje");
        cerrarConexion(socketDescriptor);
        exit(1);
    } else if(bytes == 0){
        cerrarConexion(socketDescriptor);
        exit(1);
    }
}

bool funcionBaseMandarMensaje(int socketDescriptor){
    bool mensajeDeSalida = false;
    char mensaje[512] = "";
    fgets(mensaje, sizeof(mensaje), stdin);

    if(strlen(mensaje) == 1){
        strcpy(mensaje, MENSAJE_VACIO);
    } else {
        mensaje[strcspn(mensaje, "\n")] = '\0';    
    }

    size_t bytes = send(socketDescriptor, mensaje, strlen(mensaje), 0);

    if (bytes < 0){
        perror("--->>>Error al recibir mensaje");
        cerrarConexion(socketDescriptor);
        exit(1);
    } else if((strcmp(mensaje, MENSAJE_LOGOUT) == 0)){
        mensajeDeSalida = true;
    }

    return mensajeDeSalida;
}

bool mandarMensaje(int socketDescriptor){
    return funcionBaseMandarMensaje(socketDescriptor);
}

int funcionBaseRecibirMensaje(int socketDescriptor){
    char mensaje[512] = "";
    int resultado;

    size_t bytes = recv(socketDescriptor, mensaje, sizeof(mensaje), 0);

    if(bytes < 0){
        printf("--->>>Error al recibir mensaje");
        cerrarConexion(socketDescriptor);
        exit(1);
    } else if(strcmp(mensaje, CODIGO_LIMPIAR_PANTALLA) == 0){
        return OPCION_LIMPIAR_PANTALLA;
    } else if(strcmp(mensaje, CODIGO_SERVIDOR_ESPERA_ENTRADA) == 0){
        return OPCION_ENTRADA;
    } else if(strcmp(mensaje, CODIGO_PAUSAR_PANTALLA) == 0){
        return OPCION_PAUSAR_PANTALLA;
    }else {
        printf("%s", mensaje);
        return OPCION_MENSAJE;
    } 
}

int recibirMensaje(int socketDescriptor){
    int entrada = funcionBaseRecibirMensaje(socketDescriptor);
    if(entrada != OPCION_ENTRADA){
        desbloquearProgramaServidor(socketDescriptor);
    }
    return entrada;
}



//----->>>>>Funciones de la aplicacion<<<<<-----

void interaccionConServidor(int socketDescriptor){
    bool mensajeSalida = false;
    int esperaServidor;

    while(!mensajeSalida){
        esperaServidor = recibirMensaje(socketDescriptor);

        if(esperaServidor == OPCION_LIMPIAR_PANTALLA){
            system("clear");
        }else if(esperaServidor == OPCION_PAUSAR_PANTALLA){
            PARA_PROGRAMA_CLIENTE;
        }

        if(esperaServidor == OPCION_ENTRADA){
            mensajeSalida = mandarMensaje(socketDescriptor);
        }
    }
    cerrarConexion(socketDescriptor);
}