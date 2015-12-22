import java.util.Random ;

class TipoHebra implements Runnable // opcionalmente: extends ....
{ 
  long siesta  ;      // tiempo que duerme la hebra
  public Thread thr ; // objeto hebra encapsulado
  
  public TipoHebra( String nombre, long siesta ) 
  { this.siesta = siesta;
    thr = new Thread( this, nombre );
  }
  public void run() 
  { try
    { while ( true ) 
      { System.out.println( "Hola, soy "+thr.getName() );
        if ( siesta > 0 ) Thread.sleep( siesta );  
      }
    }
    catch ( InterruptedException e ) 
    { System.out.println( "me fastidiaron la siesta!" );
    }
  }
}

class Principal1 
{
  public static void main( String[] args ) throws InterruptedException 
  {
    if ( args.length < 1 )
    {  System.out.println( "Error: falta valor de 'siesta'" );
       System.exit(1);
    }
    long siesta = Long.parseLong( args[0] ) ;
    TipoHebra obj = new TipoHebra("hebra 'obj'", siesta); // crear hebra
    
    obj.thr.start();     // lanzar hebra
    Thread.sleep( 100 ); // la hebra principal duerme 1/10 sec.
    obj.thr.join();      // esperar a que termine la hebra
  }
}

//**********************************************************************

class Dormilona implements Runnable 
{ 
  int vueltas = 0 ; // número de veces que duerme (0 == infinitas) 
  int siesta  = 0 ; // tiempo máximo que duerme cada vez
  public Thread thr ; // objeto hebra encapsulado        

  public Dormilona( String p_nombre, int p_vueltas, int p_siesta ) 
  { 
    siesta  = p_siesta ;
    vueltas = p_vueltas ;
    thr     = new Thread( this, p_nombre ) ; 
  }
  
  //...
  //...
  public void run()
  { try
    { Random random = new Random(); // crea generador de números aleatorios
      // dormir un numero de veces igual a "vueltas"
      for ( int i=0 ; i < vueltas || vueltas == 0 ; i++ )
      { // imprimir nombre
        System.out.println( "Vuelta no."+i+", de " +thr.getName());
        // duerme un tiempo aleatorio entre 0 y siesta-1 milisegundos
        if ( siesta > 0 ) 
          Thread.sleep( random.nextInt( siesta ) );
      }
    }
    catch( InterruptedException e )
    { System.out.println( Thread.currentThread().getName()+
                              ": me fastidiaron la siesta!");
    }
  } // fin {\tt run}
} // fin de la clase {\tt\bf Dormilona}


class Principal2
{
  public static void main( String[] args ) 
  { try
    { if ( args.length < 3 )
      { System.out.println( "falta num_hebras, t_max_siesta, vueltas" );
        System.exit( 1 );
      }
      int nHebras = Integer.parseInt( args[0] );
      int siesta  = Integer.parseInt( args[1] );
      int vueltas = Integer.parseInt( args[2] );
    
      System.out.println( "nHebras = "+nHebras+", vueltas = "+vueltas+
                            ", tsiesta = "+siesta );
      //.... 
      //....
            
      Dormilona[] vhd = new Dormilona[nHebras] ; // crea vector de hebras
      
      for ( int i =0 ; i < nHebras ; i++ ) 
      { String nombre = "Dormilona no."+i ;
        vhd[i] = new Dormilona(nombre,vueltas,siesta); // crear hebra i.
        vhd[i].thr.start();                        // comienza su ejec.
      }
      // la hebra principal duerme durante un segundo
      Thread.sleep( 1000 );
      System.out.println( "main(): he terminado de dormir" );
      
      // esperar a que terminen todas las hebras creadas
      for( int i = 0 ; i < nHebras ; i++ )
        vhd[i].thr.join();  
    } 
    catch( InterruptedException e ) 
    { System.out.println( "main(): me fastidiaron la siesta!" );
    }
  }
}
 
