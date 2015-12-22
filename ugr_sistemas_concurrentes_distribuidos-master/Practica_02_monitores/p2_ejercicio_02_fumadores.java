/******************************************************************************
 *
 * SCD - Sistemas Concurrentes y Distribuidos
 * Grado en Ingeniería Informática
 *
 * 2014 - Ernesto Serrano <erseco@correo.ugr.es>
 * ---------------------------------------------
 *
 *  El problema de los fumadores con monitores haroe
 *
 *
 *    // COLORES ANSI PARA LA CONSOLA
 *    System.out.println("\033[0m BLACK");
 *    System.out.println("\033[31m RED");
 *    System.out.println("\033[32m GREEN");
 *    System.out.println("\033[33m YELLOW");
 *    System.out.println("\033[34m BLUE");
 *    System.out.println("\033[35m MAGENTA");
 *    System.out.println("\033[36m CYAN");
 *    System.out.println("\033[37m WHITE");
 *
 ******************************************************************************/

import java.util.Random;
import monitor.*;


class Estanco extends AbstractMonitor
{
    private int ing_anterior;
    private Condition sem_estanquero = makeCondition();
    private Condition[] sem_fumador = new Condition[3];

    public Estanco()
    {
        this.ing_anterior = -1;
        for (int i=0; i<3; i++)
            sem_fumador[i] = makeCondition();
    }

    // invocado por cada fumador, indicando su ingrediente o numero
    public void obtenerIngrediente(int miIngrediente)
    {
        enter();

            if (miIngrediente != ing_anterior)
            {
                sem_fumador[miIngrediente].await();

            }
            System.out.println("\033[32m" + "Fumador, fumando        " + miIngrediente + "\033[0m");
            // El fumador recoge el ingrediente (pone -1)
            ing_anterior = -1;
            sem_estanquero.signal();

        leave();
    }
    // invocado por el estanquero, indicando el ingrediente que pone
    public void ponerIngrediente(int ingrediente)
    {
        enter();

            ing_anterior = ingrediente;
            System.out.println("\033[31m" + "Estanquero, repartiendo " + ingrediente + "\033[0m");
            sem_fumador[ingrediente].signal();

        leave();
    }
    // invocado por el estanquero
    public void esperarRecogidaIngrediente()
    {
        enter();

            if (ing_anterior != -1)
                sem_estanquero.await();

        leave();
    }

}

class Estanquero implements Runnable
{
    private Estanco estanco;
    Thread thr;
    public Estanquero(Estanco pMonEstanco)
    {
        estanco    = pMonEstanco;
        thr   = new Thread(this,"estanquero");
    }
    public void run()
    {
        try
        {
            int ingrediente;
            while (true)
            {
                ingrediente = (int) (Math.random() * 3.0); // 0,1 o 2
                estanco.ponerIngrediente(ingrediente);
                estanco.esperarRecogidaIngrediente();
            }
        }
        catch(Exception e)
        {
            System.err.println("Excepcion en main: " + e);
        }
    }
}

class Fumador implements Runnable
{
    private Estanco estanco;
    private int miIngrediente;;
    public Thread thr;
    public Fumador(Estanco pMonEstanco, int p_miIngrediente)
    {
        estanco    = pMonEstanco;
        miIngrediente  = p_miIngrediente;
        thr   = new Thread(this,"fumador " + p_miIngrediente);
    }
    public void run()
    {
        try
        {
            while(true)
            {
                estanco.obtenerIngrediente(miIngrediente);
                auxFumadores.dormir_max(2000);
            }
        }
        catch(Exception e)
        {
            System.err.println("Excepcion en main: " + e);
        }
    }
}

class auxFumadores
{
    static Random genAlea = new Random();
    static void dormir_max(int milisecsMax)
    {
        try
        {
            Thread.sleep(genAlea.nextInt(milisecsMax));
        }
        catch(InterruptedException e)
        {
            System.err.println("sleep interumpido en 'aux.dormir_max()'");
        }
    }
}

class p2_ejercicio_02_fumadores
{
    public static void main(String[] args)
    {

        // leer parametros, crear vectores y buffer intermedio
        Estanco estanco = new Estanco();
        Estanquero estanquero = new Estanquero(estanco);
        Fumador[] fumadores = new Fumador[3];


        fumadores[0]= new Fumador(estanco, 0);
        fumadores[1]= new Fumador(estanco, 1);
        fumadores[2]= new Fumador(estanco, 2);


        // crear hebras

        // poner en marcha las hebras
        estanquero.thr.start();
        fumadores[0].thr.start();
        fumadores[1].thr.start();
        fumadores[2].thr.start();


    }
}

