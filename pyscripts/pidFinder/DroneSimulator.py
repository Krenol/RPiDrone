import time
import json
from websocket import create_connection
import thread
import DroneInput
import DroneController

class DroneSimulator:
    def __init__(self, droneController: DroneController, sleepTime: int = 5):
        self.droneController = droneController
        self.sleepTime = sleepTime
        self.droneResponses = []
        
    def getDroneResponses(self):
        return self.droneResponses

    def clearDroneResponses(self):
        self.droneResponses.clear()
        
    def liftOffDrone(self):
        for i in range(2, 8):
            self.throttle = i * 10
            droneInput = DroneInput(throttle=self.throttle)
            self.droneController.sendInputToDrone(droneInput)
            time.sleep(self.sleepTime)

    def idleDrone(self):
        self.droneController.sendInputToDrone(DroneInput())
        time.sleep(self.sleepTime)

    def rotateDroneRight(self):
        droneInput = DroneInput(rotation=0.7, throttle=self.throttle)
        self.droneController.sendInputToDrone(droneInput)
        time.sleep(self.sleepTime)
    
    def rotateDroneLeft(self):
        droneInput = DroneInput(rotation=-0.7, throttle=self.throttle)
        self.droneController.sendInputToDrone(droneInput)
        time.sleep(self.sleepTime)

    def flyDroneRight(self):
        droneInput = DroneInput(
            joystickOffset=0.8, joystickDegress=90, throttle=self.throttle)
        self.droneController.sendInputToDrone(droneInput)
        time.sleep(self.sleepTime)

    def flyDroneLeft(self):
        droneInput = DroneInput(
            joystickOffset=0.8, joystickDegress=270, throttle=self.throttle)
        self.droneController.sendInputToDrone(droneInput)
        time.sleep(self.sleepTime)

    def hoverDrone(self):
        droneInput = DroneInput(throttle=self.throttle)
        self.droneController.sendInputToDrone(droneInput)
        time.sleep(self.sleepTime)
        
    def startReadThread(self):
        self.readThread = thread.MyThread(self.storeDroneData)
        self.readThread.start()

    def stopReadThread(self):
        self.readThread.stop()
        self.readThread.join()
        
    def storeDroneData(self):
        data = self.droneController.readFromDrone()
        j = json.loads(data)
        self.droneResponses.append(j)
    
    def fullSimulationRun(self):
        self.clearDroneResponses()
        self.droneController.connectToDrone()
        self.startReadThread()
        self.idleDrone()
        self.liftOffDrone()
        self.hoverDrone()
        self.rotateDroneRight()
        self.rotateDroneLeft()
        self.hoverDrone()
        self.flyDroneRight()
        self.hoverDrone()
        self.flyDroneLeft()
        self.hoverDrone()
        self.stopReadThread()
        self.droneController.disconnectFromDrone()
        
    def hoverSimulationRun(self):
        self.clearDroneResponses()
        self.droneController.connectToDrone()
        self.startReadThread()
        self.idleDrone()
        self.liftOffDrone()
        self.hoverDrone()
        self.stopReadThread()
        self.droneController.disconnectFromDrone()