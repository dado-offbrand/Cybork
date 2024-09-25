import time
import busio
import board
import countio

import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.ads1x15 import Mode, Comp_Mode, Comp_Polarity, Comp_Latch
from adafruit_ads1x15.analog_in import AnalogIn

i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1015(i2c)
chan = AnalogIn(ads, ADS.P0)
#https://github.com/adafruit/Adafruit_CircuitPython_ADS1x15/blob/main/examples/ads1x15_comparator_example.py
# complete when controller is designed and wiring is done, adafruit_ads1x15 library
# will only run on the RPI, not a windows computer