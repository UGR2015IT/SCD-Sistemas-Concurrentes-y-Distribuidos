class Contador
{ private long valor;
  public Contador( long inicial )
  { valor = inicial ;
  }
  void retrasoOcupado() // ocupa la CPU durante cierto tiempo
  { long tmp = 0 ;
    for( int i = 0 ; i < 100000 ; i++ )
      tmp = tmp*i-tmp+i ;
  }
  public synchronized void incrementa () // incrementa valor en 1
  { long aux = valor ; // hace copia local del valor actual
    retrasoOcupado() ; // permite entrelazado cuando no se hace en EM
    valor = aux+1 ;    // escribe el valor compartido (incrementado)
  }
  public synchronized long getvalor () // devuelve el valor actual
  {  return valor;
  }
}

class Hebra implements Runnable
{ int numero ;               // cuenta múltiplos de este número
  public Thread thr ;        // objeto encapsulado
  private Contador cont;     // contador de número de múltiplos

  public Hebra( String nombre, int p_numero, Contador p_contador )
  { numero = p_numero;
    cont   = p_contador;
    thr = new Thread( this, nombre );
  }
  public void run ()
  { for ( int i = 1 ; i <= 1000 ; i++ )
      if (i % numero == 0)
        cont.incrementa();
  }
}

class Multiplos
{ public static void main( String[] args )
  { try
    { Contador contH = new Contador(0); // contador usado por la hebras
      Hebra[] vc = new Hebra[2] ;
      vc[0] = new Hebra( "Contador2 ", 2, contH);
      vc[1] = new Hebra( "Contador3 ", 3, contH);
      vc[0].thr.start(); vc[1].thr.start(); // lanza hebras
      vc[0].thr.join();  vc[1].thr.join();  // espera terminación
      System.out.println("Resultado hebras  : "+contH.getvalor());
      long contV = 0 ;  // contador de verificacion
      for ( int i = 1 ; i <= 1000 ; i++ )
      { if ( i % 2 == 0 ) contV = contV + 1 ;
        if ( i % 3 == 0 ) contV = contV + 1 ;
      }
      System.out.println("Resultado correcto: " + contV);
    }
    catch (InterruptedException e)
    { System.out.println ("ha ocurrido una excepcion.");
    }
  }
}
