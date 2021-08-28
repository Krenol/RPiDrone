import socket
import thread
from time import sleep
import json


HOSTNAME = "127.0.0.1"
PORT = 8889
DELIM = "\r\n"


def send(t: tuple):
    s = t[0]
    j = {
            "angles": {"yaw": 0, "pitch": 0, "roll": 0},
            "position": {"latitude": 0, "longitude": 0, "altitude": 0},
            "sensors": {"barometric_height": 0}}
    data_str = json.dumps(j) + DELIM
    print(data_str)
    s.sendall(data_str.encode())
    sleep(0.05)


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOSTNAME, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        thr = thread.MyThread(send, conn)
        thr.start()
        while True:
            data = conn.recv(1024)
            if not data:
                thr.stop()
                thr.join()
                break
            print(data)