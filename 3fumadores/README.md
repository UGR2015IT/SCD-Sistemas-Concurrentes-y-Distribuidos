### SPANISH VERSION (ENGLISH BELOW)
-------------------------------------
## El problema de los fumadores.

#### Descripción del problema.

En este apartado se intenta resolver un problema algo más complejo usando hebras y semáforos POSIX. 

Considerar un estanco en el que hay tres fumadores y un estanquero.

* Cada fumador representa una hebra que realiza una actividad (fumar), invocando a una función fumar(), en un bucle infinito.

* Cada fumador debe esperar antes de fumar a que se den ciertas condiciones (tener suministros para fumar), que dependen de la actividad del proceso que representa al estanquero.

* El estanquero produce suministros para que los fumadores puedan fumar, también en un bucle infinito.

* Para asegurar concurrencia real, es importante tener en cuenta que la solución diseñada debe permitir que varios fumadores fumen simultáneamente.

A continuación se describen los requisitos para que los fumadores puedan fumar y el funcionamiento del proceso estanquero:

* Antes de fumar es necesario liar un cigarro, para ello el fumador necesita tres ingredientes: tabaco, papel y cerillas.

* Uno de los fumadores tiene solamente papel, otro tiene solamente tabaco, y el otro tiene solamente cerillas.

* El estanquero coloca aleatoriamente dos ingredientes diferentes de los tres que se necesitan para hacer un cigarro, desbloquea al fumador que tiene el tercer ingrediente y después se bloquea.

* El fumador desbloqueado toma los dos ingredientes del mostrador, desbloquea al estanquero para que pueda seguir sirviendo ingredientes y fuma durante un tiempo después de liarse el cigarro.

* El estanquero, cuando se desbloquea, vuelve a poner dos ingredientes aleatorios en el mostrador, y se repite el ciclo.