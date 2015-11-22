import monitor.*;
import java.util.Random;

class Olla extends AbstractMonitor {
	// Class which incapsulate the monitor	
	private int misionero;
	private static final int maxOlla = 10;

	private Condition salvaje = makeCondition();		//for example, read
	private Condition cocinero = makeCondition();		//for example, write

	public Olla(){
		// Constructor
		misionero = maxOlla;
	}

	public void Dormir() throws InterruptedException {
		enter();
			if (misionero != 0) cocinero.await();
		leave();
	}

	public void rellenarOlla() throws InterruptedException {
		enter();
			misionero = maxOlla;
			salvaje.signal();
		leave();
	}
	
	public void comidaSalvaje() throws InterruptedException {
		enter();
			if (misionero == 0) {
				System.out.println(">> La olla esta vacia!");
				cocinero.signal();
				salvaje.await();
			}
			System.out.println("Se come!");
			misionero--;
		leave();
	}
}

class Cocinero implements Runnable {
	public Thread thr;
	private Olla olla;
	
	public Cocinero(String name, Olla pMon){
		thr = new Thread(this, name);
		olla = pMon;
	}
	public void run(){
		while(true){
			try { 
				olla.Dormir();
				System.out.println("> Ahora la olla esta llena!");
				olla.rellenarOlla();
			} catch( InterruptedException e ) { 
				System.err.println("Interumpido");
			}
		}
	}
}

class Salvaje implements Runnable {
	public Thread thr;
	private Olla olla;
	static Random genAlea = new Random() ;

	public Salvaje(String name, Olla pMon){
		thr = new Thread(this, name);
		olla = pMon;
	}
	
	private void comer(){
		try { 
			Thread.sleep( genAlea.nextInt( 5000 ) ) ;
		} catch( InterruptedException e ) { 
			System.err.println("sleep interumpido");
		}
	}

	public void run(){
		while (true){
			try {
				olla.comidaSalvaje();
				comer();
			} catch (InterruptedException e) {
				System.err.println("Interrumpido");
			}
		}
	}
}

class Tribu{
	private static final int numeroSalvajes = 15; 
	public static void main(String[] args){
		int i=0;
		try {
			Olla olla = new Olla();
			Cocinero coc = new Cocinero("Cocinero", olla);
			Salvaje[] salv = new Salvaje[numeroSalvajes];
			
			for (i=0;i<numeroSalvajes;i++)
				salv[i] = new Salvaje("Salvaje"+i, olla);

			coc.thr.start();
			for (i=0;i<numeroSalvajes;i++)
				salv[i].thr.start();

			for (i=0;i<numeroSalvajes;i++)
				salv[i].thr.join();
			coc.thr.join();

		} catch (InterruptedException e) { 
			System.out.println ("Exceptions happen sometimes.");
		}
	}
}
