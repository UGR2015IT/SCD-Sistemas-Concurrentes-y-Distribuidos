//==============================================================//
//		   Exercise 3: Barberia    			//
//     Relacion de tipo cliente-servidor entre procesos		//
//==============================================================//
// Las condiciones usadas en esto codigo son:			//
// - silla: condicion para comprobar si esta un cliente servido	//
//		para el barbero					//
// - salaEspera: condicion por todos los clientes que esperan	//
//		de ser servidos					//
// - barbero: condicion por comprobar si el barbero trabaje	//		
//==============================================================//

import monitor.*;
import java.util.Random;

class Barberia extends AbstractMonitor {
	// Class which incapsulate the monitor	
	private int esperando = 0;

	private Condition silla = makeCondition();
	private Condition salaEspera = makeCondition();	
	private Condition barbero = makeCondition();
	
	public void cortarPelo(){	//METODO DE LOS CLIENTES
		enter();
			
			if (!salaEspera.isEmpty()) {				
				esperando++;
				System.out.println("Soy el cliente n."+esperando);
				salaEspera.await();
			} else if (salaEspera.isEmpty() && !silla.isEmpty()) {
				esperando++;
				System.out.println("Soy el cliente n."+esperando);
				salaEspera.await();
			}
			barbero.signal();
			silla.await();
		leave();
	}

	public void siguenteCliente(){
		enter();
			if (salaEspera.isEmpty() && silla.isEmpty()) barbero.await();
			else if (silla.isEmpty()) {
				salaEspera.signal();
				esperando--;
				System.out.println("Siguiente! Ahora esperan en "+esperando+".");
			}
		leave();
	}

	public void finCliente(){
		enter();
			System.out.println("Cliente hecho!");
			silla.signal();
		leave();
	}
}

class aux{
	static Random genAlea = new Random() ;
	static void dormir_max( int milisecsMax ) {
		try {
			Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
		} catch( InterruptedException e ) {
			System.err.println("sleep interumpido en 'aux.dormir_max()'");
		}
	}
}

class Barbero implements Runnable {
	private int anyVar;
	public Thread thr;		//encapsulated object for the thread
	private Barberia barberia;		//encapsulated object for the monitor
	public Barbero(String name, int tNumb, Barberia tMon){
		// Initialization:
		anyVar = tNumb;
		barberia = tMon;
		thr = new Thread(this, name);
		System.out.println("La barberia està abierta ahora!");
	}
	public void run(){
		while(true){
			barberia.siguenteCliente();
			System.out.println ("En trabajo.");
			aux.dormir_max(2500);
			barberia.finCliente();
		}
	}
}

class Cliente implements Runnable {
	private int anyVar;
	public Thread thr;		//encapsulated object for the thread
	private Barberia barberia;		//encapsulated object for the monitor
	public Cliente(String name, int tNumb, Barberia tMon){
		// Initialization, for example:
		anyVar = tNumb;
		barberia = tMon;
		thr = new Thread(this, name);
	}
	public void run(){
		while(true){
			barberia.cortarPelo();
			aux.dormir_max(2000);
		}
	}
}

class barberoMain{
	public static void main(String[] args){
		try {
			int numberOfThreads = 5, i = 0;
	
			Barberia barberia = new Barberia();
			Cliente[] clients = new Cliente[numberOfThreads];		// the number of threads I want to execute concurrently
			
			for (i=0;i<numberOfThreads;i++){
				clients[i] = new Cliente("Cliente "+(i+1), i+1, barberia);		//Creates the threads
			}
			Barbero barbero = new Barbero("Barbero",0,barberia);
			
			barbero.thr.start();
			for (i=0;i<numberOfThreads;i++){
				clients[i].thr.start();		//Executes the threads previously created
			}
			for (i=0;i<numberOfThreads;i++){
				clients[i].thr.join();		//Waits for the threads to have finished
			}
			barbero.thr.join();
		} catch (InterruptedException e) { 
			System.out.println ("Exceptions happen sometimes.");
		}
	}
}

//======================================//
// 		SALIDA: 		//
//======================================//

/*
dave95@G6-DVX:~/GITs/SCD-Sistemas-Concurrentes-y-Distribuidos/Practicas/Practica 2$ javac barberoMain.java 
dave95@G6-DVX:~/GITs/SCD-Sistemas-Concurrentes-y-Distribuidos/Practicas/Practica 2$ java barberoMain 
La barberia està abierta ahora!
Soy el cliente n.1
En trabajo.
Soy el cliente n.2
Soy el cliente n.3
Soy el cliente n.4
Cliente hecho!
Siguiente! Ahora esperan en 3.
En trabajo.
Soy el cliente n.4
Cliente hecho!
Siguiente! Ahora esperan en 3.
En trabajo.
Soy el cliente n.4
Cliente hecho!
Siguiente! Ahora esperan en 3.
En trabajo.
Cliente hecho!
Siguiente! Ahora esperan en 2.
En trabajo.
Soy el cliente n.3
Soy el cliente n.4
Cliente hecho!
Siguiente! Ahora esperan en 3.
En trabajo.
Cliente hecho!
Siguiente! Ahora esperan en 2.
< ctrl + c >
*/
