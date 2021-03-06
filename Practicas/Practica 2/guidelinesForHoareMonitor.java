//====================================================//
//  Follow this guideline to solve an exercise with   //
//	   	  JAVA HOARE MONITORS		      //
//====================================================//

import monitor.*;

class Monitor extends AbstractMonitor {
	// Class which incapsulate the monitor	
	private int sharedVar;							//for example, number of readers

	private boolean conditionToBeChecked = false;				//for example, writing
	private Condition oneActionOnCondition = makeCondition();		//for example, read
	private Condition otherActionOnCondition = makeCondition();		//for example, write

	public Monitor(int value){
		// Constructor
		sharedVar = value;
	}
	
	private void anyOtherMethod(){
		//And his code
	}

	public int getSharedVar() throws InterruptedException {
		// The old synchronized synthax is substituted by
		// enter(); mutual_exclusion_code; leave();
		enter();
			if (conditionToBeChecked) oneActionOnCondition.await();
			//MUTEX Code
			otherActionOnCondition.signal();
		leave();
		return value;
	}

	public void setSharedVar(int value) throws InterruptedException {
		// The old synchronized synthax is substituted by
		// enter(); mutual_exclusion_code; leave();
		enter();
			if (!conditionToBeChecked) otherActionOnCondition.wait();
			//MUTEX Code
			oneActionOnCondition.signal();
		leave();
	}
}

class actionOneThread implements Runnable {
	private int anyVar;
	public Thread thr;		//encapsulated object for the thread
	private Monitor mon;		//encapsulated object for the monitor
	public actionOneThread(String name, int tNumb, Monitor tMon){
		// Initialization, for example:
		anyVar = tNumb;
		mon = pMon;
		thr = new Thread(this, name);
	}
	public void run(){
		//Method executed concurrently by the thread, for example:
		mon.getSharedVar();
		// It's called from within the monitor, and it's in MUTEX as of definition
	}
}

class actionOtherThread implements Runnable {
	private int anyVar;
	public Thread thr;		//encapsulated object for the thread
	private Monitor mon;		//encapsulated object for the monitor
	public actionOtherThread(String name, int tNumb, Monitor tMon){
		// Initialization, for example:
		anyVar = tNumb;
		mon = pMon;
		thr = new Thread(this, name);
	}
	public void run(){
		//Method executed concurrently by the thread, for example:
		mon.getSharedVar();
		// It's called from within the monitor, and it's in MUTEX as of definition
	}
}

class basis{
	int numberOfThreads = 5, i = 0;
	public static void main(String[] args){
		try {
			Monitor threadsMon = new Monitor(0);
			oneActionThread[] myThreads = new MyThread[numberOfThreads];		// the number (and type) of threads I want to execute concurrently
			for (i=0;i<numberOfThreads;i++){
				myThreads[i] = new MyThread("Monitor"+(i+1), i+1, threadsMon);		//Creates the threads
			}
			for (i=0;i<numberOfThreads;i++){
				myThreads[i].thr.start();		//Executes the threads previously created
			}
			for (i=0;i<numberOfThreads;i++){
				myThreads[i].thr.join();		//Waits for the threads to have finished
			}
		} catch (InterruptedException e) { 
			System.out.println ("Exceptions happen sometimes.");
		}
	}
}
