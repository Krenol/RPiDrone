import socket
import json
import time
from visualizer import *
import os

FILE = "/home/krenol/git/RPiDrone/logs/pid.csv"

with open('../conf/config.json') as json_file:
    data = json.load(json_file)

delim = data["server"]["delimiter"]
cntls = data['controls']["escs"]["controllers"]
pltName = "kp_" + str(cntls['k_p']) + "-kd_" + str(cntls['k_d']) + "-ki_" + str(cntls['k_i']) + "-kaw_" + str(cntls['k_aw'])

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("raspberrypi", 8889))

while True:
    data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":0,"offset":0, "rotation": 0.0},"throttle":0}
    data_str = json.dumps(data) + delim
    print(data_str)
    s.send(data_str.encode())
    time.sleep(30)

    for i in range(1, 10):
        data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":0,"offset": 0.1 * i, "rotation": 0},"throttle":0}
        data_str = json.dumps(data) + delim
        print(data_str)
        s.send(data_str.encode())
        time.sleep(1)

    for i in range(1, 10):
        data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":0,"offset": 0, "rotation": 0.1 * i},"throttle":0}
        data_str = json.dumps(data) + delim
        print(data_str)
        s.send(data_str.encode())
        time.sleep(1)

    for i in range(1, 180):
        data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees": i,"offset": 1, "rotation": 0 },"throttle":0}
        data_str = json.dumps(data) + delim
        print(data_str)
        s.send(data_str.encode())
        time.sleep(1)

s.close()
time.sleep(2)
plotPID(FILE, 'plots/' + pltName)