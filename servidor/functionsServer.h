#ifndef FUNCTIONS_SERVER_H
#define FUNCTIONS_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define TAMANIO_MENSAJE 512
#define PUERTO_DEFAULT 5005

//--->>>Nombres Archivos<<<---
#define ARCHIVO_CREDENCIALES "credenciales.txt"
#define ARCHIVO_TRADUCCIONES "traducciones.txt"
#define ARCHIVO_LOG "server.log"

#define METODO_READING_PLUS "r+"
#define METODO_READING "r"
#define METODO_ESCRITURA "a"

//--->>>Roles de usuario<<<---
#define ROL_ADMINISTRADOR "ADMIN"
#define ROL_CONSULTA "CONSULTA"

//--->>>Codigos de resultado<<<---

//->aciertos
#define ROL_SUFICIENTE true
#define INICIO_DE_SESSION_CORRECTO 0
#define USUARIO_VALIDO 1
#define TRADUCCION_ENCONTRADA 4
#define TRADUCCION_AGREGADA 6
#define USUARIO_AGREGADO 7
#define CADENA_NO_VACIA 8
#define USUARIO_DESBLOQUEADO 9
#define FORMATO_CORRECTO 10

//->errores
#define ERROR_ROL_INSUFICIENTE false
#define ERROR_CADENA_VACIA false
#define	ERROR_DE_PROCESAMIENTO -1
#define ERROR_CREACION_USUARIO -1
#define TRADUCCION_NO_ENCONTRADA -2
#define ERROR_TRADUCCION_REPETIDA -3
#define ERROR_USUARIO_DATOS_VACIOS -4
#define ERROR_USUARIO_EXISTENTE -5
#define ERROR_FALTA_PERMISO -6
#define ERROR_USUARIO_NO_ENCONTRADO -7
#define ERROR_FORMATO -8

#define	CUENTA_INEXISTENTE 1
#define CUENTA_BLOQUEADA 2
#define CONTRASEÑA_INCORRECTA 3

//--->>>Comandos de cliente<<<---
#define CODIGO_CLIENTE_RECEPCION "RECEPCION"
#define CODIGO_ENTRADA_CLIENTE "ENTRADA"
#define MENSAJE_VACIO "MENSAJE_SIN_CARACTERES"
#define MENSAJE_LIMPIAR_PANTALLA "LIMPIAR_PANTALLA"
#define MENSAJE_PAUSAR_PROGRAMA "PAUSAR_PROGRAMA"

