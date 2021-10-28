#! /usr/bin/python3
import os
import sys
import time
from flask import Flask, request, jsonify, abort, jsonify, current_app
import mysql.connector
from flaskext.mysql import MySQL
from mysql.connector import Error
import json
 
app = Flask(__name__)
 
mysql = MySQL()
app.config['MYSQL_DATABASE_USER'] = 'ryan'
app.config['MYSQL_DATABASE_PASSWORD'] = '1324'
app.config['MYSQL_DATABASE_DB'] = 'ryanDev'
app.config['MYSQL_DATABASE_HOST'] = 'localhost'

mysql.init_app(app)

@app.route('/leaderboard_api', methods = ['GET'])
def leaderboard_api():
    query = "SELECT * FROM leaderboard"
    conn = mysql.connect()
    cursor = conn.cursor()
    cursor.execute(query)
    result = cursor.fetchall()
    rows = [dict(zip([key[0] for key in cursor.description], row)) for row in result]
    return jsonify({"leaderboard" : rows})

@app.route('/leaders')
def leaders():
    return current_app.send_static_file('leaders.html')

@app.route('/test_api')
def test_api():
    current_time = time.localtime()
    return time.strftime('%a, %d %b %Y %H:%M:%S GMT', current_time)

if __name__ == '__main__':
    app.run() 
