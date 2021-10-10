import constants
import time
import json
from websocket import create_connection
import thread
import DroneInput

class DroneSimulator():
    def __init__(self):
        self.throttle = 0
        self.droneResponses = []
        self.sleepTime = 5
    
    def getDroneResponses(self):
        return self.droneResponses
    
    def clearDroneResponses(self):
        self.droneResponses.clear()
    
    def simulationRun(self):
        self.clearDroneResponses()
        self.connectDrone()
        self.startReadThread()
        self.idleDrone()
        self.liftOffDrone()
        self.hoverDrone()
        self.rotateDrone()
        self.hoverDrone()
        self.flyDroneRight()
        self.hoverDrone()
        self.flyDroneLeft()
        self.hoverDrone()
        self.stopReadThread()
        self.disconnectDrone()
        
    def connectDrone(self):
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
    
    def disconnectDrone(self):
        self.websocket.close()
            
    def startReadThread(self):
        self.readThread = thread.MyThread(readFromDrone, self.websocket)
        self.readThread.start()
        
    def stopReadThread(self):
        self.readThread.stop()
        self.readThread.join()
        
    def readFromDrone(self, t: tuple):
        websocket = t[0]
        try:
            data = websocket.recv()
            if data:
                storeReceivedDroneData(data)
        except Exception:
            print("error while parsing json")
    
    def storeReceivedDroneData(self, data: str): 
        j = json.loads(data)
        self.droneResponses.append(j)
            
    def sendToDrone(self, droneInput: DroneInput):
        data = createDroneControlJson(droneInput)
        data_str = json.dumps(data)
        self.websocket.send(data_str.encode())
        
    def createDroneControlJson(self, droneInput: DroneInput):
        data = {"gps": {"altitude": 0, "latitude": 0, "longitude": 0}, "joystick": {"degrees": droneInput.joystickDegrees,
                                                                                "offset": droneInput.joystickOffset, "rotation": droneInput.joystickRotation}, "throttle": droneInput.rotation}
    
    def liftOffDrone(self):
        for i in range(2, 8):
            self.throttle = i * 10
            droneInput = DroneInput(throttle = self.throttle)
            self.sendToDrone(droneInput)
            time.sleep(self.sleepTime)

    def idleDrone(self):
        self.sendToDrone(DroneInput())
        time.sleep(self.sleepTime)
        
    def rotateDrone(self):
        droneInput = DroneInput(rotation = 0.7, throttle = self.throttle)
        self.sendToDrone(droneInput)
        time.sleep(self.sleepTime) 
        
    def flyDroneRight(self):
        droneInput = DroneInput(joystickOffset = 0.8, joystickDegress = 90, throttle = self.throttle)
        self.sendToDrone( droneInput)
        time.sleep(self.sleepTime) 
        
    def flyDroneLeft(self):
        droneInput = DroneInput(joystickOffset = 0.8, joystickDegress = 270, throttle = self.throttle)
        self.sendToDrone( droneInput)
        time.sleep(self.sleepTime) 
        
    def hoverDrone(self):
        droneInput = DroneInput(throttle = self.throttle)
        self.sendToDrone( droneInput)
        time.sleep(self.sleepTime) 
        