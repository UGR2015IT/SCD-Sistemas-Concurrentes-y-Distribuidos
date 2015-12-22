
CC = mpicxx

CFLAGS = -Wall

COMPILE = $(CC) $(CFLAGS)

EXECFILES := holamundo sendrecv1 sendrecv2 intercambio_sincrono multireceive intercambio_nobloq


all: $(EXECFILES)



%: %.cpp

	$(COMPILE) -o $@ $<


clean:
	rm *~ $(EXECFILES)