//Name: Blayten Jones
//ID: 010979697

import java.util.Random;
import java.util.concurrent.*;
import java.util.List;
import java.util.ArrayList;

class DiningPhilosopher{
	//Split the table into 5 sections for each chopstick combination	
	//Define necessary semaphores
	private static Semaphore chopstick12Mutex, chopstick23Mutex, chopstick34Mutex, chopstick45Mutex,
	chopstick51Mutex;

	//Define default values
	static double sleepTime = 20;

	private static Random generator = new Random();

	public static void main(String[] args) throws Exception{
		//Check to see if program has been called correctly
		if(args.length != 1){
			System.out.println("INCORRECT NUMBER OF ARGUMENTS. PLEASE PROVIDE ONE ARGUMENT (FLOAT) IN THIS FORMAT:\n\"java DiningPhilosopher [INSERT NUMBER OF SECONDS TO RUN PHILOSOPHERS]\"");
			System.out.println("USING DEFAULT VALUE (sleepTime = 20.0)!");
		}else{
			try{
				double arg1 = Double.parseDouble(args[0]);
				sleepTime = arg1;
			}catch(NumberFormatException e){
				System.out.println("INCORRECT FORMAT OF ARGUMENTS. PLEASE PROVIDE ONE ARGUMENT (FLOAT) IN THIS FORMAT:\n\"java DiningPhilosopher [INSERT NUMBER OF SECONDS TO RUN PHILOSOPHERS]\"");
				System.exit(1);
			}
		}

		//Establish mutex
		chopstick12Mutex = new Semaphore(1); chopstick23Mutex = new Semaphore(1); chopstick34Mutex = new Semaphore(1);
		chopstick45Mutex = new Semaphore(1); chopstick51Mutex = new Semaphore(1);

		Semaphore[] mutexes = {chopstick12Mutex, chopstick23Mutex, chopstick34Mutex, chopstick45Mutex, chopstick51Mutex};

		List<Thread> threadList = new ArrayList<Thread>();

		long startTime = System.currentTimeMillis();
		long currTime = System.currentTimeMillis();

		int i = 0;
		while(((currTime - startTime)/1000 < sleepTime)){
			if(i < 5){
				Philosopher newPhilosopher = new Philosopher(mutexes[(4+i)%5], mutexes[i]);
				Thread tN = new Thread(newPhilosopher, "Philosopher " + String.valueOf(i + 1));
				threadList.add(tN); newPhilosopher.setThread(tN);
				tN.start();
				i++;
			}
			Thread.sleep(generator.nextInt((int)(20*sleepTime)));
			currTime = System.currentTimeMillis();
		}

	    currTime = System.currentTimeMillis();

		//Measure time
		String CPUTime = String.valueOf(((double)(currTime - startTime))/1000.0);
		System.out.println("DINNER HAS FINISHED; PHILOSOPHERS ATE WITHIN THE GIVEN TIME OF " + CPUTime + " SECONDS!");
        System.exit(0);
    }
}

class Philosopher implements Runnable {
	private String name;
	private final Random generator = new Random();

	private Semaphore left, right;
	private Thread t;

	//Initialize the philosopher
	public Philosopher(Semaphore left, Semaphore right){
		this.left = left;
		this.right = right;
	}

	public void setThread(Thread t){
		this.t = t;
		this.name = t.getName();
		System.out.println(name + " Thinking");
	}
	
	//Tries to get the waiting room mutex to see if there are enough permits
	private void getChopsticks(){
		if(left.tryAcquire()){
            if(right.tryAcquire()){
                System.out.println(name + " Picked up left chopstick");
                System.out.println(name + " Picked up right chopstick");
                eat();
            }else{
                left.release();
            }
		}
	}

	private void eat(){
		try{
			Thread.sleep(generator.nextInt(1000));
		}catch(InterruptedException e){
			e.printStackTrace();
		}finally{
			left.release();
			System.out.println(name + " Put down left chopstick");
			right.release();
			System.out.println(name + " Put down right chopstick");
		}
	}

	private void think(){
		System.out.println(name + " Thinking");
		try{
			Thread.sleep(generator.nextInt(1000));
		}catch(InterruptedException e){
			e.printStackTrace();
		}
	}

	public void run(){
        try{
			Thread.sleep(generator.nextInt(1000));
		}catch(InterruptedException e){
			e.printStackTrace();
		}
        while(true){
		    think();
	        getChopsticks();
        }
	}
}