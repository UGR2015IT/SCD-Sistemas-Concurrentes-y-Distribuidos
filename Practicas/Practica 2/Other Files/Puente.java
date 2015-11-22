//====================================================//
//  			PUENTE			      //
//====================================================//

import monitor.*;

class Puente extends AbstractMonitor {
	// Class which incapsulate the monitor	
	private Condition cocheN = makeCondition();
	private Condition cocheS = makeCondition();
	private int cocheNCruzando, cocheSCruzando, i;
	public static final int time = 1;

	public Puente(){
		cocheNCruzando = 0;
		cocheSCruzando = 0;
		i=0;
	}

	public void entrarCocheN(){
		enter();
			System.out.println(">> Un nuevo coche del Norte quiere cruzar.");
			if ( cocheSCruzando!=0 ){
				System.out.println(" >> Pero estan otros del Sur cruzando! "+cocheSCruzando);
				cocheN.await();
			}
			System.out.println("Un coche del Norte està cruzando.");
			cocheNCruzando++;
		leave();
	}

	public void salirCocheN(){
		enter();
			cocheNCruzando--;
			System.out.println(cocheNCruzando+" coches del Norte tienen que cruzar.");
			if (cocheNCruzando==0) 
				while(!cocheS.isEmpty()) cocheS.signal();
		leave();
	}

	public void entrarCocheS(){
		enter();
			System.out.println(">> Un nuevo coche del Sur quiere cruzar.");
			if ( cocheNCruzando!=0 ){
				System.out.println(" >> Pero estan otros del Norte cruzando! "+cocheNCruzando);
				cocheS.await();
			}
			System.out.println("Un coche del Sur està cruzando.");
			cocheSCruzando++;
		leave();
	}

	public void salirCocheS(){
		enter();
			cocheSCruzando--;
			System.out.println(cocheSCruzando+" coches del Sur tienen que cruzar.");
			if (cocheSCruzando==0) 
				while(!cocheN.isEmpty()) cocheN.signal();
		leave();
	}
}

class cocheN implements Runnable {
	//int cocheNumero;
	public Thread thr;		//encapsulated object for the thread
	private Puente puente;		//encapsulated object for the monitor
	public cocheN(String name, Puente tMon){
		// Initialization, for example:
		//cocheNumero = tNumb;
		puente = tMon;
		thr = new Thread(this, name);
	}
	public void run(){
		//Method executed concurrently by the thread
		try { 
			//while(true){
				puente.entrarCocheN();
				Thread.sleep( puente.time ) ;
				puente.salirCocheN();
			//}
		} catch( InterruptedException e ) { 
			System.err.println("There's something wrong with one cocheN");
		}
	}
}

class cocheS implements Runnable {
	//int cocheNumero;
	public Thread thr;		//encapsulated object for the thread
	private Puente puente;		//encapsulated object for the monitor
	public cocheS(String name, Puente tMon){
		// Initialization
		//cocheNumero = tNumb;
		puente = tMon;
		thr = new Thread(this, name);
	}
	public void run(){
		//Method executed concurrently by the thread
		try { 
			//while(true){
				puente.entrarCocheS();
				Thread.sleep( puente.time ) ;
				puente.salirCocheS();
			//}
		} catch( InterruptedException e ) { 
			System.err.println("There's something wrong with one cocheS");
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
				carsN[i] = new cocheN("CocheN n°"+(i+1), myPuente);		//Creates the threads
			}
			for (i=0;i<coches;i++){
				carsS[i] = new cocheS("CocheS n°"+(i+1), myPuente);		//Creates the threads
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
