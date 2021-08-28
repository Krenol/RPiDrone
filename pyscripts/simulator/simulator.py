import socket
import time
import json
import os
import thread

HOSTNAME = "localhost"
PORT = 8889

DELIM = "\r\n"

def send(s: socket, deg: float = 0, offset: float = 0, rotation: float = 0, throttle: int = 0):
    data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees": deg,"offset": offset, "rotation": rotation},"throttle": throttle}
    data_str = json.dumps(data) + DELIM
    print(data_str)
    s.send(data_str.encode())

def read(t: tuple):
    s = t[0]
    try:
        data = s.recv(1024)
        if not data:
            time.sleep(1)
        j = json.loads(data)
        print(j)
    except Exception:
        time.sleep(1)

def run():
    while True:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(1)
        while True:
            try:
                s.connect((HOSTNAME, PORT))
                break
            except KeyboardInterrupt:
                return
            except Exception:
                print("failed to connect")
            time.sleep(1)
        thr = thread.MyThread(read, s)
        thr.start()
        throttle = 0
        send(s)
        time.sleep(5)

        # for i in range(2, 8):
        #     throttle = i * 10
        #     send(s, throttle = throttle)
        #     time.sleep(3)

        # send(s, throttle = throttle, rotation = 0.7)
        # time.sleep(5)

        # send(s, throttle = throttle, deg = 30, offset = 0.7)
        # time.sleep(5)

        # send(s, throttle = throttle, deg = 100, offset = 0.8)
        # time.sleep(5)
        thr.stop()
        thr.join()
        s.close()
        time.sleep(1)


if __name__ == "__main__":
    run()
