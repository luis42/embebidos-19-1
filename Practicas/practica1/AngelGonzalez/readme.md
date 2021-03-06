#			**INSTITUTO POLITECNICO NACIONAL.**</h1>

# 			  ESCUELA SUPERIOR DE CÓMPUTO.<h2>


#		 Título: Uso de las RaspberryPi 3.

				Alumnos: 
				          Angel Miranda Pedro.
				          Gonzale Ambriz Luis Angel.

				Materia:  Embedded Systems.
		
				Grupo:    4CM3.
		
				Profesor: García Ortega Víctor Hugo.




Raspbian
Como podemos observar ya se tiene intalada en la raspberry un sistema como linux.


![GitHub Logo](imagenes/rasp.png)





SSH

Para verificar la  lista  de  servicios  disponible  y  su  estado ejecutamos los comandos siguientes podrás  observar  que  el  servicio  esta  activo  por  lo  que  ya se inicializo  previamente pero ahora con capturas.

Aquí se podemos observar como tenemos el servicio ya activo y funcionando correctamente.


```sh

$sudo  systemctl  status  ssh

```

![GitHub Logo](imagenes/2018-10-08-140329_640x480_scrot.png)


Con este comando podemos encender el servicio


```sh

$sudo  systemctl  start  ssh

```


![GitHub Logo](imagenes/2018-10-08-140418_640x480_scrot.png)


```sh

$sudo  systemctl  enable  ssh

```


![GitHub Logo](imagenes/2018-10-08-140443_640x480_scrot.png)



Ahora hay que saber que dirección ip es la que tenemos con el siguiente comando podremos saberla


```sh

$ifconfig

```



![GitHub Logo](imagenes/2018-10-15-092658_640x480_scrot.png)




Determinar  la  dirección  ipasignada  por  el  router  WIFI  a  la  tarjeta  RaspberryPi  3.

Finalmente  ejecutar  la  conexión  con  el  comando  ssh.  Si  la  dirección  IP  asignada  a  nuestratarjeta  RaspberryPi  3  fuera 192.168.100.32 tendríamos:


```sh

$ssh pi@192.168.100.32

```

![GitHub Logo](imagenes/ssh.png)


Usando la  RaspberryPi  3  mediante  VNC.


```sh

$sudo  systemctl  status  vncserver-x11-serviced

```

![GitHub Logo](imagenes/2018-10-15-093511_640x480_scrot.png)



```sh

$sudo  systemctl  start  vncserver-x11-serviced

```


![GitHub Logo](imagenes/2018-10-15-093615_640x480_scrot.png)



```sh

$sudo  systemctl  enable  vncserver-x11-serviced

```


![GitHub Logo](imagenes/2018-10-15-093615_640x480_scrot.png)
 


```sh

Instalamos vncserver en una Ipad

```

Como podemos observar esta instalada en la Ipad

![GitHub Logo](imagenes/vnc.jpg)


Ahora la ejecutamos y necesitamos poner la direccion ip antes ya obtenida.

![GitHub Logo](imagenes/vnc2.jpg)


Llenamos los campos.

![GitHub Logo](imagenes/vnc3.jpg)


Ahora ya solo es conectarnos y nos pedira la contraseña que pusimos al instalar

![GitHub Logo](imagenes/vnc4.jpg)

Y ahora ya solo es conectarnos


![GitHub Logo](imagenes/vnc5.jpg)


Como podemos observar ya estamos conectados a la raspberry


![GitHub Logo](imagenes/rasp.png)


 Usando  la  RaspberryPi  3  mediante  consola con un fts232.

```sh

$sudo  nano /boot/config.txt

```


![GitHub Logo](imagenes/2018-10-15-094027_640x480_scrot.png)



Agregamos  al  final  las  sentencias  de  habilitación  del  mini-UART  y  la  configuración  de  suoscilador  de  250  Mhz  para  el  cálculo  de  la  velocidad  de  transmisión  en  baudios.

```sh

enable_uart=1
core_freq=250

```


![GitHub Logo](imagenes/2018-10-15-094036_640x480_scrot.png)



En  nuestra  computadora  personal  conectamos  el  módulo  FT232  y  verificamos  el  nombre  dedispositivo  asignado,  normalmente  es  ttyUSB0.


```sh

$ls  -l  /dev/ttyUSB0

```

Como podemos observar ya tenemos el ttyUSB0 habilitado


![GitHub Logo](imagenes/fts232 (1).png)


![GitHub Logo](imagenes/fts2321.png)


Verás  algo  como  esto:  “crw-rw----T  1  root  dialout  ...”,  la  letra  c  significa  que  es  un  dispositivode  carácter,  root  puede  leer  y  escribir,  el  grupo  dialout  puede  leer  y  escribir,  todos  los  demásno  tienen  acceso.  Debemos  ser  parte  del  grupo  dialout  para  poder  comunicarnos  con  elmódulo  FT232.

Si  no  aparece  el  grupo  dialout  en  la  lista  debemos  agregarnos  al  grupo  con:


$  sudo  usermod  -a  -G  dialout  “$(whoami)”



![GitHub Logo](imagenes/fts2322.png)


Entonces  podemos  conectar  nuestra  computadora  personal  con  GNU  screen.  Screen  es  unprograma  que  permite  administrar  ventanas  multiplexando  una  terminal  física  entre  variosprocesos.  Con  screen  podemos  realizar  la  conexión  a  nuestra  tarjeta  Raspberry  usando  lacomunicación  UART.  La  conexión  la  realizamos  con  el  comando  screen  epecificando  eldispositivo  al  que  nos  vamos  a  conectar  y  la  velocidad  de  comunicación:


```sh

$screen  /dev/ttyUSB0  11520

```



En  este  instante  tenemos  que  reniciar  nuestra  tarjeta  RaspberryPi  3.  
```sh

$sudo  reboot

```


Como podemos observar ya estamos en la terminal de la raspberry

![GitHub Logo](imagenes/ft232op.png)

Y podemos visualizar el contenido.

![GitHub Logo](imagenes/fts23210.png.png)





## Autores ✒️

* **Angel Miranda Pedro.** - *Reporte* - [peterwtf](https://github.com/peterwtf)
* **Gonzale Ambriz Luis Angel.** - *Reporte* - [luis42](https://github.com/luis42)

## Victor Hugo García Ortega 

* **Profesor** [Victor Hugo García Ortega](https://github.com/vgarciaortega)


