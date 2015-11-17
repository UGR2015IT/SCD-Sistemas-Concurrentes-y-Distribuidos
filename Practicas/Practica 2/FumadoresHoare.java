// ============================================ //
// 		Ejercicio 2: 			//
// 	   Problema de los fumadores		//
// ============================================ //

/* */

// ****************************************************************************

import monitor.*;
import java.util.Random;

class Estanco extends AbstractMonitor{
	int enLaMesa;
	private Condition condEstanquero = makeCondition();
	private Condition[] fumando = new Condition[3];
	public Estanco(){
		for (int i=0;i<3;i++)
			fumando[i] = makeCondition();
		enLaMesa = -1;
	}

	public void obtenerIngrediente(int miIngrediente){
		enter();
			// invocado por cada fumador, indicando ingrediente o numero
			if ( miIngrediente != enLaMesa )
				fumando[miIngrediente].await();
			System.out.println("Fumador "+miIngrediente+" està fumando");
			enLaMesa = -1;
			condEstanquero.signal();
		leave();
	}

	public void ponerIngrediente(int ingrediente){
		enter();
			// invocado por el estanquero, indicando el ingrediente que pone
			enLaMesa = ingrediente;
			System.out.println("Estanquero reparte "+ingrediente);
			fumando[ingrediente].signal();
		leave();
	}

	public void esperarRecogidaIngrediente(){
		enter();
			// invocado por el estanquero
			if (enLaMesa != -1)
				condEstanquero.await();
		leave();
	}
}

class Fumador implements Runnable{
	private Estanco estanco;
	int miIngrediente;
	public Thread thr;
	public Fumador(Estanco p_MonEstanco, int p_miIngrediente){
		//Initialize thread
		estanco = p_MonEstanco;
		miIngrediente = p_miIngrediente;
		thr = new Thread(this, "fumador " + p_miIngrediente);
	}
	public void run(){
		try {
			while (true){
				estanco.obtenerIngrediente(miIngrediente);
				aux.dormir_max(2000);
			}
		} catch(Exception e) {System.err.println("Excepcion en main: " + e);}	
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

class Estanquero implements Runnable{
	private Estanco estanco;
	public Thread thr;
	public Estanquero(Estanco p_MonEstanco){
		estanco = p_MonEstanco;
		thr = new Thread(this,"estanquero");
	}
	public void run(){
		try{
			int ingrediente;
			while (true){
				ingrediente = (int) (Math.random() * 3.0);
				estanco.ponerIngrediente(ingrediente);
				estanco.esperarRecogidaIngrediente();
			}
		}catch(Exception e){
	        	System.err.println("Excepcion en main: " + e);
		}
	}
}

class FumadoresHoare{
	
	public static void main( String[] args ) { 
		try {
			Estanco estanco = new Estanco();
			Estanquero estanquero = new Estanquero(estanco);
			Fumador[] fum = new Fumador[3];

			for (int i=0;i<3;i++)
				fum[i] = new Fumador(estanco, i);

			estanquero.thr.start();
	
			for (int i=0;i<3;i++)
				fum[i].thr.start();

			for (int i=0;i<3;i++)
				fum[i].thr.join();

			estanquero.thr.join();
		} catch (InterruptedException e) {
			System.out.println("Exceptions happen");
		}
	}
}
