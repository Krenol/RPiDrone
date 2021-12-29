from DroneSimulator import DroneSimulator
import json
import requests
import constants
from OffsetCalculator import OffsetCalculator
import random
from OffsetCalculator import OffsetCalculator

class PidFinder():
    def __init__(self):
        self.UPDATE_JSON = {"controls": {"escs": {"controllers": 
            {"k_i": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "k_p": {"roll_rate": 1, "pitch_rate": 1, "roll_output": 1, "pitch_output": 1, "yaw_output": 1}, 
            "k_d": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "k_aw": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "max_diff": {"roll_rate": 8, "pitch_rate": 8, "roll_output": 50, "pitch_output": 50, "yaw_output": 25}}}}}
        self.BEST_JSON = {"controls": {"escs": {"controllers": 
            {"k_i": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "k_p": {"roll_rate": 1, "pitch_rate": 1, "roll_output": 1, "pitch_output": 1, "yaw_output": 1}, 
            "k_d": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "k_aw": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "max_diff": {"roll_rate": 8, "pitch_rate": 8, "roll_output": 50, "pitch_output": 50, "yaw_output": 25}}}}}
        self.BEST_OFFSET = {'yaw': 5, 'pitch': 5, "roll": 5, 'total': 5}
        self.DECIMAL_PLACES = 3


    def update_config(self):
        resp = requests.patch("http://" + constants.DRONE_HOSTNAME +
                            ":" + str(constants.DRONE_API_PORT) + "/config", json=json.dumps(self.UPDATE_JSON))
        if resp.status_code != 200:
            raise Exception("Couldn't patch config")
        return resp.json()


    def createNewUpdateJson(self):       
        kp_pr_rate, kd_pr_rate, ki_pr_rate = self.getRateControlValuesForPR()
        kp_pr_out, kd_pr_out, ki_pr_out = self.getOutputControlValuesForPR()
        kp_yaw_out, kd_yaw_out, ki_yaw_out = self.getOutputControlValuesForYaw()
        self.UPDATE_JSON = {"controls": {"escs": {"controllers": 
            {"k_i": {"roll_rate":  ki_pr_rate, "pitch_rate": ki_pr_rate, "roll_output": ki_pr_out, "pitch_output": ki_pr_out, "yaw_output": ki_yaw_out}, 
            "k_p": {"roll_rate": kp_pr_rate, "pitch_rate": kp_pr_rate, "roll_output": kp_pr_out, "pitch_output": kp_pr_out, "yaw_output": kp_yaw_out}, 
            "k_d": {"roll_rate": kd_pr_rate, "pitch_rate": kd_pr_rate, "roll_output": kd_pr_out, "pitch_output": kd_pr_out, "yaw_output": kd_yaw_out}, 
            "k_aw": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "max_diff": {"roll_rate": 8, "pitch_rate": 8, "roll_output": 50, "pitch_output": 50, "yaw_output": 25}}}}}

    def getRateControlValuesForPR(self):
        kp = self.UPDATE_JSON['controls']['escs']['controllers']['k_p']['roll_rate']
        ki = self.UPDATE_JSON['controls']['escs']['controllers']['k_i']['roll_rate']
        kd = self.UPDATE_JSON['controls']['escs']['controllers']['k_d']['roll_rate']
        maxOffset = max(self.BEST_OFFSET["pitch"], self.BEST_OFFSET["roll"])
        kp = kp + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        ki = ki + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        kd = kd + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        
        return kp, kd, ki
    
    def getOutputControlValuesForPR(self):
        kp = self.UPDATE_JSON['controls']['escs']['controllers']['k_p']['roll_output']
        ki = self.UPDATE_JSON['controls']['escs']['controllers']['k_i']['roll_output']
        kd = self.UPDATE_JSON['controls']['escs']['controllers']['k_d']['roll_output']
        maxOffset = max(self.BEST_OFFSET["pitch"], self.BEST_OFFSET["roll"])
        kp = kp + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        ki = ki + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        kd = kd + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        
        return kp, kd, ki
    
    def getOutputControlValuesForYaw(self):
        kp = self.UPDATE_JSON['controls']['escs']['controllers']['k_p']['yaw_output']
        ki = self.UPDATE_JSON['controls']['escs']['controllers']['k_i']['yaw_output']
        kd = self.UPDATE_JSON['controls']['escs']['controllers']['k_d']['yaw_output']
        maxOffset = self.BEST_OFFSET["yaw"]
        kp = kp + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        ki = ki + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        kd = kd + round(random.uniform(-maxOffset, maxOffset), self.DECIMAL_PLACES)
        
        return kp, kd, ki

    def printResult(self):
        print("--------------------------")
        print("Found the following PID configs to be suitable:")
        print(self.BEST_JSON)
        print("--------------------------")


    def run(self):
        offsetCalculator = OffsetCalculator()
        self.update_config()
        while self.BEST_OFFSET['total'] > 0.5:
            droneSim = DroneSimulator()
            droneSim.simulationRun()
            droneOutput = droneSim.getDroneResponses()
            offset = offsetCalculator.calculateOffsets(droneOutput)
            if offset['total'] < self.BEST_OFFSET['total']:
                self.BEST_JSON = self.UPDATE_JSON
                self.BEST_OFFSET = offset
            self.createNewUpdateJson()
            self.update_config()
        self.printResult()


if __name__ == "__main__":
    pidFInder = PidFinder()
    pidFInder.run()
