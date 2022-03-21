import serial
import time
import math
from tkinter import *
import sys
import csv
#----------------------------INICIALIZACION DE LA COMUNICACION UART------------------------------
#Se inicializa la comunicacion UART con la Tiva C a un Baudrate=115200
serialTiva = serial.Serial(port = 'com3', baudrate = 115200,
                    parity = serial.PARITY_NONE,
                    stopbits = serial.STOPBITS_ONE,
                    bytesize = serial.EIGHTBITS, timeout = 1)
#Se lee inicialmente la tiva para verificar que funciona correctamente
s = serialTiva.read()

#-----------------------------FUNCION PARA COMUNICACION UART-----------------------------------

#definicion de funcion para realizar lectura de la parte del valor entero y decimal
#para luego concatenarlas y asignar un signo al valor retornando asi el valor leido de la Tiva C
def lectura_mensaje():
    signo = serialTiva.read().decode('utf-8') #se lee un valor inicial enviado desde la Tiva C
                                              #Para asignar el signo del valor
    
    entero = serialTiva.readline().decode('utf-8')#Se lee la parte entera del mensaje el valor en bruto recibido
                                                  #posee una estructura que no se puede operar de forma sencilla
                                                  #por ejemplo b'3\r', debido a \r este no puede concatenarse
                                                  #ni operarse
    
    decimal = datos = serialTiva.readline().decode('utf-8')#Se lee la parte decimal del mensaje
    
    datos = float(str(int(entero)) + '.' + str(int(decimal)))#se convierten los valores a "int"(integers) siguiendo
                                                             #el ejemplo anteriro si se hace int(b'3\r') este se
                                                             #vuelve 3 y luegos se debe volver un "str"(strings)
                                                             #para tener la forma '3' y este se pueda concartenar
    
    if (int(signo) == 2):#si se recive un 2 se coloca un signo negativo
       mensaje = -datos;
    else:
        mensaje = datos;#Si no se recive 2 se deja positivo
    
    return mensaje#Se retorna el valor del mensaje

#*************************************FUNCIONES DE BOTONES DE LA GUI************************************
#------------------------------------FUNCION PARA CREAR ACRCHIVO CSV-------------------------------------
def guardar():
    filename = "Datos de la imu.csv"#se define el nombre del archivo 
    with open(filename, 'w') as csvfile:#se abre el archivo, la constante 'w' puede variar('r','a','w' y 'r+')
                                        #pero en este caso se desea realizar un nuevo archivo
        
        #Se crea el objeto a llamar para modificar el documento
        csvwriter = csv.writer(csvfile)  
          
        #Se escriben los titulos de los espacios  
        csvwriter.writerow(titulos_para_csv)  
          
        #se escriben las lineas almacenadas de las variables leida del UART  
        csvwriter.writerows(data_para_csv)
        
#---------------------FUNCION PARA ENVIAR VALOR DE REF----------------------------------   
def enviar():
    num = Numero.get()#se obtine el string que se coloco en el espacio de escritura
    valor=float(num)#se guarda el valor del string como float en otra variable
    if(not(float(num)>=(-90) and float(num)<=90)):#se verifica si el valor esta dentro del rango permitido 
        Text.set('ingrese valor de -90 a 90')#si no se esta dentro de los rango permitidos se despliega un mensaje
        label.config(fg='red')
    else:
        Text.set('enviado')#si el valor esta dentro de los valores permitidos se despliega el mensaje de enviado 
        label.config(fg='green')
        serialTiva.write('2'.encode('utf-8'))#se escribe un dos al MIC para indicar que se realizara una escritura
        if valor < 0:#se verifica si el valor es negativo o no
            send = round((-valor)*(255/90),4)#se mapea entre valores de 0 a 255 y se redonde a 4 cifras significativas
            serialTiva.write((('1'+str(send)).encode('utf-8')))#el primer caracter enviado indica si es negativo o positivo el valor
                                                               #si este es 1 es negativo y si es 2 es positivo por ultimo se concatena con el mensaje
        else:
            send = round((valor)*(255/90),4)#se mapea entre valores de 0 a 255 y se redonde a 4 cifras significativas
            serialTiva.write((('2'+str(send)).encode('utf-8')))#el primer caracter enviado indica si es negativo o positivo el valor
                                                               #si este es 1 es negativo y si es 2 es positivo por ultimo se concatena con el mensaje
        serialTiva.flushInput()
        serialTiva.flushOutput()

        
#inicializacion de variables a recibir
imu_x=0.00
gx=0.00
gy=0.00
gz=0.00
ax=0.00
ay=0.00
az=0.00


titulos_para_csv= ['Inclinación', 'Gyro x', 'Gyro y', 'Gyro z', 'Accel x', 'Accel y', 'Accel z'] #se colocan los titulos para el csv
data_para_csv = []#se iniciali un array vacio para los datos a concatenar
nueva_fila_para_csv = []#se iniciali un array vacio para guardar los nuevos datos

gui = Tk()#se crea el objeto para realizar la GUI
Text = StringVar()#variable para modificar texto a desplegar



gui.title("Micro proyecto estabilizador")#titulo de la gui

nombre = Label(gui, text ="Carlos Avendaño \n17192", fg = 'white', bg = 'blue')#Label de nombre y carne llamado "nombre"
nombre.grid (row = 1, column = 2)#posicion de nombre
#se deja un espacio de fila para ordenar mejor la GUI
Espacio1 = Label (gui, text ='')
Espacio1.grid (row = 2, column = 2)

