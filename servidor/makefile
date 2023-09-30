# compilar servidor
all: programaServidor clean

programaServidor: servidorObjeto funcionesServidor
	gcc -o servidor servidor.o functionsServer.o

servidorObjeto: servidor.c functionsServer.h 
	gcc -c servidor.c -lsocket -lisl

funcionesServidor: functionsServer.c functionsServer.h
	gcc -c functionsServer.c -lsocket -lisl

clean:
	rm -f *.o