/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cliente;

/**
 *
 * @author luis
 */
import java.io.*;
import java.net.*;

class TCPCliente {

    public static void main(String args[]) throws IOException {
        String latitud = "";
        String longitud = "";
        String hora = "";

        int c, longCad, puerto = 51717;
        String direccion = "192.168.100.15";
        String cadena = "ok";
        Socket s = null;
        InputStream sIn;
        OutputStream s1out;

        // Abrimos una conexión con breogan en el puerto 4321
        try {
            s = new Socket(direccion, puerto);
        } catch (IOException e) {
            System.out.println(e);
        }

        // Obtenemos un controlador de fichero de entrada del socket y
        // leemos esa entrada
        sIn = s.getInputStream();
        s1out = s.getOutputStream();

        //Recibimos datos de la aplicación
        while ((c = sIn.read()) != '\0') {
            if(c != 'Â')
                latitud += (char) c;

        }
        System.out.println("-->"+latitud);
        //Enviamos la cadena que deseeamos
        longCad = cadena.length();
        for (int i = 0; i < longCad; i++) {
            s1out.write((int) cadena.charAt(i));
            
        }

        //Recibimos datos de la aplicación
        while ((c = sIn.read()) != '\0') {
            if(c != 'Â')
                longitud += (char) c;
        }
        System.out.println("-->"+longitud);

        //Enviamos la cadena que deseeamos
        longCad = cadena.length();
        for (int i = 0; i < longCad; i++) {
            s1out.write((int) cadena.charAt(i));
        }
        
        
        
        
        //Recibimos datos de la aplicación
        while ((c = sIn.read()) != 'g') {
            hora += (char) c;
           
        }
        hora += 'g';
        System.out.println("-->"+hora);

        //Enviamos la cadena que deseeamos
        longCad = cadena.length();
        for (int i = 0; i < longCad; i++) {
            s1out.write((int) cadena.charAt(i));
        }

        //Recibimos datos de la aplicación
        
        
        
        

        // Cuando se alcance el fin de fichero, cerramos la conexión y
        // abandonamos
        s.close();
    }
}
