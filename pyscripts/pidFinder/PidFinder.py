from DroneSimulator import DroneSimulator
import os
import requests
import constants



UPDATE_JSON = {"controls": {"escs": {"controllers": {"k_i": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, "k_p": {"roll_rate": 1, "pitch_rate": 1, "roll_output": 1, "pitch_output": 1, "yaw_output": 1}, "k_d": {"roll_rate": 0,
                                                                                                                                                                                                                                                            "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, "k_aw": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, "max_diff": {"roll_rate": 8, "pitch_rate": 8, "roll_output": 50, "pitch_output": 50, "yaw_output": 25}}}}}
BEST_JSON = {"controls": {"escs": {"controllers": {"k_i": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, "k_p": {"roll_rate": 1, "pitch_rate": 1, "roll_output": 1, "pitch_output": 1, "yaw_output": 1}, "k_d": {"roll_rate": 0,
                                                                                                                                                                                                                                                           "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, "k_aw": {"roll_rate": 0, "pitch_rate": 0, "roll_output": 0, "pitch_output": 0, "yaw_output": 0}, "max_diff": {"roll_rate": 8, "pitch_rate": 8, "roll_output": 50, "pitch_output": 50, "yaw_output": 25}}}}}
def update_config(json):
    resp = requests.patch("http://" + constants.DRONE_HOSTNAME + ":" + constants.DRONE_API_PORT + "/config", json=json)
    if resp.status_code != 200:
        raise Exception("Couldn't patch config")
    return resp.json()
    
def run():
    while True:
        droneSim = DroneSimulator()
        droneSim.simulationRun()
        simRawResults = droneSim.getDroneResponses()

if __name__ == "__main__":
    run()
