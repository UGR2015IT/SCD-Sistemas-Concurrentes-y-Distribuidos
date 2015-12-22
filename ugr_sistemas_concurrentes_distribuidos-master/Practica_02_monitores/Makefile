
.SUFFIXES:
.PHONY: msg_uso msg_clases

JAVASRCS:=$(wildcard *.java)
CLASSES_BASE:=$(shell grep class *.java | cut -d' ' -f2)
CLASSES:=$(addsuffix .class,$(CLASSES_BASE))

msg_uso: comp
	@echo "SCD: Ejemplos de monitores en Java."
	@echo "usar con:"
	@echo "   make comp  : compilar todos los archivos java si es necesario"
	@echo "   make recomp: compilar todo incondicionalmente"
	@echo "   make pc    : compilar todo y ejecutar ejemplo del productor-consumidor (20 10 5 2 4)"
	@echo "   make le    : idem para el ejemplo de lectores-escritores (5 20 15 25)"
	@echo "   make cd    : idem para el ejemplo del contador de días"
	@echo "   make clean : elimina todos los archivos .class"
	@echo "   make zip   : crea archivo 'scd-p02-ejemplos.zip' con los fuentes"

comp: msg_clases monitor_classes $(CLASSES)

recomp:
	make clean
	make comp

$(CLASSES): $(JAVASRCS)
	javac -g $(JAVASRCS)

monitor_classes: $(wildcard monitor/*.class)
	javac -g monitor/*.java -d .

clean:
	rm -rf *.class monitor/*.class scd-p02-ejemplos.zip

ej1: p2_ejercicio_01_productor_consumidor.class
	java p2_ejercicio_01_productor_consumidor 2 2 3 5 5
	#java ejercicio_01_productor_consumidor 20 10 5 2 4

ej2: p2_ejercicio_02_fumadores.class
	java p2_ejercicio_02_fumadores

ej3: p2_ejercicio_03_barbero.class
	java p2_ejercicio_03_barbero

ex: berbero_ex.class
	java berbero_ex

pc: EjemploProductorConsumidor.class
	java EjemploProductorConsumidor 20 10 5 2 4

le: EjemploLectorEscritor.class
	java EjemploLectorEscritor 5 20 15 25

cd: EjemploContadorDias.class
	java EjemploContadorDias

msg_clases:
	@echo "fuentes java = " $(JAVASRCS)
	@echo "clases = " $(CLASSES_BASE)

zip:	scd-p02-ejemplos.zip

scd-p02-ejemplos.zip: $(JAVASRCS) $(wildcard monitor/*.java) makefile
	make clean
	zip scd-p02-ejemplos.zip *.java *.txt Makefile monitor/*

