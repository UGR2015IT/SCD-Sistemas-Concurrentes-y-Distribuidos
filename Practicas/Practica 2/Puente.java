//====================================================//
//  			PUENTE			      //
//====================================================//

import monitor.*;

class Puente extends AbstractMonitor {
	// Class which incapsulate the monitor	
	private Condition cocheN = makeCondition();
	private Condition cocheS = makeCondition();
	private int cocheNCruzando, cocheSCruzando;

	public Puente(){
		cocheNCruzando = 0;
		cocheSCruzando = 0;
	}

	public void entrarCocheN(){
		enter();
			if ( cocheSCruzando>0 )
				cocheN.await();
			System.out.println("Un coche del Norte està cruzando.");
			cocheNCruzando++;
			cocheN.signal();
		leave();
	}

	public void salirCocheN(){
		enter();
			cocheNCruzando--;
		leave();
	}

	public void entrarCocheS(){
		enter();
			if (cocheNCruzando>0 )
				cocheS.await();
			System.out.println("Un coche del Sur està cruzando.");
			cocheSCruzando++;
			cocheS.signal();
		leave();
	}

	public void salirCocheS(){
		enter();
			cocheSCruzando--;
		leave();
	}
}

class cocheN implements Runnable {
	int cocheNumero;
	public Thread thr;		//encapsulated object for the thread
	private Puente puente;		//encapsulated object for the monitor
	public cocheN(String name, int tNumb, Puente tMon){
		// Initialization, for example:
		cocheNumero = tNumb;
		puente = tMon;
		thr = new Thread(this, name);
	}
	public void run(){
		//Method executed concurrently by the thread
		try { 
			puente.entrarCocheN();
			Thread.sleep( 1000 ) ;
			puente.salirCocheN();
		} catch( InterruptedException e ) { 
			System.err.println("There's something wrong with cocheN n°"+cocheNumero);
		}
	}
}

class cocheS implements Runnable {
	int cocheNumero;
	public Thread thr;		//encapsulated object for the thread
	private Puente puente;		//encapsulated object for the monitor
	public cocheS(String name, int tNumb, Puente tMon){
		// Initialization
		cocheNumero = tNumb;
		puente = tMon;
		thr = new Thread(this, name);
	}
	public void run(){
		//Method executed concurrently by the thread
		try { 
			puente.entrarCocheS();
			Thread.sleep( 1000 ) ;
			puente.salirCocheS();
		} catch( InterruptedException e ) { 
			System.err.println("There's something wrong with cocheS n°"+cocheNumero);
		}
	}
}

class PuenteMain{
	
	public static void main(String[] args){
		try {
			int coches = 5, i = 0;
			Puente myPuente = new Puente();
			cocheN[] carsN = new cocheN[coches];		// the number (and type) of threads I want to execute concurrently
			cocheS[] carsS = new cocheS[coches];		// the number (and type) of threads I want to execute concurrently
			for (i=0;i<coches;i++){
				carsN[i] = new cocheN("CocheN n°"+(i+1), i+1, myPuente);		//Creates the threads
			}
			for (i=0;i<coches;i++){
				carsS[i] = new cocheS("CocheS n°"+(i+1), i+1, myPuente);		//Creates the threads
			}
			for (i=0;i<coches;i++){
				carsN[i].thr.start();		//Executes the threads previously created
				carsS[i].thr.start();		//Executes the threads previously created
			}
			for (i=0;i<coches;i++){
				carsN[i].thr.join();		//Waits for the threads to have finished
				carsS[i].thr.join();		//Waits for the threads to have finished
			}
		} catch (InterruptedException e) { 
			System.out.println ("Exceptions happen sometimes.");
		}
	}
}
