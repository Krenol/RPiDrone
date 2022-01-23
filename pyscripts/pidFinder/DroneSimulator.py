import time
import json
from websocket import create_connection
import thread
from DroneInput import DroneInput
import DroneController
import math 
import constants

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
        for i in range(3, 6):
            self.throttle = i * 10
            droneInput = DroneInput(throttle=self.throttle)
            self.droneController.sendInputToDrone(droneInput)
            time.sleep(self.sleepTime)

    def idleDrone(self):
        droneInput = DroneInput()
        self.droneController.sendInputToDrone(droneInput)
        time.sleep(self.sleepTime)

    def rotateDroneRight(self):
        droneInput = DroneInput(joystickRotation=0.7, throttle=self.throttle)
        self.droneController.sendInputToDrone(droneInput)
        time.sleep(self.sleepTime)
    
    def rotateDroneLeft(self):
        droneInput = DroneInput(joystickRotation=-0.7, throttle=self.throttle)
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
        
    def evaluateEmergencyStop(self, data):
        yaw = abs((data['angles']['is']['yaw'] - data['angles']['should']['yaw']) * 180 / math.pi)
        pitch = abs((data['angles']['is']['pitch'] - data['angles']['should']['pitch']) * 180 / math.pi)
        roll = abs((data['angles']['is']['roll'] - data['angles']['should']['roll']) * 180 / math.pi)
        if(yaw > constants.MAX_ANGLE_BEFORE_EMERGENCY_OFF or pitch > constants.MAX_ANGLE_BEFORE_EMERGENCY_OFF or roll > constants.MAX_ANGLE_BEFORE_EMERGENCY_OFF):
            self.idleDrone()
            raise Exception("Maximum allowed offset angle reached")
        
    def storeDroneData(self, args):
        data = self.droneController.readFromDrone()
        j = json.loads(data)
        self.evaluateEmergencyStop(data)
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