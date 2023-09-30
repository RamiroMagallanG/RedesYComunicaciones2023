# compilar servidor
all: programaCliente clean

programaCliente: clienteObjeto funcionesCliente
	gcc -o cliente cliente.o functionsClient.o

clienteObjeto: cliente.c functionsClient.h 
	gcc -c cliente.c -lsocket -lisl

funcionesCliente: functionsClient.c functionsClient.h
	gcc -c functionsClient.c -lsocket -lisl

clean:
	rm -f *.o