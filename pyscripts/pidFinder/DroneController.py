import constants
import time
import json
from websocket import create_connection
import DroneInput


class DroneController:
    def __init__(self):
        self.throttle = 0

    def connectToDrone(self):
        droneHostname = self.getDroneHostname()
        print("connecting to: " + droneHostname)
        while True:
            try:
                self.websocket = create_connection(droneHostname)
                return
            except KeyboardInterrupt:
                raise KeyboardInterrupt()
            except Exception:
                print("failed to connect")
            time.sleep(1)

    def getDroneHostname(self):
        return constants.DRONE_WS_PROTOCOL + "://" + constants.DRONE_HOSTNAME + ":" + str(constants.DRONE_WS_PORT) + constants.DRONE_WS_CONTEXT

    def disconnectFromDrone(self):
        self.websocket.close()

    def readFromDrone(self) -> str:
        try:
            data = self.websocket.recv()
            if data:
                return data
        except Exception:
            print("error while parsing json")

    def sendInputToDrone(self, droneInput: DroneInput):
        data = self.createDroneControlJson(droneInput)
        data_str = json.dumps(data)
        self.websocket.send(data_str.encode())

    def createDroneControlJson(self, droneInput: DroneInput):
        data = {"gps": {"altitude": 0, "latitude": 0, "longitude": 0}, "joystick": {"degrees": droneInput.joystickDegrees,
                                                                                    "offset": droneInput.joystickOffset, "rotation": droneInput.joystickRotation}, "throttle": droneInput.rotation}
        return data

    
