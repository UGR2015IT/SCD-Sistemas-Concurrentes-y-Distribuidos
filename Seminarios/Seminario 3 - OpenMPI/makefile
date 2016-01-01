.SUFFIXES:
.PHONY:  todo, 1, 2, 3, 4, 5, 6

compilar := mpicxx -g -Wall 
ejecutar := mpirun 
ejecutables := holamundo sendrecv1 sendrecv2 intercambio_sincrono multireceive intercambio_nobloq

all: $(ejecutables)

1: holamundo
	$(ejecutar) -np 4 ./holamundo
	
2: sendrecv1
	$(ejecutar) -np 2 ./sendrecv1
	
3: sendrecv2
	$(ejecutar) -np 2 ./sendrecv2

4: intercambio_sincrono
	$(ejecutar) -np 8 ./intercambio_sincrono

5: multireceive
	$(ejecutar) -np 11 ./multireceive

6: intercambio_nobloq
	$(ejecutar) -np 8 ./intercambio_nobloq


%: %.cpp
	$(compilar) -o $@ $<

clean:
	rm -f *~ $(EXECFILES)