//--->>>Mensajes del servidor<<<---
#define MENSAJE_INGRESAR_USERNAME "Ingrese su nombre de usuario:"
#define MENSAJE_INGRESAR_CONTRASENIA "Ingrese su contrasenia:"
#define MENSAJE_BIENVENIDA "\n\nBienvenido al servidor de traduccion.\n\n"
#define MENSAJE_INGRESAR_COMANDO "¿Qué desea hacer?:"
#define MENSAJE_PEDIDO_PALABRA_A_TRADUCIR "¿Que palabra quiere traducir?:"
#define MENSAJE_SUBMENU_USUARIOS "¿Quiere dar de alta un usuario o debloquearlo?:"
#define MENSAJE_CON_TRADUCCION "%s en ingles es %s en espaniol.\n"
#define MENSAJE_NO_SE_ENCONTRO_TRADUCCION "\nNo fue posible encontrar la traduccion para: %s.\n"
#define MENSAJE_DEFAULT "\nNo se reconoce el comando.\n"
#define MENSAJE_NUEVA_TRADUCCION "\nInserte una nueva traduccion en formato <ingles>:<espaniol>:"
#define MENSAJE_ERROR_FORMATO_N_TRADUCCION "\nNo fue posible insertar traduccion, el formato debe ser <palabraIngles>:<palabraEspañol>.\n"
#define MENSAJE_ERROR_TRADUCCION_REPETIDA "\nYa existe una traduccion para %s: %s.\n"
#define MENSAJE_TRADUCCION_GUARDADA "\nNueva traduccion insertada correctamente.\n"
#define MENSAJE_USUARIO_EXISTENTE "\nError al dar de alta el nuevo usuario: usuario existente.\n"
#define MENSAJE_USUARIO_CREADO "%s dado de alta correctamente.\n"
#define MENSAJE_NUEVO_USERNAME "\nIngrese un usarname:"
#define MENSAJE_NUEVA_CONTRASENIA "\nIngrese una contrasenia:"
#define MENSAJE_ROL_NUEVO_USUARIO "¿Rol admin? (y/n):"
#define MENSAJE_ERROR_FALTA_PERMISOS "\nNo tiene permitida esta accion con su actual rol.\n"
#define MENSAJE_LISTA_USUARIOS_BLOQUEADOS "-%s.\n"
#define MENSAJE_USUARIO_BLOQUEADO "\nSe ha superado la cantida maxima de intentos, la cuenta %s ha sido bloqueada.\n\n"
#define MENSAJE_ERROR_NO_SE_ENCONTRARON_BLOQUEADOS "\nNo se encontraron usuarios bloqueados.\n"
#define MENSAJE_ELEGIR_DESBLOQUEAR_USUARIO "¿Cual usuario quiere desbloquear?:"
#define MENSJAE_USUARIO_DESBLOQUEADO "\n%s desbloqueado correctamente.\n"
#define MENSAJE_ERROR_LISTAR_BLOQUEADOS "\nError al listar usuarios bloqueados.\n"
#define MENSAJE_ERROR_DESBLOQUEAR_USUARIOS "vError en el proceso de desbloqueo.\n"
#define MENSAJE_ERROR_DATOS_VACIOS "\nError al dar de alta el nuevo usuario: datos incompletos.\n"
#define MENSAJE_ESPERAR_ENTER "\nPresione enter para salir al menu principal."
#define MENSAJE_CONTRASENIA_INCORRECTA "Datos de usuario incorrectos.\n"
#define USUARIO_BLOQUEADO "\nEl usuario se encuentra bloqueado.\n"
#define USUARIO_NO_ENCONTRADO "\nNo se encontro usuario bloqueado con ese username.\n"

//--->>>Mensajes LOG<<<---
#define LOG_FECHA_HORA_BASE "%d-%02d-%02dT%02d:%02d:%02d: "
#define LOG_ERROR_CREAR_SOCKET "Error al crear socket.\n"
#define LOG_EXITO_CREAR_SOCKET "Socket creado.\n"
#define LOG_ERROR_VINCULAR_SOCKET "Error al Vincular socket.\n"
#define LOG_EXITO_VINCULAR_SOCKET "Socket vinculado, puerto:%d.\n"
#define LOG_ERROR_ESPERAR_CONEXION "Error al esperar conexiones.\n"
#define LOG_EXITO_ESPERAR_CONEXION "Esperando conexiones.\n"
#define LOG_ERROR_ACEPTAR_CONEXION "Error al aceptar conexion.\n"
#define LOG_EXITO_ACEPTAR_CONEXION "Conexion aceptada, ip:%d.\n"
#define LOG_ERROR_CERRAR_SOCKET "Error al cerrar socket.\n"
#define LOG_EXITO_CERRAR_SOCKET "Socket cerrado.\n"
#define LOG_EXITO_RECIBIR_MENSAJE "Mensaje recibido.\n"
#define LOG_EXITO_MANDAR_MENSAJE "Mensaje mandado\n"
#define LOG_ERROR_USUARIO_INICIO_SESION "Inicio de sesion fallido:no existe usuario.\n"
#define LOG_ERROR_CONTRASENIA_INICIO_SESION "Inicio de sesion fallido:contrasenia incorrecta.\n"
#define LOG_ERROR_BLOQUEADO_INICIO_SESION "Inicio de sesion fallido:cuenta bloqueada.\n"
#define LOG_EXITO_INICIO_SESION "inicio de sesion:%s.\n"


//--->>>Opciones del menu<<<---
#define MENSAJE_LOGOUT "/LOGOUT"
#define USUARIO_LOGOUT 8
#define SALIR_MENU_ACTUAL "/SALIR"
#define USUARIO_QUIERE_SALIR -5
#define OPCION_SALIR_MENU_ACTUAL -3
#define MENU_OPCION_TRADUCIR "/TRADUCIR"
#define OPCION_TRADUCIR 1
#define MENU_NUEVA_TRADUCCION "/NUEVA_TRADUCCION"
#define OPCION_NUEVA_TRADUCCION 2
#define MENU_USUARIOS "/USERS"
#define OPCION_USERS 3
#define SUBMENU_USUARIOS_ALTA "/ALTA"
#define OPCION_ALTA 4
#define SUBMENU_USUARIOS_DESBLOQUEO "/DESBLOQUEO"
#define OPCION_DESBLOQUEO 5
#define MENU_REGISTRO_ACTIVIDADES "/ACTIVIDADES"
#define OPCION_ACTIVIDADES 6
#define MENU_CERRAR_SESION "/LOGOUT"
#define OPCION_CERRAR_SESION 7
#define MENSAJE_APAGAR_SERVIDOR "/APAGAR"
#define OPCION_APAGAR_SERVIDOR 9

