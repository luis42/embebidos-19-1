#Práctica 2
##Reporte

En el presente escrito es para documentar la realización de la práctica del **Tema Soc-Raspberry**.

1. Conexión a la red inalámbrica.

	Para poder lograr la conexión de nuestro sistema embebido, tenemos que conectarnos a la red
	inalámbrica, modificando la configuración por defecto del sistema.

	1.1 *Comandos ejecutados**

		En el archivo de configuración */etc/wpa_supplicant/wpa_supplicant.conf**

		`sudo nano /etc/wpa_supplicant/wpa_supplicant.conf`

		Y se agrego la siguiente informacion en el archivo:

		`network={
			ssid="Tenda_80EDFA"
			key_mgmt=NONE
			}`
		Como se muestra en la siguiente imagen:

		![archivo de configuración wpa_supplicant.conf](/imgs/wpa_supplicant.png)

		Todo esto se hace conectado a la Raspberry PI 3 B+ mediante teclado, mouse y un monitor con
		entrada HDMI.

2. Conexión mediante SSH

	El protocolo SSH permite la conexión y el intecambio de datos entre dos equipos de manera segura.

	2.1 *Comandos ejecutados **

		`sudo systemctl list-unit-files``

		![Salida del comando anteriormente citado](/imgs/lista_servicios_ssh.png)

		`sudo systemctl status ssh``

		![Salida del comando anteriormente citado](/imgs/status_ssh.png)

		`sudo systemctl start ssh``

		![Salida del comando anteriormente citado](/imgs/start_ssh.png)

		`sudo systemctl enable ssh``

		![Salida del comando anteriormente citado](/imgs/enable_ssh.png)

		`hostname -I``

		![Salida del comando anteriormente citado](/imgs/ip.png)

		Desde otra computadora con GNU/Linux, ejecutamos el  siguiente comando:

		`ssh pi@192.168.1.75``

		![Salida del comando anteriormente citado](/imgs/conexion_ssh.png)

3. VNC

	Permite tomar el control de una computadora servidor de forma remota a través de una computadora cliente.

	2.1 *Comandos ejecutados **

		`sudo systemctl list-unit-files``

		![Salida del comando anteriormente citado](/imgs/lista_servicios_vnc.png)

		`sudo systemctl status vnc``

		![Salida del comando anteriormente citado](/imgs/status_vnc.png)

		`sudo systemctl start vnc``

		![Salida del comando anteriormente citado](/imgs/start_vnc.png)

		`sudo systemctl enable vnc``

		![Salida del comando anteriormente citado](/imgs/enable_vnc.png)

		Desde otra computadora con GNU/Linux, ejecutamos el  siguiente comando:

		`yay -S realvnc-vnc-viewer``

		![Salida del comando anteriormente citado](/imgs/install_vnc_client.png)

		`vncviewer``

		![Salida del comando anteriormente citado](/imgs/client_vnc_0.png)

		![Ventana principal](/imgs/client_vnc_1.png)

		![Ingreso de datos para la conexión](/imgs/client_vnc_2.png)

		![Interfaz gráfica de nuestra Raspberry a través de VNC](/imgs/client_vnc_3.png)

4. Conexión mediante consola (UART)

		Lo único que necesito es una computadora personal, módulo de comunicación FT232 (este
		módulo permite hacer la conversión de señales RS232 a USB) y la Raspberry Pi 3 B+. Esto
		permite usar un conector USB de nuestra computadora personal para comunicarnos con la
		tarjeta Raspberry directamente por consola sin necesidad de mouse, teclado o monitor.

	4.1 *Comandos ejecutados **

		`sudo nano /boot/config.txt``

		![Salida del comando anteriormente citado, computadora personal](/imgs/uart_0.png)

		![Conexión física a la tarjeta ](/imgs/uart_1.jpg)

		`ls -l /dev/ttyUSB0``

		![Salida del comando anteriormente citado](/imgs/uart_2.png)

		`sudo usermod -a -G uucp alan``

		![Salida del comando anteriormente citado, computadora personal](/imgs/uart_3.png)

		`picocom -b 115200 /dev/ttyUSB0``

		![Salida del comando anteriormente citado, computadora personal](/imgs/uart_4.png)
