/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package servidor;

/**
 *
 * @author luis
 */
import java.awt.*;
import java.net.*;
import java.io.*;
 class TCPServidor
 {

public static void main(String args[]) throws Exception
 {
ServerSocket s = (ServerSocket)null;
        Socket s1;
        String cadena = "Tutorial de Java!";
        int longCad;
        OutputStream s1out;

        // Establece el servidor en el socket 4321 (espera 300 segundos)
        try {
            s = new ServerSocket( 4321,300 );
        } catch( IOException e ) {
            System.out.println( e );
            }

        // Ejecuta un bucle infinito de listen/accept
        while( true ) {
            try {
                // Espera para aceptar una conexión
                s1 = s.accept();
                // Obtiene un controlador de fichero de salida asociado
                // con el socket
                s1out = s1.getOutputStream();

                // Enviamos nuestro texto
                longCad =cadena.length();
                for( int i=0; i < longCad; i++ )
                    s1out.write( (int)cadena.charAt( i ) );

                // Cierra la conexión, pero no el socket del servidor
                s1.close();
            } catch( IOException e ) {
                System.out.println( e );
                }
            }
 }
 }

