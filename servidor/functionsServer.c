#include "functionsServer.h"



//----->>>>>Funciones relacionadas con la conexion del socket<<<<<-----

int obtenerPuerto(){
    //int puerto;
    char auxPuerto[10];

    printf("Introduzca el puerto de la conexion (default %hd):", PUERTO_DEFAULT);
    fgets(auxPuerto, sizeof(auxPuerto), stdin);

    if (auxPuerto[0] == '\n'){
        return PUERTO_DEFAULT;
    } else {
        return atoi(auxPuerto);
    }
}

struct sockaddr_in configurarServidor(short tipo_conexion, int puerto){
    struct sockaddr_in servidor;
    
    servidor.sin_family = tipo_conexion;
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = INADDR_ANY;

    return servidor;
}

int crearSocket(short tipo_conexion){
    int socketDescriptor = socket(tipo_conexion, SOCK_STREAM, 0);

    if(socketDescriptor < 0){
        registrarLog(LOG_ERROR_CREAR_SOCKET);
        cerrarConexion(socketDescriptor);
        exit(1);
    } else {
        registrarLog(LOG_EXITO_CREAR_SOCKET);
        return socketDescriptor;
    }
}

void vincular_socket(int socketDescriptor, struct sockaddr_in servidor){
    if (bind(socketDescriptor, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        registrarLog(LOG_ERROR_VINCULAR_SOCKET);
        cerrarConexion(socketDescriptor);
        exit(1);
    } else {
        char log[TAMANIO_MENSAJE];
        snprintf(log, sizeof(log), LOG_EXITO_VINCULAR_SOCKET, servidor.sin_port);
        registrarLog(log);
    }
}

void escuchar_socket(int socketDescriptor){
    if(listen(socketDescriptor, 1) < 0){
        registrarLog(LOG_ERROR_ESPERAR_CONEXION);
        cerrarConexion(socketDescriptor);
        exit(1);
    } else {
        registrarLog(LOG_EXITO_ESPERAR_CONEXION);
    }
}

int iniciarServidor(short tipo_conexion){
    int puerto = obtenerPuerto();

    struct sockaddr_in servidor = configurarServidor(tipo_conexion, puerto);
    
    int socketDescriptor = crearSocket(tipo_conexion);

    vincular_socket(socketDescriptor, servidor);

    escuchar_socket(socketDescriptor);

    return socketDescriptor;
}

int aceptarConexion(int socketDescriptor){
    struct sockaddr_in cliente;
    int longitud_cliente = sizeof(struct sockaddr_in);
    
    int socketCliente = accept(socketDescriptor, (struct sockaddr*) &cliente, &longitud_cliente);

    if(socketCliente < 0){
        registrarLog(LOG_ERROR_ACEPTAR_CONEXION);
        cerrarConexion(socketCliente);
        exit(1);
    } else {
        char log[TAMANIO_MENSAJE];
        snprintf(log, sizeof(log), LOG_EXITO_ACEPTAR_CONEXION, cliente.sin_addr.s_addr);
        registrarLog(log);
    }

    return socketCliente;
}

void cerrarConexion(int socketDescriptor){
    if(close(socketDescriptor) < 0){
        registrarLog(LOG_ERROR_CERRAR_SOCKET);
        exit(1);
    } else {
        registrarLog(LOG_EXITO_CERRAR_SOCKET);
    }
}



//----->>>>>Funciones de Comunicación<<<<<-----

void desbloquearProgramaCliente(int socketDescriptor){
    funcionBaseMandarMensaje(socketDescriptor, CODIGO_ENTRADA_CLIENTE, sizeof(CODIGO_ENTRADA_CLIENTE));
}

void mandarAlertaEntrada(int socketDescriptor){
    funcionBaseMandarMensaje(socketDescriptor, CODIGO_ENTRADA_CLIENTE, sizeof(CODIGO_ENTRADA_CLIENTE));
}

void funcionBaseRecibirMensaje(int socketDescriptor, char *mensaje, size_t tamanioMensaje){
    char mensajeTemporal[TAMANIO_MENSAJE] = "";

    size_t bytes = recv(socketDescriptor, mensajeTemporal, tamanioMensaje, 0);

    if(bytes < 0){
        perror("--->>>Error al recibir mensaje");
        cerrarConexion(socketDescriptor);
        exit(1);
    } else if (bytes > 0){
        strcpy(mensaje, mensajeTemporal);
    }
}

void recibirMensaje(int socketDescriptor, char *mensaje, size_t tamanioMensaje){
    mandarAlertaEntrada(socketDescriptor);
    funcionBaseRecibirMensaje(socketDescriptor, mensaje, tamanioMensaje);
    registrarLog(LOG_EXITO_RECIBIR_MENSAJE);
}

void funcionBaseMandarMensaje(int socketDescriptor, char *mensaje, size_t tamanioMensaje){
    size_t bytes = send(socketDescriptor, mensaje, tamanioMensaje, 0);

    if(bytes < 0){
        perror("--->>>Error al enviar mensaje");
        cerrarConexion(socketDescriptor);
        exit(1);
    } else if(bytes == 0){
        cerrarConexion(socketDescriptor);
        exit(1);
    }
}

void mandarMensaje(int socketDescriptor, char* mensaje, size_t tamanioMensaje){
    bool bloqueo = true;
    char controlBloque[TAMANIO_MENSAJE];

    funcionBaseMandarMensaje(socketDescriptor, mensaje, tamanioMensaje);

    while(bloqueo){
        funcionBaseRecibirMensaje(socketDescriptor, controlBloque, sizeof(controlBloque));
        bloqueo = (strcmp(controlBloque, CODIGO_CLIENTE_RECEPCION) == 0) ? false : true;
    }
    registrarLog(LOG_EXITO_MANDAR_MENSAJE);
}



//----->>>>>Funciones de la aplicacion<<<<<-----
//------------------------------------------>>>utiles<<<------------------------------------------
FILE* abrirArchivo(char* nombreArchivo, char* metodo){
    FILE* archivo = fopen(nombreArchivo, metodo);

    if (archivo == NULL){
        perror("--->>>Error al abrir.");
        fclose(archivo);
        exit(1);
    }

    return archivo;
}

void pasarAMinusculas(char* palabra, size_t tamanioPalabra){
    char aux[tamanioPalabra];
    strcpy(aux, palabra);

    for(int indicePalabra = 0; indicePalabra<tamanioPalabra; indicePalabra++){
        palabra[indicePalabra] = tolower(aux[indicePalabra]);
    }
}

bool comprobarCadenaVacia(char* cadena){
    if (strcmp(MENSAJE_VACIO, cadena) == 0){
        return ERROR_CADENA_VACIA;
    }

    return CADENA_NO_VACIA;
}

int existeUsuario(char* username){

    FILE* archivo = abrirArchivo(ARCHIVO_CREDENCIALES, METODO_READING);

    char linea[TAMANIO_MENSAJE];
    char usernameArchivado[TAMANIO_MENSAJE];

    while(fgets(linea, sizeof(linea), archivo) != NULL){
        if (sscanf(linea, "%[^|]", usernameArchivado) < 0){
            perror("--->>>Error al leer el contenido del archivo.");
            fclose(archivo);
            return ERROR_DE_PROCESAMIENTO;
        }

        if(strcmp(usernameArchivado, username) == 0){
            fclose(archivo);
            return ERROR_USUARIO_EXISTENTE;
        }
    }

    fclose(archivo);
    return CUENTA_INEXISTENTE;
}

bool comprobarRol(char* rol, char* rolDeseado, int socketDescriptor){
    if(strcmp(rol, rolDeseado) == 0){
        return ROL_SUFICIENTE;
    }
    mandarMensaje(socketDescriptor, MENSAJE_ERROR_FALTA_PERMISOS, sizeof(MENSAJE_ERROR_FALTA_PERMISOS));
    return ERROR_ROL_INSUFICIENTE;
}

void registrarLog(char* mensaje){
    time_t tiempo = time(NULL);
    struct tm tm = *localtime(&tiempo);

    char entradaLog[TAMANIO_MENSAJE];

    if(snprintf(entradaLog, sizeof(entradaLog), LOG_FECHA_HORA_BASE, tm.tm_year+1900, tm.tm_mon+1,
        tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec) < 0){
        exit(1);
    }

    strcat(entradaLog, mensaje);

    FILE* archivo = abrirArchivo(ARCHIVO_LOG, METODO_ESCRITURA);
    fputs(entradaLog, archivo);
    fclose(archivo);
}



//------------------------------------------>>>Traduccion<<<------------------------------------------
int traducirPalabra(char* palabraATraducir, char* palabraYaTraducida){
    FILE* archivo = abrirArchivo(ARCHIVO_TRADUCCIONES, METODO_READING);

    char linea[256];
    char palabraIngles[128];
    char palabraEspaniol[128];

    pasarAMinusculas(palabraATraducir, strlen(palabraATraducir));

    while (fgets(linea, sizeof(linea), archivo) != NULL){
        if (sscanf(linea, "%[^:]:%s", palabraIngles, palabraEspaniol) < 0){
            perror("--->>>Error al leer el contenido del archivo.");
            fclose(archivo);
            return ERROR_DE_PROCESAMIENTO;
        }

        if (strcmp(palabraIngles, palabraATraducir) == 0){
            strcpy(palabraYaTraducida, palabraEspaniol);
            fclose(archivo);
            return TRADUCCION_ENCONTRADA;
        }
    }
    fclose(archivo);
    return TRADUCCION_NO_ENCONTRADA;
}

void traducir(int socketDescriptor){
    char palabraATraducir[TAMANIO_MENSAJE];

    mandarMensaje(socketDescriptor, MENSAJE_PEDIDO_PALABRA_A_TRADUCIR, sizeof(MENSAJE_PEDIDO_PALABRA_A_TRADUCIR));
    recibirMensaje(socketDescriptor, palabraATraducir, sizeof(palabraATraducir));

    if (strcmp(palabraATraducir, SALIR_MENU_ACTUAL) != 0){
        char palabraTraducida[TAMANIO_MENSAJE];
        char respuesta[TAMANIO_MENSAJE];

        //pasarCadenaAMinuscula(palabraATraducir);
        
        int resultado = traducirPalabra(palabraATraducir, palabraTraducida);

        if(resultado == TRADUCCION_ENCONTRADA){
            int formateoCadenaResultado = 
                snprintf(respuesta, sizeof(respuesta), MENSAJE_CON_TRADUCCION, palabraATraducir, palabraTraducida);

            if(formateoCadenaResultado > 0){
                mandarMensaje(socketDescriptor, respuesta, strlen(respuesta));
            }

        } else if (resultado == TRADUCCION_NO_ENCONTRADA){
            int formateoCadenaResultado = 
                snprintf(respuesta, sizeof(respuesta), MENSAJE_NO_SE_ENCONTRO_TRADUCCION, palabraATraducir);

            if(formateoCadenaResultado > 0){
                mandarMensaje(socketDescriptor, respuesta, strlen(respuesta));
            }
        }
    }
}

int comprobarFormateNuevaTraduccion(char* traduccion, char* palabraIngles, char *palabraEspanio){
    // TODO cambiar el retorno a bool.
    char palabraInglesAComprobar[TAMANIO_MENSAJE];
    char palabraEspaniolAComprobar[TAMANIO_MENSAJE];

    if(sscanf(traduccion, "%[^:]:%s", palabraInglesAComprobar, palabraEspaniolAComprobar) != 2){
        return ERROR_FORMATO;
    }

    pasarAMinusculas(palabraInglesAComprobar, sizeof(palabraInglesAComprobar));
    pasarAMinusculas(palabraEspaniolAComprobar, sizeof(palabraEspaniolAComprobar));

    strcpy(palabraIngles, palabraInglesAComprobar);
    strcpy(palabraEspanio, palabraEspaniolAComprobar);

    return FORMATO_CORRECTO;
}

int nuevaTraduccion(int socketDescriptor){
    char linea[TAMANIO_MENSAJE];

    char palabraIngles[TAMANIO_MENSAJE];
    char palabraEspaniol[TAMANIO_MENSAJE];

    mandarMensaje(socketDescriptor, MENSAJE_NUEVA_TRADUCCION, sizeof(MENSAJE_NUEVA_TRADUCCION));
    recibirMensaje(socketDescriptor, linea, sizeof(linea));

    if(comprobarFormateNuevaTraduccion(linea, palabraIngles, palabraEspaniol) != FORMATO_CORRECTO){
        mandarMensaje(socketDescriptor, MENSAJE_ERROR_FORMATO_N_TRADUCCION, sizeof(MENSAJE_ERROR_FORMATO_N_TRADUCCION));
        return ERROR_FORMATO;
    } else {
        FILE* archivo = abrirArchivo(ARCHIVO_TRADUCCIONES, METODO_READING_PLUS);

        char palabra[TAMANIO_MENSAJE];

        while (fgets(linea, sizeof(linea), archivo) != NULL){
            if(sscanf(linea, "%[^:]:", palabra) > 0){
                if(strcmp(palabra, palabraIngles) == 0){

                    traducirPalabra(palabraIngles, palabraEspaniol);

                    if(snprintf(linea, sizeof(linea), MENSAJE_ERROR_TRADUCCION_REPETIDA, palabraIngles, palabraEspaniol) > 0){
                        mandarMensaje(socketDescriptor, linea, sizeof(linea));
                        fclose(archivo);
                        return ERROR_TRADUCCION_REPETIDA;                        
                    } else {
                        perror("--->>>Error al formatear traduccion.");
                        fclose(archivo);
                        return ERROR_DE_PROCESAMIENTO;
                    }

                }
            } else {
                perror("--->>>Error al leer el contenido del archivo.");
                fclose(archivo);
                return ERROR_DE_PROCESAMIENTO;
            }
        }

        if(snprintf(linea, sizeof(linea), "%s:%s\n", palabraIngles, palabraEspaniol) < 0){
            perror("--->>>Error al formatear traduccion.");
            fclose(archivo);
            return ERROR_DE_PROCESAMIENTO;
        }

        if(fputs(linea, archivo) != EOF){
            mandarMensaje(socketDescriptor, MENSAJE_TRADUCCION_GUARDADA, sizeof(MENSAJE_TRADUCCION_GUARDADA));
            fclose(archivo);
            return TRADUCCION_AGREGADA;
        } else {
            perror("--->>>Error al guardar el contenido del archivo.");
            fclose(archivo);
            return ERROR_DE_PROCESAMIENTO;
        }
    }
}



//------------------------------------------>>>Administracion de usuarios<<<------------------------------------------
int validarCuenta(int socketDescriptor, char parInicioSesion[][512], char* rol){
    char username[TAMANIO_MENSAJE]; //username recibido; 
    char usernameArchivo[TAMANIO_MENSAJE]; //username en el archivo
    
    char password[TAMANIO_MENSAJE]; //contraseña recibida;
    char passwordArchivo[TAMANIO_MENSAJE]; //contraseña en el archivo

    char rolUsuario[TAMANIO_MENSAJE];
    int numeroIntentos; //cantidad de veces que se intento entrar en esa cuenta con una contraseña incorrecta.
    
    strcpy(username, parInicioSesion[0]);
    strcpy(password, parInicioSesion[1]);
    
    int contadorLineas = 0;
    char linea[TAMANIO_MENSAJE];

    FILE *archivo = abrirArchivo(ARCHIVO_CREDENCIALES, METODO_READING_PLUS);

    while (fgets(linea, sizeof(linea), archivo) != NULL){
        contadorLineas++;

        if (sscanf(linea, "%[^|]|%[^|]|%[^|]|%d", usernameArchivo, passwordArchivo, rolUsuario, &numeroIntentos) < 0){
            fclose(archivo);
            return ERROR_DE_PROCESAMIENTO;
        }

        if (strcmp(usernameArchivo, username) == 0){
            if ((strcmp(rolUsuario, ROL_ADMINISTRADOR) != 0) && (numeroIntentos == 0)){
                //Si el usuario no tiene el rol de 'ADMIN' y el numero de intentos es igual a 0
                
                snprintf(linea, sizeof(linea), MENSAJE_USUARIO_BLOQUEADO, username);
                mandarMensaje(socketDescriptor, linea, strlen(linea));
                registrarLog(LOG_ERROR_USUARIO_INICIO_SESION);
                fclose(archivo);
                return CUENTA_BLOQUEADA;
            } else {
                if (strcmp(passwordArchivo, password) == 0){ 
                    //Si la contraseña en el archivo y la pasada por argumentos son iguales
                    char log[TAMANIO_MENSAJE];
                    snprintf(log, sizeof(log), LOG_EXITO_INICIO_SESION, username);

                    registrarLog(log);

                    strcpy(rol, rolUsuario);
                    fclose(archivo);
                    return INICIO_DE_SESSION_CORRECTO;

                } else {
                    mandarMensaje(socketDescriptor, MENSAJE_CONTRASENIA_INCORRECTA, sizeof(MENSAJE_CONTRASENIA_INCORRECTA));
                    if(strcmp(rolUsuario, ROL_ADMINISTRADOR) != 0){
                        //Si el usuario no tiene el rol de 'ADMIN'
                        numeroIntentos--; 

                        if(numeroIntentos == 0){
                            snprintf(linea, sizeof(linea), USUARIO_BLOQUEADO);
                            mandarMensaje(socketDescriptor, linea, strlen(linea));
                        }

                        for(int indiceReemplazoLinea = 0; indiceReemplazoLinea<contadorLineas; indiceReemplazoLinea++){
                            if(indiceReemplazoLinea == contadorLineas-1){
                                if(snprintf(linea, sizeof(linea), "%s|%s|%s|%d\n", usernameArchivo, passwordArchivo, rolUsuario, numeroIntentos) < 0){
                                    perror("--->>>Error al formatear usuario.");
                                    fclose(archivo);
                                    return ERROR_DE_PROCESAMIENTO;
                                }
                                fseek(archivo, -strlen(linea), SEEK_CUR);
                                fputs(linea, archivo);
                            }
                        }
                    }

                    registrarLog(LOG_ERROR_CONTRASENIA_INICIO_SESION);
                    fclose(archivo);
                    return CONTRASEÑA_INCORRECTA;
                }
            }
        }
    }

    fclose(archivo);
    registrarLog(LOG_ERROR_BLOQUEADO_INICIO_SESION);
    return CUENTA_INEXISTENTE;
}

int incioSesion(int socketDescriptor, char* rol){
    char mensajeEntrada[2][TAMANIO_MENSAJE];
    int estado;

    do{
        //username
        mandarMensaje(socketDescriptor, MENSAJE_INGRESAR_USERNAME, strlen(MENSAJE_INGRESAR_USERNAME));
        recibirMensaje(socketDescriptor, mensajeEntrada[0], sizeof(mensajeEntrada[0]));

        //password
        mandarMensaje(socketDescriptor, MENSAJE_INGRESAR_CONTRASENIA, strlen(MENSAJE_INGRESAR_CONTRASENIA));
        recibirMensaje(socketDescriptor, mensajeEntrada[1], sizeof(mensajeEntrada[1]));

        estado = validarCuenta(socketDescriptor, mensajeEntrada, rol);

    }while (estado != INICIO_DE_SESSION_CORRECTO);
}

int tomarDatosNuevoUsuario(int socketDescriptor, char* user, char* usernameValidado){
    char username[120] = "";

    mandarMensaje(socketDescriptor, MENSAJE_NUEVO_USERNAME, sizeof(MENSAJE_NUEVO_USERNAME));
    recibirMensaje(socketDescriptor, username, sizeof(username));

    if(comprobarCadenaVacia(username) == ERROR_CADENA_VACIA){
        mandarMensaje(socketDescriptor, MENSAJE_ERROR_DATOS_VACIOS, sizeof(MENSAJE_ERROR_DATOS_VACIOS));
        return ERROR_USUARIO_DATOS_VACIOS;
    } else if(existeUsuario(username) != CUENTA_INEXISTENTE){
        mandarMensaje(socketDescriptor, MENSAJE_USUARIO_EXISTENTE, sizeof(MENSAJE_USUARIO_EXISTENTE));
        return ERROR_USUARIO_EXISTENTE;
    }

    pasarAMinusculas(username, sizeof(username));
    
    char contrasenia[120];
    mandarMensaje(socketDescriptor, MENSAJE_NUEVA_CONTRASENIA, sizeof(MENSAJE_NUEVA_CONTRASENIA));
    recibirMensaje(socketDescriptor, contrasenia, sizeof(contrasenia));

    if(comprobarCadenaVacia(contrasenia) == ERROR_CADENA_VACIA){
        mandarMensaje(socketDescriptor, MENSAJE_ERROR_DATOS_VACIOS, sizeof(MENSAJE_ERROR_DATOS_VACIOS));
        return ERROR_USUARIO_DATOS_VACIOS;
    }

    char rol[15];
    char desicion[5];

    mandarMensaje(socketDescriptor, MENSAJE_ROL_NUEVO_USUARIO, sizeof(MENSAJE_ROL_NUEVO_USUARIO));
    recibirMensaje(socketDescriptor, desicion, sizeof(desicion));

    if(strcmp(desicion, "y") == 0){
        strcpy(rol, ROL_ADMINISTRADOR);
    } else {
        strcpy(rol, ROL_CONSULTA);
    }

    char linea[TAMANIO_MENSAJE];

    if(snprintf(linea, sizeof(linea), "%s|%s|%s|%d\n", username, contrasenia, rol, 3) < 0){
        perror("--->>>Error al formatear usuario.");
        return ERROR_DE_PROCESAMIENTO;
    }

    strcpy(user, linea);
    strcpy(usernameValidado, username);

    return USUARIO_VALIDO;
}

int nuevoUsuario(int socketDescriptor){
    char usuario[TAMANIO_MENSAJE];
    char username[TAMANIO_MENSAJE];

    if(tomarDatosNuevoUsuario(socketDescriptor, usuario, username) == USUARIO_VALIDO){
        FILE* archivo = abrirArchivo(ARCHIVO_CREDENCIALES, METODO_ESCRITURA);
        if(fputs(usuario, archivo) != EOF){
            char linea[TAMANIO_MENSAJE];

            if(snprintf(linea, sizeof(linea), MENSAJE_USUARIO_CREADO, username) < 0){
                perror("--->>>Error al formatear usuario.");
                fclose(archivo);
                return ERROR_DE_PROCESAMIENTO;
            }

            mandarMensaje(socketDescriptor, linea, sizeof(linea));
            fclose(archivo);
            return USUARIO_AGREGADO;
        }
    } else {
        return ERROR_CREACION_USUARIO;
    }
}

int listaDeUsuariosBloqueados(int socketDescriptor){
    FILE* archivo = abrirArchivo(ARCHIVO_CREDENCIALES, METODO_READING);
    char linea[TAMANIO_MENSAJE];

    int contador = 0;

    char username[TAMANIO_MENSAJE];
    int intentos;
    char rol[TAMANIO_MENSAJE];

    while(fgets(linea, sizeof(linea), archivo) != NULL){
        if(sscanf(linea, "%[^|]|%*[^|]|%[^|]|%d", username, rol, &intentos) < 0){
            perror("--->>>Error al leer el contenido del archivo.");
            fclose(archivo);
            return ERROR_DE_PROCESAMIENTO;
        }

        if((strcmp(rol, ROL_ADMINISTRADOR) != 0) && (intentos <= 0)) {
            char linea[TAMANIO_MENSAJE];
            if(snprintf(linea, sizeof(linea), MENSAJE_LISTA_USUARIOS_BLOQUEADOS, username) < 0){
                perror("--->>>Error al procesar mensaje.");
                fclose(archivo);
                return ERROR_DE_PROCESAMIENTO;
            }
            mandarMensaje(socketDescriptor, linea, sizeof(linea));
            contador++;
        }
    }

    if(contador == 0){
        mandarMensaje(socketDescriptor, MENSAJE_ERROR_NO_SE_ENCONTRARON_BLOQUEADOS, sizeof(MENSAJE_ERROR_NO_SE_ENCONTRARON_BLOQUEADOS));
    }

    fclose(archivo);
    return contador;
}

int desbloquearUsuario(int socketDescriptor){
    char username[TAMANIO_MENSAJE];

    char usernameArchivo[TAMANIO_MENSAJE];
    char rol[TAMANIO_MENSAJE];
    char contrasenia[TAMANIO_MENSAJE];
    int intentos;

    char linea[TAMANIO_MENSAJE];

    mandarMensaje(socketDescriptor, MENSAJE_ELEGIR_DESBLOQUEAR_USUARIO, sizeof(MENSAJE_ELEGIR_DESBLOQUEAR_USUARIO));
    recibirMensaje(socketDescriptor, username, sizeof(username));

    if(strcmp(username, SALIR_MENU_ACTUAL) == 0){
        return USUARIO_QUIERE_SALIR;
    }

    FILE* archivo = abrirArchivo(ARCHIVO_CREDENCIALES, METODO_READING_PLUS);

    while(fgets(linea, sizeof(linea), archivo) != NULL){
        if(sscanf(linea, "%[^|]|%[^|]|%[^|]|%d", usernameArchivo, rol, contrasenia, &intentos) < 0){
            perror("--->>>Error al leer el contenido del archivo.");
            fclose(archivo);
            return ERROR_DE_PROCESAMIENTO;
        }
        if((strcmp(rol, ROL_ADMINISTRADOR) != 0) && 
            (strcmp(username, usernameArchivo) == 0) &&
            intentos == 0){

            if(snprintf(linea, sizeof(linea), "%s|%s|%s|%d\n", usernameArchivo, rol, contrasenia, 3) < 0){
                perror("--->>>Error al leer el contenido del archivo.");
                fclose(archivo);
                return ERROR_DE_PROCESAMIENTO;
            }

            fseek(archivo, -strlen(linea)-1, SEEK_CUR);
            fprintf(archivo, "\n%s", linea);

            if(snprintf(linea, sizeof(linea), MENSJAE_USUARIO_DESBLOQUEADO, usernameArchivo) < 0){
                perror("--->>>Error al leer el contenido del archivo.");
                fclose(archivo);
                return ERROR_DE_PROCESAMIENTO;
            }

            mandarMensaje(socketDescriptor, linea, sizeof(linea));
            fclose(archivo);
            return USUARIO_DESBLOQUEADO;
        }
    }
    mandarMensaje(socketDescriptor, USUARIO_NO_ENCONTRADO, sizeof(USUARIO_NO_ENCONTRADO));
    fclose(archivo);
    return ERROR_USUARIO_NO_ENCONTRADO;
}


//------------------------------------------>>>Funciones administrador<<<------------------------------------------
void mostrarRegistro(int socketDescriptor){
    FILE* archivo = abrirArchivo(ARCHIVO_LOG, METODO_READING);
    char mensaje[TAMANIO_MENSAJE];

    int contador = 0;

    while(fgets(mensaje, sizeof(mensaje), archivo)){
        contador++;
    }

    fclose(archivo);
    archivo = abrirArchivo(ARCHIVO_LOG, METODO_READING);

    while(fgets(mensaje, sizeof(mensaje), archivo) && contador){
        mandarMensaje(socketDescriptor, mensaje, strlen(mensaje));
        contador--;
    }

    fclose(archivo);
}



//------------------------------------------>>>Funciones del Menu<<<------------------------------------------

void mostrarMenuPrincipal(int socketDescriptor){
    mandarMensaje(socketDescriptor, MENU_BASE_TRADUCIR, sizeof(MENU_BASE_TRADUCIR));
    mandarMensaje(socketDescriptor, MENU_BASE_NUEVA_TRADUCCION, sizeof(MENU_BASE_NUEVA_TRADUCCION));
    mandarMensaje(socketDescriptor, MENU_BASE_USERS, sizeof(MENU_BASE_USERS));
    mandarMensaje(socketDescriptor, MENU_BASE_ACTIVIDADES, sizeof(MENU_BASE_ACTIVIDADES));
    mandarMensaje(socketDescriptor, MENU_BASE_LOGOUT, sizeof(MENU_BASE_LOGOUT));
    mandarMensaje(socketDescriptor, MENU_BASE_APAGAR, sizeof(MENU_BASE_APAGAR));
}

void mostrarSubMenuUsuarios(int socketDescriptor){
    mandarMensaje(socketDescriptor, MENU_SUBMENU_USUARIOS_ALTA, sizeof(MENU_SUBMENU_USUARIOS_ALTA));
    mandarMensaje(socketDescriptor, MENU_SUBMENU_USUARIOS_DESBLOQUEO, sizeof(MENU_SUBMENU_USUARIOS_DESBLOQUEO));
    mandarMensaje(socketDescriptor, MENU_SUBMENU_USUARIOS_SALIR, sizeof(MENU_SUBMENU_USUARIOS_SALIR));
}

int menuOpcion(int socketDescriptor, char* rol){
    char mensaje[TAMANIO_MENSAJE];
    int resultado = -1;

    mostrarMenuPrincipal(socketDescriptor);
    mandarMensaje(socketDescriptor, MENSAJE_INGRESAR_COMANDO, sizeof(MENSAJE_INGRESAR_COMANDO));

    recibirMensaje(socketDescriptor, mensaje, sizeof(mensaje));

    if (strcmp(mensaje, MENU_OPCION_TRADUCIR) == 0){
        if(!comprobarRol(rol, ROL_CONSULTA, socketDescriptor)){   
            return ERROR_FALTA_PERMISO;
        }
        resultado = OPCION_TRADUCIR;
    } else if (strcmp(mensaje, MENU_NUEVA_TRADUCCION) == 0){
        if(!comprobarRol(rol, ROL_ADMINISTRADOR, socketDescriptor)){   
            return ERROR_FALTA_PERMISO;
        }
        resultado = OPCION_NUEVA_TRADUCCION;
    } else if (strcmp(mensaje, MENU_USUARIOS) == 0){

        if(!comprobarRol(rol, ROL_ADMINISTRADOR, socketDescriptor)){   
            return ERROR_FALTA_PERMISO;
        }

        mostrarSubMenuUsuarios(socketDescriptor);
        mandarMensaje(socketDescriptor, MENSAJE_INGRESAR_COMANDO, sizeof(MENSAJE_INGRESAR_COMANDO));
        recibirMensaje(socketDescriptor, mensaje, sizeof(mensaje));

        if(strcmp(mensaje, SALIR_MENU_ACTUAL) == 0) return USUARIO_QUIERE_SALIR;

        if (strcmp(mensaje, SUBMENU_USUARIOS_ALTA) == 0){
            resultado = OPCION_ALTA;
        } else if (strcmp(mensaje, SUBMENU_USUARIOS_DESBLOQUEO) == 0){
            resultado = OPCION_DESBLOQUEO;
        }

    } else if (strcmp(mensaje, MENU_REGISTRO_ACTIVIDADES) == 0){
        if(!comprobarRol(rol, ROL_ADMINISTRADOR, socketDescriptor)){   
            return ERROR_FALTA_PERMISO;
        }
        resultado = OPCION_ACTIVIDADES;
    } else if (strcmp(mensaje, MENU_CERRAR_SESION) == 0){
        resultado = OPCION_CERRAR_SESION;
    } else if (strcmp(mensaje, MENSAJE_APAGAR_SERVIDOR) == 0){
        if(!comprobarRol(rol, ROL_ADMINISTRADOR, socketDescriptor)){   
            return ERROR_FALTA_PERMISO;
        }

        resultado = OPCION_APAGAR_SERVIDOR;
    }
    return resultado;
}

void menuServidor(int socketServidor){
    int socketDescriptor = aceptarConexion(socketServidor);
    char rol[15];
    incioSesion(socketDescriptor, rol);
    mandarMensaje(socketDescriptor, MENSAJE_BIENVENIDA, sizeof(MENSAJE_BIENVENIDA));
    int opcion;

    while (opcion != OPCION_APAGAR_SERVIDOR) {

        opcion = menuOpcion(socketDescriptor, rol);

        switch (opcion){
            case OPCION_TRADUCIR:
                traducir(socketDescriptor);
                mandarMensaje(socketDescriptor, MENSAJE_ESPERAR_ENTER, sizeof(MENSAJE_ESPERAR_ENTER));
                mandarMensaje(socketDescriptor, MENSAJE_PAUSAR_PROGRAMA, sizeof(MENSAJE_PAUSAR_PROGRAMA));
                break;
            case OPCION_NUEVA_TRADUCCION:
                nuevaTraduccion(socketDescriptor);
                mandarMensaje(socketDescriptor, MENSAJE_ESPERAR_ENTER, sizeof(MENSAJE_ESPERAR_ENTER));
                mandarMensaje(socketDescriptor, MENSAJE_PAUSAR_PROGRAMA, sizeof(MENSAJE_PAUSAR_PROGRAMA));
                break;
            case OPCION_ALTA:
                int resultado = nuevoUsuario(socketDescriptor);
                mandarMensaje(socketDescriptor, MENSAJE_ESPERAR_ENTER, sizeof(MENSAJE_ESPERAR_ENTER));
                mandarMensaje(socketDescriptor, MENSAJE_PAUSAR_PROGRAMA, sizeof(MENSAJE_PAUSAR_PROGRAMA));
                break;
            case OPCION_DESBLOQUEO:
                if(listaDeUsuariosBloqueados(socketDescriptor) == 0) break;
                desbloquearUsuario(socketDescriptor);
                mandarMensaje(socketDescriptor, MENSAJE_ESPERAR_ENTER, sizeof(MENSAJE_ESPERAR_ENTER));
                mandarMensaje(socketDescriptor, MENSAJE_PAUSAR_PROGRAMA, sizeof(MENSAJE_PAUSAR_PROGRAMA));
                break;
            case OPCION_ACTIVIDADES:
                mostrarRegistro(socketDescriptor);
                mandarMensaje(socketDescriptor, MENSAJE_ESPERAR_ENTER, sizeof(MENSAJE_ESPERAR_ENTER));
                mandarMensaje(socketDescriptor, MENSAJE_PAUSAR_PROGRAMA, sizeof(MENSAJE_PAUSAR_PROGRAMA));
                break;
            case USUARIO_QUIERE_SALIR:
                break;
            case OPCION_CERRAR_SESION:
                socketDescriptor = aceptarConexion(socketServidor);
                incioSesion(socketDescriptor, rol);
                break;
            case ERROR_FALTA_PERMISO:
                mandarMensaje(socketDescriptor, MENSAJE_ESPERAR_ENTER, sizeof(MENSAJE_ESPERAR_ENTER));
                mandarMensaje(socketDescriptor, MENSAJE_PAUSAR_PROGRAMA, sizeof(MENSAJE_PAUSAR_PROGRAMA));
                break;
            case OPCION_APAGAR_SERVIDOR:
                break;
            default:
                mandarMensaje(socketDescriptor, MENSAJE_DEFAULT, sizeof(MENSAJE_DEFAULT));
                mandarMensaje(socketDescriptor, MENSAJE_ESPERAR_ENTER, sizeof(MENSAJE_ESPERAR_ENTER));
                mandarMensaje(socketDescriptor, MENSAJE_PAUSAR_PROGRAMA, sizeof(MENSAJE_PAUSAR_PROGRAMA));
                break; 
        }
        
        mandarMensaje(socketDescriptor, MENSAJE_LIMPIAR_PANTALLA, sizeof(MENSAJE_LIMPIAR_PANTALLA));
    }
    cerrarConexion(socketServidor);
}
