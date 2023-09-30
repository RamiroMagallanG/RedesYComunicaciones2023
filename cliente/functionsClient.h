#ifndef FUNCTIONS_CLIENT_H
#define FUNCTIONS_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define PUERTO_DEAFULT 5005
#define DIRECCION_DEFAULT "127.0.0.1"

#define TAMANIO_MENSAJE 512
#define MENSAJE_DE_SALIDA "SALIR"
#define MENSAJE_LOGOUT "/LOGOUT"
#define MENSAJE_APAGAR "/APAGAR"
#define MENSAJE_VACIO "MENSAJE_SIN_CARACTERES"

#define CODIGO_MENSAJE_RECIBIDO "RECEPCION"
#define CODIGO_SERVIDOR_ESPERA_ENTRADA "ENTRADA"
#define CODIGO_LIMPIAR_PANTALLA "LIMPIAR_PANTALLA"
#define CODIGO_PAUSAR_PANTALLA "PAUSAR_PROGRAMA"

#define LIMPIAR_PANTALLA system("clear")
#define PARA_PROGRAMA_CLIENTE getchar()

#define OPCION_MENSAJE 0
#define OPCION_LIMPIAR_PANTALLA 1
#define OPCION_ENTRADA 2
#define OPCION_PAUSAR_PANTALLA 3

//----->>>>>funciones de conexion del cliente<<<<<-----

//->Obtiene la direccion del servidor por medio del cliente.
//pre:	Toma como argumento el puntero a la variable que guarda la dirección.
//pos:	No devuelve nada.
void obtenerDireccionServidor(char * direccion);

//->Obtiene el puerto de conexión por medio del cliente.
//pre:	No toma argumentos.
//pos:	Devuelve el puerto.
int obtenerPuertoDeConexion();

//->Configura la estructura sockaddr_in de la conexion.
//pre:	Toma como argumento la familia de la conexión, el numero de puerto y la dirección del servidor.
//pos:	Devuelve la estructura sockaddr_in configurada.
struct sockaddr_in configurarConexion(short tipo_conexion, int puerto, char direccion[]);

//->Crea el socket.
//pre:	Toma como argumento la familia de la conexión, no toma el tipo de conexion.
//pos:	Devuelve el descriptor del socket.
int crearSocket(short tipo_conexion);

//->Conecta el socket al servidor.
//pre:	Toma como argumento el descriptor del socket y la estructura de la conexión.
//pos:	No devuelve nada.
void conectarSocket(int socketDescriptor, struct sockaddr_in servidor);

//->inicia el socket del cliente.
//pre:	Toma como argumento la familia, puerto y direccion de la conexion del cliente
//pos:	Devuelve el descriptor del socket
int iniciarCliente(short tipo_conexion);



//----->>>>>Funciones de Comunicación<<<<<-----

//->Desbloquea al SERVIDOR y le permite que mande un mensaje.
//pre:	Recibe el descriptor del socket.
//pos:	No devuelve nada.
void desbloquearProgramaServidor(int socketDescriptor);

//->Función basica para mandar mensajes
//pre:	Toma como argumento el descriptor del socket.
//pos:	Devuelve un dato booleano:
//			true: Cierra la conexión con el servidor por decision del cliente.
//			false: Continua interactuando con el servidor.
bool funcionBaseMandarMensaje(int socketDescriptor);

//->El cliente manda un mensaje, y se espera la confirmación de recepcion.
//pre:	Recibe un descriptor de socket y una cadena de char para usar de buffer.
//pos:	No devuelve nada, manda el mansaje.
bool mandarMensaje(int socketDescriptor);

//->Función basica para recibir mensajes
//pre:	Toma como argumento el descriptor del socket.
//pos:	Devuelve un dato entero que representa el estado:
//			OPCION_MENSAJE: 0 (Se recibio un mensaje)
//			OPCION_LIMPIAR_PANTALLA: 1 (Se recibio la orden de limpiar la pantalla)
//			OPCION_PAUSAR_PROGRAMA: 2 (Se recibio la orden de pausar el programa)
//			OPCION_ENTRADA: 3 (Se recibio el aviso de que el servidor espera una entrada)
int funcionBaseRecibirMensaje(int socketDescriptor);

//->El cliente recibe un mensaje, y manda la confirmación de que se recibio.
//pre:	Recibe un descriptor de socket.
//pos:	Devuelve un entero, que conside con el estado de funcionBaseRecibirMensaje.
//			Recibe un mensaje y manda la confirmación de recepción.
int recibirMensaje(int socketDescriptor);



//----->>>>>Funciones de la aplicacion<<<<<-----

//->un bucle de recibir y mandar mensajes a un servidor.
//pre:	Toma como argumento el descriptor del socket.
//pos:	No devuelve nada.
void interaccionConServidor(int socketDescriptor);

//->cierra la conexion
//pre:	Recibe un descriptor de socket.
//pos:	No devuelve nada, cierra la conexion del socket.
void cerrarConexion(int socketDescriptor);

#endif