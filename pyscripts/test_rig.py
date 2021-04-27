import socket
import json
import time
import os

def send(s: socket, data: dict, delim: str):
    data_str = json.dumps(data) + delim
    s.send(data_str.encode())

def run():
    with open('../conf/config.json') as json_file:
        data = json.load(json_file)

    delim = data["server"]["delimiter"]

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    while True:
        try:
            s.connect(("192.168.178.61", 8889))
            break
        except KeyboardInterrupt:
            return
        except Exception:
            print("failed to connect")
        time.sleep(1)

    print("connected to RPi")
    read_str = ""
    data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":0,"offset":0, "rotation": 0.0},"throttle": 0}
    send(s, data, delim)
    throttle = 0
    while read_str != "e":
        data_str = json.dumps(data) + delim
        s.send(data_str.encode())
        print("Current throttle value: " + str(throttle) + "\ninput new throttle value:")
        read_str = input()
        print("received string " + read_str)
        if read_str.isdigit():
            throttle = int(read_str)
        data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":0,"offset":0, "rotation": 0.0},"throttle": throttle}
        send(s, data, delim)

    data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees":0,"offset":0, "rotation": 0.0},"throttle": 0}
    send(s, data, delim)
    s.close()

if __name__ == "__main__":
    run()