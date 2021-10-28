#! /usr/bin/python3

import time
import serial
import pymysql

ser = serial.Serial(
 port='/dev/tty.usbmodem146301',
 baudrate = 9600,
 parity=serial.PARITY_NONE,
 stopbits=serial.STOPBITS_ONE,
 bytesize=serial.EIGHTBITS,
 timeout=1
)

while 1:
    data = ser.readline()
    data = data.decode('utf-8')
    length = len(data)
    
    if length != 0:
        pieces = data.split('\t')
        userID = pieces[0]
        liters = pieces[1]
        
        connection = pymysql.connect(host='localhost',
                             user='rcurcio',
                             password='12345678',
                             database='ryanDev',
                             port=8889,
                             cursorclass=pymysql.cursors.DictCursor)
        
        
        with connection:
            with connection.cursor() as cursor:
                # Create a new record
                sql = "INSERT INTO `main` (`userID`, `liters`) VALUES (%s, %s)"
                cursor.execute(sql, (userID, liters))
                connection.commit()
                print ("Success!")
                cursor.close()
        
        