//--->>>Paginas de menu<<<---
#define MENU_BASE_TRADUCIR "Traducir palabra -----------> /TRADUCIR\n"
#define MENU_BASE_NUEVA_TRADUCCION "Insertar nueva traduccion --> /NUEVA_TRADUCCION\n"
#define MENU_BASE_USERS "Administracion de usuarios -> /USERS\n"
#define MENU_BASE_ACTIVIDADES "Registro de actividades ----> /ACTIVIDADES\n"
#define MENU_BASE_LOGOUT "Cerrar sesion --------------> /LOGOUT\n"
#define MENU_BASE_APAGAR "Apagar servidor ------------> /APAGAR\n\n"


#define MENU_SUBMENU_USUARIOS_ALTA "\n\nAlta de nuevos usuarios ---> /ALTA\n"
#define MENU_SUBMENU_USUARIOS_DESBLOQUEO "Desbloqueo de usuario -----> /DESBLOQUEO\n" 
#define MENU_SUBMENU_USUARIOS_SALIR "Volver al menu anterior ---> /SALIR\n\n"


//----->>>>>Funciones relacionadas con la conexion del socket<<<<<-----

//->Pregunta al administrador del servidor el puerto al que vincular el socket.
//pre:	No toma argumentos.
//pos:	Devuelve el numero del servidor.
int obtenerPuerto();

//->Configura la estructura del servidor.
//pre:	Toma como argumento el tipo de conexion y el numero de puerto.
//pos:	Devuelve la estructura del servidor ya configurada.
struct sockaddr_in configurarServidor(short tipo_conexion, int puerto);

//->Crea el socket.
//pre:	Toma Como argumento el tipo o familia de conexion, no configura el tipo de socket.
//pos:	Devuelve el descriptor del socket.
int crearSocket(short tipo_conexion);

//->Vincula la configuracion del servidor y la direccion con el socket.
//pre:	Toma como argumento el descriptor del socket y la estructura de configuracion del servidor.
//pos:	No devuelve nada.
void vincular_socket(int socketDescriptor, struct sockaddr_in servidor);

//->Pone en escucha el socket.
//pre:	Toma como argumentos el descriptor del socket.
//pos:	No devuelve nada.
void escuchar_socket(int socketDescriptor);

//->Crea la conexión y pone en espera al servidor.
//pre:	Toma como argumento la familia, puerto y dirección de la conexión del servidor.
//pos:	Devuelve el descriptor del socket.
int iniciarServidor(short tipo_conexion);

//->Acepta conexiones de clientes.
//pre:	Toma un descriptor de socket como argumento.
//pos:	Devuelve el descriptor del socket cliente que se conecto.
int aceptarConexion(int socketDescriptor);

//->Cierra la conexion con el cliente.
//pre:	Toma un descriptor de socket.
//pos:	No devuelve nada.
void cerrarConexion(int socketDescriptor);



//----->>>>>Funciones de Comunicación<<<<<-----

//->Desbloquea al usuario y le permite que mande un mensaje.
//pre:	Recibe el descriptor del socket.
//pos:	No devuelve nada.
void desbloquearProgramaCliente(int socketDescriptor);

//->Función basica para mandar mensajes.
//pre:	Recibe un descriptor de socket, una cadena de char para mandar y el tamaño del mensaje.
//pos:	No devuelve nada, manda el mansaje.
void funcionBaseMandarMensaje(int socketDescriptor, char *mensaje, size_t tamanioMensaje);

//->Mandar mensaje.
//pre:	Recibe un descriptor de socket, una cadena como mensaje y el tamaño del mensaje.
//pos:	Espera a recibir la confirmación de recepción del mensaje, no devuelve nada.
void mandarMensaje(int socketDescriptor, char* mensaje, size_t tamanioMensaje);

