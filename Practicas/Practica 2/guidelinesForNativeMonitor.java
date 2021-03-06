//====================================================//
//  Follow this guideline to solve an exercise with   //
//	   	JAVA NATIVE MONITORS		      //
//====================================================//

class Monitor{
	// Class which incapsulate the monitor	
	private int sharedVar;

	public Monitor(int value){
		// Constructor
		sharedVar = value;
	}
	
	private void anyOtherMethod(){
		//And his code
	}

	public synchronized int getSharedVar(){
		while (conditionToBeChecked) wait();
		//MUTEX Code
		signalAll();
		return sharedVar;
	}

	public synchronized void setSharedVar(int value){
		while (!conditionToBeChecked) wait();
		//MUTEX Code, for example:
		sharedVar = value;
		signalAll();
	}
}

class MyThread implements Runnable{
	private int anyVar;
	public Thread thr;		//encapsulated object for the thread
	private Monitor mon;		//encapsulated object for the monitor
	public MyThread(String name, int tNumb, Monitor tMon){
		// Initialization, for example:
		anyVar = tNumb;
		mon = pMon;
		thr = new Thread(this, name);
	}
	public void run(){
		//Method executed concurrently by the hebra, for example:
		mon.getSharedVar();
	}
}

class basis{
	int numberOfThreads = 5, i = 0;
	public static void main(String[] args){
		try {
			Monitor threadsMon = new Monitor(0);
			MyThread[] myThreads = new MyThread[numberOfThreads];		// the number of threads I want to execute concurrently
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
