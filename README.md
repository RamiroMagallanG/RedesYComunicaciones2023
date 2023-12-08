# Redes Y Comunicaciones 2023

# Servicio de Traducción Inglés-Español

## Objetivo

Desarrollar un servicio de traducción de palabras de Inglés a Español mediante aplicaciones en modo consola. El sistema consta de un servidor y un cliente que se comunican a través de sockets. El cliente enviará la información requerida, y el servidor procesará la solicitud devolviendo el resultado al cliente.

## Requerimientos Funcionales

### Servidor

1. *Inicio y Conexión:*
   - Inicia y espera la conexión del cliente.
   - Realiza validaciones de usuario/contraseña consultando un archivo de credenciales.
   - Bloquea usuarios después de 3 intentos fallidos, permitiendo al ADMIN desbloquearlos.
   - Registra todas las actividades del servidor en un archivo llamado server.log.

2. *Traducción:*
   - Utiliza un archivo para almacenar palabras en inglés y sus equivalentes en español.
   - Responde al cliente con la traducción de una palabra en inglés.
   - Permite la inserción de nuevas traducciones.

3. *Validaciones:*
   - Informa si la palabra a traducir no se encuentra.
   - Verifica el formato correcto de inserción.
   - Evita duplicados al insertar nuevas traducciones.
   - Convierte a minúsculas para buscar o insertar palabras.
   - Gestiona el alta de usuarios validando existencia y datos completos.
   - Permite desbloquear usuarios bloqueados.

### Cliente

1. *Conexión:*
   - Especifica dirección IP, puerto y credenciales para conectarse al servidor.

2. *Opciones:*
   - Traduce palabras en inglés a español (rol CONSULTA).
   - Inserta nuevas traducciones (rol ADMIN).
   - Gestiona usuarios (alta y desbloqueo - rol ADMIN).
   - Visualiza registro de actividades (rol ADMIN).
   - Cierra sesión.

3. *Operaciones:*
   - Muestra resultados y mensajes recibidos del servidor.

4. *Menú Principal:*
   - Permite volver al menú principal con el comando "/salir".

## Pantallas

- Utiliza menús para simplificar la selección de opciones.
- Garantiza una presentación clara de los datos.

## Requerimientos No Funcionales

- Lenguaje de programación: C/C++.
- Entorno de desarrollo a elección.
- Sistema operativo: Linux o Windows, a elección.