//->Función basica para recibir mensajes.
//pre:	Recibe un descriptor de socket, una cadena de char para usar de buffer y el tamaño del mensaje.
//pos:	No devuelve nada, copia el mensaje recibido en la cadena mensaje
void funcionBaseRecibirMensaje(int socketDescriptor, char *mensaje, size_t tamanioMensaje);

//->Recibir mensaje.
//pre:	Recibe un descriptor de socket, una cadena de char para usar de buffer y el tamaño del mensaje.
//pos:	No devuelve nada, recibe un mensaje y manda la confirmación de recepción.
void recibirMensaje(int socketDescriptor, char *mensaje, size_t tamanioMensaje);



//----->>>>>Funciones de la aplicacion<<<<<-----

//------------------------------------------>>>utiles<<<------------------------------------------
//->Funcion general para abrir un archivo.
//pre:	Toma como argumento el nombre del archivo y el metodo, especificado con la macro define.
//pos:	Devuelve un puntero al archivo.
FILE* abrirArchivo(char* nombreArchivo, char* metodo);

//->Pasa a minusculas una palabra.
//pre:	Toma como argumentos un puntero a array y el tamaño de ese puntero.
//pos:	No devuelve nada, copia la palabra formateada al argumento palabra.
void pasarAMinusculas(char* palabra, size_t tamanioPalabra);

//->Comprueba si la cadena recibida esta vacia.
//pre:	Toma un array como argumento.
//pos:	Devuelve un boleano,
//		true:La cadena NO esta vacia
//		false:La cadena esta vacia.
bool comprobarCadenaVacia(char* cadena);

//->Busca al usuario pasado por argumento en el archivo de usuarios.
//pre:	Toma como argumento el nombre del usuario.
//pos:	Devuelve un entero de estado:
//		CUENTA_INEXISTENTE: No se encontro coincidencia con los nombres de usuarios guardados
//		ERROR_USUARIO_EXISTENTE: Hubo una coincidencia.
int existeUsuario(char* username);

//->Comprueba que el rol del usuario sea ROL_ADMINISTRADOR
//pre:	Toma como argumento el rol a verificar, una cadena que contiene el rol deseado, y el descriptor del 
//			socket
//pos:	devuelve un boleano:
//			ROL_SUFICIENTE: true
//			ERROR_ROL_INSUFICIENTE: false
bool comprobarRol(char* rol, char* rolDeseado, int socketDescriptor);

//->Registra una nueva entrada en el archivo log.
//pre:	Toma como argumento el mensaje a guardar, dentro de la función se le agrega la fecha y hora.
//pos:	No devuelve nada.
void registrarLog(char* mensaje);



//------------------------------------------>>>Administracion de usuarios<<<------------------------------------------
//->Comprueba que el username y la contraseña son correctos.
//pre:	Toma como argumento el descriptor del socket, un matriz con dos elementos de tipo cadena, username y 
//			password, y una cadena para copiar el rol
//pos:	Devuelve 0(cero) en caso de que se compruebe que existe la cuenta en el archivo credenciales.txt.
//	devuelve 1(uno) si la cuenta no existe.
//	devuelve 2(dos) si la cuenta esta bloqueada.
//	devuelve 3(tres) si la contraseña es incorrecta.
//	devuelve -1(uno negativo) si hubo algun problema en el procesamiento.
int validarCuenta(int socketDescriptor, char parInicioSesion[][512], char* rol);

//->Proceso de incio de sesion del usuario.
//pre:	Toma como único argumento el descriptor del socket, y una cadena para copiar el rol
//pos:	Devuelve un entero de estado, igual que validarCuenta().
int incioSesion(int socketDescriptor, char* rol);

//->Toma los datos para dar de alto un usuario
//	Solo pueden acceder usuarios con el rango de administrador.
//pre:	Toma como argumentos el descriptor del socket, una cadena user(para exportar los datos del usuario),
//			y una cadena para exportar el nombre de usuario.
//pos:	Devuelve enteros de estado:
//			USUARIO_VALIDO
//			ERROR_USUARIO_EXISTENTE: se intenta crear un usuario ya existente,
//			ERROR_USUARIO_DATOS_VACIOS: se ingresaron datos vacios.
int tomarDatosNuevoUsuario(int socketDescriptor, char* user, char* usernameValidado);

