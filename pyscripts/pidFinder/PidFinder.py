from DroneSimulator import DroneSimulator
import json
import requests
import constants
from OffsetCalculator import OffsetCalculator
import sys
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


    def update_config(self):
        resp = requests.patch("http://" + constants.DRONE_HOSTNAME +
                            ":" + str(constants.DRONE_API_PORT) + "/config", json=json.dumps(self.UPDATE_JSON))
        if resp.status_code != 200:
            raise Exception("Couldn't patch config")
        return resp.json()


    def createNewUpdateJson(self):
        kp_pr_rate = self.UPDATE_JSON['controls']['escs']['controllers']['k_p']['roll_rate']
        ki_pr_rate = self.UPDATE_JSON['controls']['escs']['controllers']['k_i']['roll_rate']
        kd_pr_rate = self.UPDATE_JSON['controls']['escs']['controllers']['k_d']['roll_rate']
        maxOffset_pr = max(self.BEST_OFFSET["pitch"], self.BEST_OFFSET["roll"])
        
        kp_pr_rate = kp_pr_rate + random.uniform(-maxOffset_pr, maxOffset_pr)
        ki_pr_rate = ki_pr_rate + random.uniform(-maxOffset_pr, maxOffset_pr)
        kd_pr_rate = kd_pr_rate + random.uniform(-maxOffset_pr, maxOffset_pr)
        self.UPDATE_JSON = {"controls": {"escs": {"controllers": 
            {"k_i": {"roll_rate":  ki_pr_rate, "pitch_rate": ki_pr_rate, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "k_p": {"roll_rate": kp_pr_rate, "pitch_rate": kp_pr_rate, "roll_output": 1, "pitch_output": 1, "yaw_output": 1}, 
            "k_d": {"roll_rate": kd_pr_rate, "pitch_rate": kd_pr_rate, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "k_aw": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, 
            "max_diff": {"roll_rate": 8, "pitch_rate": 8, "roll_output": 50, "pitch_output": 50, "yaw_output": 25}}}}}


    def printResult(self):
        print("--------------------------")
        print("Found the following PID configs to be suitable:")
        print(self.BEST_JSON)
        print("--------------------------")


    def run(self):
        offsetCalculator = OffsetCalculator()
        self.createNewUpdateJson()
        print(self.UPDATE_JSON)
        #self.update_config()
        # while BEST_OFFSET > 1:
        #     droneSim = DroneSimulator()
        #     droneSim.simulationRun()
        #     droneOutput = droneSim.getDroneResponses()
        #     offset = offsetCalculator.calculateOffsets(droneOutput)
        #     if offset['total'] < BEST_OFFSET['total']:
        #         BEST_JSON = UPDATE_JSON
        #         BEST_OFFSET = offset
        #     createNewUpdateJson()
        #     update_config(UPDATE_JSON)
        self.printResult()


if __name__ == "__main__":
    pidFInder = PidFinder()
    pidFInder.run()
