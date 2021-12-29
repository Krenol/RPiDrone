import json

class OffsetCalculator():   
    def __init__(self):
        self.offsets = {'yaw': 0, 'pitch': 0, "roll": 0, 'total': 0}
        
    def calculateOffsets(self, droneOutput):
        self.offsets = {'yaw': 0, 'pitch': 0, "roll": 0, 'total': 0}
        self.calculateYPROffsets(droneOutput)
        self.calculateMeanOffsets(len(droneOutput))
        return self.offsets
    
    def calculateYPROffsets(self, droneOutput):
        for output in droneOutput:
            output = json.loads(output)
            self.offsets['yaw'] += output['angles']['is']['yaw'] - output['angles']['should']['yaw']
            self.offsets['pitch'] += output['angles']['is']['pitch'] - output['angles']['should']['pitch']
            self.offsets['roll'] += output['angles']['is']['roll'] - output['angles']['should']['roll']
            
    def calculateMeanOffsets(self, listLength):
        self.offsets['yaw'] /= listLength
        self.offsets['pitch'] /= listLength
        self.offsets['roll'] /= listLength
        self.offsets['total'] = (self.offsets['yaw'] + self.offsets['pitch'] + self.offsets['roll']) / 3
        