//->Agrega un nuevo usuario al archivo que los guarda.
//pre:	Toma como argumento el descriptor del socket.
//pos:	Devuelve 0 en el caso que se cree exitosamente el usuario.
int nuevoUsuario(int socketDescriptor);

//->Lista los usuarios que tienen la cantidad de intentos igual a cero.
//  Solo pueden acceder usuarios con el rango de administrador.
//pre:	No toma argumentos.
//pos:	Devuelve una matriz de caracteres con todos los usuarios bloqueados.
int listaDeUsuariosBloqueados();

//->Desbloquea a un usuario que tiene la cantidad de intentos igual a cero.
//  Solo pueden acceder usuarios con el rango de administrador.
//pre:	Toma como argumento el descriptor del socket
//pos:	Devuelve enteros de estado:
//			USUARIO_DESBLOQUEADO
//			ERROR_USUARIO_NO_ENCONTRADO: no encontro al usuario
int desbloquearUsuario(int socketDescriptor);



//------------------------------------------>>>Traduccion<<<------------------------------------------
//->Devuelve una palabra traducida.
//pre:	Toma como argumento dos cadenas de caracteres, una para la palabra a traducir, y otra para exportar la palabra 
//		ya traducida.
//pos:	Devuelve un entero de estado:
//		4: TRADUCCION_ENCONTRADA
//		-2: TRADUCCION_NO_ENCONTRADA
//		-1: ERROR_DE_PROCESAMIENTO
int traducirPalabra(char* palabraATraducir, char* palabraYaTraducida);

//->Abstraccion de la funcion traducirPalabra().
//pre:	Toma como argumento el descriptor del socket.
//pos:	No devuelve nada.
void traducir(int socketDescriptor);

//->Comprueba que el formato de la cadena sea <palabraIngles>:<palabraEspañol>.
//pre:	Toma como argumenta la cadena complea y dos cadenas mas, un para exportar la palabra en ingles y otra para la cadena en espaniol.
//pos:	Devuelve el estado por medio de un boleano:
//		ERROR_FORMATO
//		FORMATO_CORRECTO
int comprobarFormateNuevaTraduccion(char* traduccion, char* palabraIngles, char *palabraEspanio);

//->Agrega una palabra a la lista de palabras disponibles para traducir.
//pre:	Toma como argumento el descriptor del socket.
//pos:	Devuelve cero si pudo agregar exitosamente la palabra al archivo de traducción.
int nuevaTraduccion(int socketDescriptor);



//------------------------------------------>>>Funciones administrador<<<------------------------------------------
//->Envia todas las entradas, hasta el momento en el que se llamo a la función, y las imprime en el cliente.
//pre:	Toma como argumento el descriptor del socket.
//pos:	No devuelve nada.
void mostrarRegistro(int socketDescriptor);



//------------------------------------------>>>Funciones del Menu<<<------------------------------------------

//->Muestra la pagina del menu principal.
//pre:	Toma como argumento el descriptor del socket.
//pos:	No devuelve nada.
void mostrarMenuPrincipal(int socketDescriptor);

//->Muestra el submenu de usuarios.
//pre:	Toma como argumento el descriptor del socket.
//pos:	No devuelve nada.
void mostrarSubMenuUsuarios(int socketDescriptor);

//->Muestra el menu de opciones y devuelve el numero de la opcion elegida.
//pre:	Toma como argumento el descriptor del socket.
//pos:	Devuelve el numero de la opcion elegida por el usuario.
int menuOpcion(int socketDescriptor, char* rol);

//->Muestra las opciones y devuelve el numero de opcion elegida del submenu de usuarios.
//pre:	Toma como argumento el descriptor del socket.
//pos:	Devuelve el numero de la opcion elegida por el usuario.
int submenuOpciones(int socketDescriptor);

//->Libera de logica al switch del menu principal.
//pre:	Toma como argumento el descriptor del socket.
//pos:	No devuelve nada.
void submenu(int socketDescriptor);

//->Maneja la interacción del servidor con el programa cliente.
//pre:	Toma como arguemento el descriptor del socket del servidor.
//pos:	No devuelve nada.
void menuServidor(int socketServidor);

#endif
