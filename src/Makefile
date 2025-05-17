# Compilação das bibliotecas estáticas
all:libGrafos.a libMensagem.a programa

libGrafos.a: Grafos.o
	ar rcs libGrafos.a Grafos.o
Grafos.o:
	gcc -c Grafos.c -o Grafos.o

libMensagem.a: Mensagem.o
	ar rcs libMensagem.a Mensagem.o

Mensagem.o:
	gcc -c Mensagem.c -o Mensagem.o

# Compilação do programa principal
programa: libMensagem.a libGrafos.a main.o Antenas.o Binario.o Funções.o Nefasto.o Menu.o
	gcc main.o Antenas.o Binario.o Funções.o Nefasto.o Menu.o -L. -lMensagem -lGrafos -o programa

main.o:
	gcc -c main.c -o main.o

Antenas.o:
	gcc -c Antenas.c -o Antenas.o

Binario.o:
	gcc -c Binario.c -o Binario.o

Funções.o:
	gcc -c Funções.c -o Funções.o

Nefasto.o:
	gcc -c Nefasto.c -o Nefasto.o

Menu.o:
	gcc -c Menu.c -o Menu.o

# Limpeza dos arquivos gerados
clean:
	rm -f *.o *.a programa