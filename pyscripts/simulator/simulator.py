from websocket import create_connection
import time
import json
import os
import thread

HOSTNAME = "localhost"
PORT = 8889
CONTEXT = "/"
UPDATE_JSON = {"controls":{"escs":{"controllers":{"k_i":{"roll_rate":0,"pitch_rate":0,"roll_output":0,"pitch_output":0,"yaw_output":0},"k_p":{"roll_rate":1,"pitch_rate":1,"roll_output":1,"pitch_output":1,"yaw_output":1},"k_d":{"roll_rate":0,"pitch_rate":0,"roll_output":0,"pitch_output":0,"yaw_output":0},"k_aw":{"roll_rate":0,"pitch_rate":0,"roll_output":0,"pitch_output":0,"yaw_output":0},"max_diff":{"roll_rate":8,"pitch_rate":8,"roll_output":50,"pitch_output":50,"yaw_output":25}}}}}
BEST_JSON = {"controls":{"escs":{"controllers":{"k_i":{"roll_rate":0,"pitch_rate":0,"roll_output":0,"pitch_output":0,"yaw_output":0},"k_p":{"roll_rate":1,"pitch_rate":1,"roll_output":1,"pitch_output":1,"yaw_output":1},"k_d":{"roll_rate":0,"pitch_rate":0,"roll_output":0,"pitch_output":0,"yaw_output":0},"k_aw":{"roll_rate":0,"pitch_rate":0,"roll_output":0,"pitch_output":0,"yaw_output":0},"max_diff":{"roll_rate":8,"pitch_rate":8,"roll_output":50,"pitch_output":50,"yaw_output":25}}}}}
DATA = []


def send(ws, deg: float = 0, offset: float = 0, rotation: float = 0, throttle: int = 0):
    data = {"gps":{"altitude":0,"latitude":0,"longitude":0},"joystick":{"degrees": deg,"offset": offset, "rotation": rotation},"throttle": throttle}
    data_str = json.dumps(data)
    print(data_str)
    ws.send(data_str.encode())

def read(t: tuple):
    ws = t[0]
    try:
        data = ws.recv()
        if not data:
            time.sleep(1)
        j = json.loads(data)
        print(j)
        DATA.append(j)
    except Exception:
        time.sleep(1)

def run():
    while True:

        while True:
            try:
                ws = create_connection("ws://" + HOSTNAME + ":" + PORT + CONTEXT)
                DATA.clear()
                break
            except KeyboardInterrupt:
                return
            except Exception:
                print("failed to connect")
            time.sleep(1)
        thr = thread.MyThread(read, ws)
        thr.start()
        throttle = 0
        send(ws)
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
        ws.close()
        time.sleep(1)


if __name__ == "__main__":
    run()
