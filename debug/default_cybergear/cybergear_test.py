from cybergear_defs import *
import moteus
import struct

transport = moteus.Transport() # blank assumes mjcanusb?
mjcanusb_id = 0xFF # just can id 255, no way this conflicts with anything, right??

######################
# UTIL COMMANDS
######################

def send_command(motor_can_id, host_id, command_byte, data):
    try:
        arbitration_id = (command_byte << 24) | (mjcanusb_id << 8) | motor_can_id

        message = moteus.Message(arbitration_id=arbitration_id, data=data)
        transport.write(message)
        print(f"Wrote message with data: {data}")
    except Exception as e:
        print(f"Error sending CAN message: {e}")
    
def write_float_data(address, value, min, max):
    data = bytearray(8)
    data[0] = address & 0x00FF
    data[1] = address >> 8

    val = max if max < value else value
    val = min if min > value else value # if anything goes wrong double check this first
    data[4:8] = struct.pack('f', val) # equivalent of "memcpy(&data[4], &value, 4); in cpp"?

    send_command(0x7E, mjcanusb_id, 0x12, data)

######################
# DRIVER COMMANDS 
######################

def enable_motor():
    send_command(0x7E, mjcanusb_id, CMD_ENABLE, bytearray(8))

def reset_motor():
    send_command(0x7E, mjcanusb_id, CMD_RESET, bytearray(8))

def set_run_mode(run_mode):
    data = bytearray(8)
    data[0] = 0x7005 & 0x00FF # 0x7005 is defined as ADDR_RUN_MODE, address of run mode variable? idk
    data[1] = 0x7005 >> 8 # do not define in defs for now, this is a different thing from run_mode value thats passed
    data[4] = run_mode

    send_command(0x7E, mjcanusb_id, CMD_RAM_WRITE, data) #0x12 (or 18 as a byte) is the CMD_RAM_WRITE command

def set_limit_speed(speed):
    write_float_data(ADDR_LIMIT_SPEED, speed, 0.0, V_MAX)

def set_position_ref(position):
    write_float_data(ADDR_LOC_REF, position, P_MIN, P_MAX)

# could totally maybee cause an infinite loop, who knows
#def process_packet():

######################
# TEST AREA
######################

def main():
    # Following control_mode_example.ino, it seems that there is a procedure to start a cybergear
    
    # init_motor (includes resetting and setting mode to position)
    reset_motor()
    set_run_mode(MODE_POSITION) # MODE_POSITION is 0x01

    # set limit speed, then finally enable motor  
    set_limit_speed(30.0)
    enable_motor()

    # set speed to a not init speed
    set_limit_speed(1.0)

    # move motor with position mode
    target_position = 0.1 # if somehow magically nothing screams error but no movement, increase this value
    #also that std::fabs statement MIGHT be important buut who knows
    set_position_ref(target_position)

    # then process packet annnd mayhbe thats supposed to be in a loop
    #process_packet()


if __name__=="__main__":
    main()