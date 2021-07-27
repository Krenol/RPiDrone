from pydantic import BaseModel, ValidationError, validator
from typing import List, Optional

class Logic(BaseModel):
    motors_off_on_disconnect: Optional[bool] = None
    rpi_reset_pin: Optional[int] = None

class Server(BaseModel):
    port: Optional[int] = None
    bytes: Optional[int] = None
    delimiter: Optional[str] = None

class Flightcontroller(BaseModel):
    port: Optional[str] = None
    baudrate: Optional[int] = None
    max_serial_buffer: Optional[int] = None

class Leds(BaseModel):
    on_led: Optional[int] = None
    status_led: Optional[int] = None

class Queues(BaseModel):
    read_size: Optional[int] = None
    write_size: Optional[int] = None

class GPS(BaseModel):
    port: Optional[str] = None
    baudrate: Optional[int] = None

class Sensors(BaseModel):
    calibration: Optional[bool] = None
    decimal_places: Optional[int] = None
    gps: Optional[GPS] = None

class Controller(BaseModel):
    roll_rate: Optional[float] = None
    pitch_rate: Optional[float] = None
    roll_output: Optional[float] = None
    pitch_output: Optional[float] = None
    yaw_output: Optional[float] = None

class Controllers(BaseModel):
    k_i: Optional[Controller] = None
    k_p: Optional[Controller] = None
    k_d: Optional[Controller] = None
    k_aw: Optional[Controller] = None
    max_diff: Optional[Controller] = None

class Pins(BaseModel):
    pos: Optional[str] = None
    pin: Optional[int] = None
    @validator('pos')
    def pos_must_exist(cls,pos):
      posis=['lf','rf','lb', 'rb']
      if pos not in posis:
        raise ValueError(f'must be in {posis}')
      return pos
    @validator('pin')
    def pin_must_exist(cls,pin):
      if pin < 0 or pin > 32:
        raise ValueError(f'must be in range of 0..32')
      return pin

class ESCs(BaseModel):
    calibrate: Optional[bool] = None
    min: Optional[int] = None
    max: Optional[int] = None
    idle: Optional[int] = None
    controllers: Optional[Controllers] = None
    pins: List[Pins] = None

class Controls(BaseModel):
    max_pitch_rate: Optional[float] = None
    max_roll_rate: Optional[float] = None
    max_yaw_velocity: Optional[float] = None
    escs: Optional[ESCs] = None

class Config(BaseModel):
    logic: Optional[Logic] = None
    server: Optional[Server] = None
    flightcontroller: Optional[Flightcontroller] = None
    leds: Optional[Leds] = None
    queues: Optional[Queues] = None
    sensors: Optional[Sensors] = None
    controls: Optional[Controls] = None