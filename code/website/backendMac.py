#! /usr/bin/python3
import os
import sys
import time
from flask import Flask, request, jsonify, abort, jsonify, current_app
import json
import pymysql
 
app = Flask(__name__)


@app.route('/leaderboard_api', methods = ['GET'])
def leaderboard_api():
    connection = pymysql.connect(host='localhost',
                             user='rcurcio',
                             password='12345678',
                             database='ryanDev',
                             port=8889,
                             charset='utf8mb4',
                             cursorclass=pymysql.cursors.DictCursor)
    
    with connection:
            with connection.cursor() as cursor:
                # Create a new record
                sql = "SELECT * FROM leaderboard ORDER BY liters DESC, name DESC"
                cursor.execute(sql)
                result = cursor.fetchall()
                return jsonify({"leaderboard" : result})

@app.route('/leaders')
def leaders():
    return current_app.send_static_file('leaders.html')

@app.route('/test_api')
def test_api():
    current_time = time.localtime()
    return time.strftime('%a, %d %b %Y %H:%M:%S GMT', current_time)

if __name__ == '__main__':
    app.run() 
