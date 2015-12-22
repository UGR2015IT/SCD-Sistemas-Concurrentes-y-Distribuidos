/******************************************************************************
 *
 * SCD - Sistemas Concurrentes y Distribuidos
 * Grado en Ingeniería Informática
 *
 * 2014 - Ernesto Serrano <erseco@correo.ugr.es>
 * ---------------------------------------------
 *
 *  El problema del productor-consumidor con monitores haroe
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

import monitor.*;


class ej1_Buffer extends AbstractMonitor
{
    private int numSlots = 0, cont = 0;
    private double[] buffer = null;

    private Condition produciendo = makeCondition();
    private Condition consumiendo = makeCondition();


    public ej1_Buffer(int p_numSlots)
    {
        numSlots = p_numSlots;
        buffer = new double[numSlots];
    }

    public void depositar(double valor) throws InterruptedException
    {
        enter();
            if (cont == numSlots)
                produciendo.await();
            buffer[cont] = valor;
            cont++;
            consumiendo.signal();
        leave();
    }

    public double extraer() throws InterruptedException
    {
        enter();
            double valor;
            if (cont == 0)
                consumiendo.await();
            cont--;
            valor = buffer[cont];
            produciendo.signal();
        leave();
        return valor;
    }
}

class ej1_Productor implements Runnable
{
    private ej1_Buffer bb;
    int veces;
    int numP;
    Thread thr;
    public ej1_Productor(ej1_Buffer pbb, int pveces, int pnumP)
    {
        bb    = pbb;
        veces = pveces;
        numP  = pnumP;
        thr   = new Thread(this,"productor "+numP);
    }
    public void run()
    {
        try
        {
            double item = 100*numP;
            for (int i=0; i<veces; i++)
            {
                System.out.println("\033[32m"+thr.getName() + ", produciendo " + item + "\033[0m");
                bb.depositar(item++);
            }
        }
        catch(Exception e)
        {
            System.err.println("Excepcion en main: " + e);
        }
    }
}

class ej1_Consumidor implements Runnable
{
    private ej1_Buffer bb;
    int veces;
    int numC;
    Thread thr;
    public ej1_Consumidor(ej1_Buffer pbb, int pveces, int pnumC)
    {
        bb    = pbb;
        veces = pveces;
        numC  = pnumC;
        thr   = new Thread(this,"consumidor "+numC);
    }
    public void run()
    {
        try
        {
            for (int i=0; i<veces; i++)
            {
                double item = bb.extraer ();
                System.out.println("\033[31m"+thr.getName() + ", consumiendo " + item + "\033[0m");
            }
        }
        catch(Exception e)
        {
            System.err.println("Excepcion en main: " + e);
        }
    }
}


class p2_ejercicio_01_productor_consumidor
{
    public static void main(String[] args)
    {
        if (args.length != 5)
        {
            System.err.println("Uso: ncons nprod tambuf niterp niterc");
            return;
        }

        // leer parametros, crear vectores y buffer intermedio

        ej1_Consumidor[] cons = new ej1_Consumidor[Integer.parseInt(args[0])];
        ej1_Productor[]  prod = new ej1_Productor[Integer.parseInt(args[1])];
        ej1_Buffer buffer = new ej1_Buffer(Integer.parseInt(args[2]));
        int iter_cons = Integer.parseInt(args[3]);
        int iter_prod = Integer.parseInt(args[4]);

        if (cons.length*iter_cons != prod.length*iter_prod)
        {
            System.err.println("no coinciden número de items a producir con a consumir");
            return;
        }

        // crear hebras
        for (int i = 0; i < cons.length; i++)
            cons[i] = new ej1_Consumidor(buffer,iter_cons,i);

        for (int i = 0; i < prod.length; i++)
            prod[i] = new ej1_Productor(buffer,iter_prod,i);

        // poner en marcha las hebras
        for (int i = 0; i < prod.length; i++)
            prod[i].thr.start();

        for (int i = 0; i < cons.length; i++)
            cons[i].thr.start();
    }
}