#-------------------------------------------VALOR DE INCLINACION-----------------------------------------
Inclinacion = Label(gui, text = 'Inclinación del balancin\n(grados°)', fg='white', bg='green')#Label de Inclinación del balancin "Inclinacion"
Inclinacion.grid (row = 3, column = 2)#posicion de Inclinacion
#despliego del valor de inclinacion actual 
Grados = Label(gui,text=str(imu_x) ,fg="white", bg = 'orange',width=10)#valor actual de inclinacion
Grados.grid(row=4,column = 2)
#se deja un espacio de fila para ordenar mejor la GUI
Espacio2 = Label (gui, text ='')
Espacio2.grid (row = 5, column = 2)

#----------------------------------------VALORES LEIDOS DE LA MPU6050-----------------------------------
Raw = Label(gui, text = 'Valores leidos\ndirectamente de MPU6050', fg='white', bg='green')#Label para indicar datos Raw 
Raw.grid (row = 6, column = 2)#posicion
#se deja un espacio de fila para ordenar mejor la GUI
Espacio4 = Label (gui, text ='')
Espacio4.grid (row = 7, column = 2)

titulo_gx = Label(gui, text = 'Gyro x (rad/s)', fg='white', bg='green', width=12)#Label de titulo gx
titulo_gx.grid (row = 8, column = 1)#posicion
Gyrox = Label(gui, text = str(gx), fg='white', bg='green', width=6)#valor gx
Gyrox.grid (row = 9, column = 1)#posicion

titulo_gy = Label(gui, text = 'Gyro y (rad/s)', fg='white', bg='green', width=12)#Label de titulo gy
titulo_gy.grid (row = 8, column = 2)#posicion
Gyroy = Label(gui, text = str(gy), fg='white', bg='green', width=6)#Label de titulo gy
Gyroy.grid (row = 9, column = 2)#posicion

titulo_gz = Label(gui, text = 'Gyro z (rad/s)', fg='white', bg='green', width=12)#Label de titulo gz
titulo_gz.grid (row = 8, column = 3)#posicion
Gyroz = Label(gui, text = str(gz), fg='white', bg='green', width=6)#valor gz
Gyroz.grid (row = 9, column = 3)#posicion
#se deja un espacio de fila para ordenar mejor la GUI
Espacio5 = Label (gui, text ='')
Espacio5.grid (row = 10, column = 2)

titulo_ax = Label(gui, text = 'Accel x (m/s^2)', fg='white', bg='green', width=12)#Label de titulo ax
titulo_ax.grid (row = 11, column = 1)#posicion
Accelx = Label(gui, text = str(ax), fg='white', bg='green', width=6)#valor ax
Accelx.grid (row = 12, column = 1)#posicion

titulo_ay = Label(gui, text = 'Accel y (m/s^2)', fg='white', bg='green', width=12)#Label de titulo ay
titulo_ay.grid (row = 11, column = 2)#posicion
Accely = Label(gui, text = str(ay), fg='white', bg='green', width=6)#Label de titulo ay
Accely.grid (row = 12, column = 2)#posicion

titulo_az = Label(gui, text = 'Accel z (m/s^2)', fg='white', bg='green', width=12)#Label de titulo az
titulo_az.grid (row = 11, column = 3)#posicion
Accelz = Label(gui, text = str(az), fg='white', bg='green', width=6)#valor az
Accelz.grid (row = 12, column = 3)#posicion
#se deja un espacio de fila para ordenar mejor la GUI
Espacio6 = Label (gui, text ='')
Espacio6.grid (row = 13, column = 2)

#---------------------------------ESPECIFICACION DE CONSTANTE DE REFERENCIA--------------------------
ref = Label(gui, text = 'REFERENCIA', fg='white', bg='green', width=12)#Label de titulo REF
ref.grid (row = 14, column = 2)#posicion

Numero = StringVar(value='')#espera un valor de entrada para volvero un string
caja = Entry(gui, textvariable=Numero)#se lee el numero ingresado
caja.grid(row = 15, column=1)#posicion
label = Label(gui, textvariable=Text)#se coloca el label que inicara el mensaje de error
label.grid(row = 16, column = 1)


#boton
boton1 = Button(gui, text='enviar', fg = 'white', bg='blue', command=enviar)#se coloca el texto, color y se conecta la funcion creada para el primer boton
boton1.grid (row = 18 , column = 2)
boton1 = Button(gui, text='Guardar csv', fg = 'white', bg='blue', command=guardar)#se coloca el texto, color y se conecta la funcion creada para el segundo boton
boton1.grid (row = 18 , column = 3)


while True:
    serialTiva.write(('1'.encode('utf-8')))#se envia un mensaje para indicar que se esta listo para recibir datos
    n = serialTiva.read()#se espera el mensaje de confirmacion
    
    if (n==b'1'):#si se recibe un 1 de confirmacion indica que se reciben datos
        #se reciben todos los datos que se vayan a utilizar
        imu_x = lectura_mensaje()
        gx = lectura_mensaje()
        gy = lectura_mensaje()
        gz = lectura_mensaje()
        ax = lectura_mensaje()
        ay = lectura_mensaje()
        az = lectura_mensaje()
        gui.update()#se actualiza la gui luego de recivir datos nuevos
        
        nueva_fila_para_csv = [str(imu_x),str(gx),str(gy),str(gz),str(ax),str(ay),str(az)]#se almacenan los nuevos valores de lectura para el csv
        data_para_csv.append(nueva_fila_para_csv)#se añade la nueva linea para la informacion del csv
        
#se configuran los valores en la lectura del UART para ser desplegados en la gui
    Grados.config(text = str(imu_x)[0:7])
    Gyrox.config(text = str(gx)[0:7])
    Gyroy.config(text = str(gy)[0:7])
    Gyroz.config(text = str(gz)[0:7])
    Accelx.config(text = str(ax)[0:7])
    Accely.config(text = str(ay)[0:7])
    Accelz.config(text = str(az)[0:7])

    time.sleep(0.05)
    
        
        

