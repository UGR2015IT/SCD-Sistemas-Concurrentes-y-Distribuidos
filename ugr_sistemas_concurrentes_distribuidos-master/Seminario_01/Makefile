###############################################################################
#
# SCD - Sistemas Concurrentes y Distribuidos
# Grado en Ingeniería Informática
#
# 2014 - Ernesto Serrano <erseco@correo.ugr.es>
# ---------------------------------------------
#
# Fichero Makefile para generacion de ejecutables, paquetes y documentacion
#
###############################################################################

# VARIABLES
BIN 	= bin
SRC 	= src
OBJ 	= obj
LIB 	= lib
INCLUDE = include

# Definición de compiladores
CXX = g++ -Ofast
CC = gcc -Ofast

# banderas para el enlazador
# Se indica directorios donde encontrar las funciontecas con -L. Usando -l seguido del
# nombre se indica la funcionteca que se desea enlazar.
OS := $(shell uname)
ifeq ($(OS),Darwin) #flags para MAC OS X
LDFLAGS =  -lpthread
else  #flags para linux
LDFLAGS =  -lpthread -lrt
endif

OBJECTS = ejemplos s1_ejercicio_01 s1_ejercicio_02_factorial


# TODO (sin empaquetado)
all: clean $(OBJECTS)

# COMPILACION
$(OBJECTS): $(OBJ)/fun_tiempo.o
	$(CXX) -o $(BIN)/$@ $(SRC)/$@.cpp -I$(INCLUDE) $(LDFLAGS) $(OBJ)/fun_tiempo.o

# $(BIN)/ejemplos : $(SRC)/ejemplos.cpp $(OBJ)/fun_tiempo.o
# 	$(CXX) -o $(BIN)/ejemplos $(SRC)/ejemplos.cpp $(OBJ)/fun_tiempo.o -I$(INCLUDE) $(LDFLAGS)

# $(BIN)/ejercicio_01 : $(SRC)/ejercicio_01.cpp $(OBJ)/fun_tiempo.o
# 	$(CXX) -o $(BIN)/ejercicio_01 $(SRC)/ejercicio_01.cpp $(OBJ)/fun_tiempo.o -I$(INCLUDE) $(LDFLAGS)

# $(BIN)/ejercicio_02_factorial : $(SRC)/ejercicio_02_factorial.cpp $(OBJ)/fun_tiempo.o
# 	$(CXX) -o $(BIN)/ejercicio_02_factorial $(SRC)/ejercicio_02_factorial.cpp $(OBJ)/fun_tiempo.o -I$(INCLUDE) $(LDFLAGS)

$(OBJ)/fun_tiempo.o : $(LIB)/fun_tiempo.c
	$(CC) -c -o $(OBJ)/fun_tiempo.o $(LIB)/fun_tiempo.c -I$(INCLUDE) $(LDFLAGS)



# DOCUMENTACION
# Genera la documentacion mediante doxygen en la carpeta doc
.PHONY: doc
doc:
	doxygen doc/doxys/Doxyfile

# EMPAQUETADO
# Genera el archivo .zip de la práctica mediante la linea de comandos
# Omite los archivos propios de MacOSX y coge el nombre del directorio como nombre
.PHONY: package
package: clean
	@echo "Empaquetando..."
	zip $(shell basename "$(CURDIR)")_Ernesto_Serrano.zip -9 -r ./* -x "*.DS_Store" -x "__MACOSX"

# LIMPEZA
# Limpia los archivos compilados, así como los archivos objeto y los paquetes zip
.PHONY: clean
clean:
	@echo "Limpiando..."
	@rm -f $(OBJ)/*\
        $(BIN)/*\
        ./*.zip
