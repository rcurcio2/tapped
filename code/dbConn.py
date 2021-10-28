#! /usr/bin/python3

import time
import serial
import pymysql


connection = pymysql.connect(host='localhost',
                             user='rcurcio',
                             password='12345678',
                             database='ryanDev',
                             port='8888',
                             cursorclass=pymysql.cursors.DictCursor)
        
        
with connection:
            with connection.cursor() as cursor:
                # Create a new record
                sql = "INSERT INTO `main` (`userID`, `liters`) VALUES (%s, %s)"
                cursor.execute(sql, (1, 0.01))
                connection.commit()
                print ("Success!")
                cursor.close()
        
        
