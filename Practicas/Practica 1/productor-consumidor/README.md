###SPANISH VERSION (ENGLISH BELOW)

---------------------------------------------

## El problema del productor-consumidor

#### Descripción del problema.

**Problema y aplicaciones**

El problema del productor consumidor surge cuando se quiere diseñar un programa en el cual un proceso o hebra produce items de datos en memoria que otro proceso o hebra consume.

* Un ejemplo sería una aplicación de reproducción de vídeo:
 * El productor se encarga de leer de disco o la red y descodificar cada cuadro de vídeo.
 * El consumidor lee los cuadros descodificados y los envía a la memoria de vídeo para que se muestren en pantalla

Hay muchos ejemplos de situaciones parecidas.

* En general, el productor calcula o produce una secuencia de items de datos (uno a uno), y el consumidor lee o consume dichos items (tambien uno a uno).
* El tiempo que se tarda en producir un item de datos puede ser variable y en general distinto al que se tarda en consumirlo (también variable).

**Solución de dos hebras con un vector de items**

Para diseñar un programa que solucione este problema:

* Suele ser conveniente implementar el productor y el consumidor como dos hebras independientes, ya que esto permite tener ocupadas las CPUs disponibles el máximo de tiempo.
* Se puede usar una variable compartida que contiene un ítem de datos.
* Las esperas asociadas a la lectura y la escritura pueden empeorar la eficiencia. Esto puede mejorarse usando un vector que pueda contener muchos items de datos producidos y pendientes de leer.

**Condición de sincronización**
En esta situación, la implementación debe asegurar que:

* Cada ítem producido es leído (ningún ítem se pierde)
* Ningún ítem se lee más de una vez. 

Lo cual implica:

* El productor tendrá que esperar antes de poder escribir en el vector cuando haya creado un ítem pero el vector esté completamente ocupado por ítems pendientes de leer.

* El consumidor debe esperar cuando vaya a leer un ítem del vector pero dicho vector no contenga ningún ítem pendiente de leer.

* En algunas aplicaciones el orden de lectura debe coincidir con el de escritura, en otras podría ser irrelevante.