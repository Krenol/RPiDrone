import socket
import json
import time
from visualizer import *
import os

FILE = "pid.csv"

with open('../conf/config.json') as json_file:
    data = json.load(json_file)

cntls = data['controls']["escs"]["controllers"]
pltName = "kp_" + str(cntls['k_p']) + "-kd_" + str(cntls['k_d']) + "-ki_" + str(cntls['k_i'])

if os.path.exists(FILE):
    os.remove(FILE)
f = open(FILE, "x")
f.close()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("raspberrypi", 8889))

data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":0,"offset":0,"throttle":0}}
data_str = json.dumps(data)
s.send(data_str.encode())
time.sleep(5)

# for i in range(1, 10):
#     data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":20,"offset": 0.1 * i,"throttle":0}}
#     data_str = json.dumps(data)
#     s.send(data_str.encode())
#     time.sleep(1)

# for i in range(1, 100):
#     data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees": 1,"offset": 1,"throttle":0}}
#     data_str = json.dumps(data)
#     s.send(data_str.encode())
#     time.sleep(2)

plotPID(FILE, 'plots/' + pltName)

s.close()