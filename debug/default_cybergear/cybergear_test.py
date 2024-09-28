from cybergear_defs import *
import moteus
import struct
import time
import asyncio

transport = moteus.Transport() # blank assumes mjcanusb?
mjcanusb_id = 0xFF # just can id 255, no way this conflicts with anything, right??

######################
# UTIL COMMANDS
######################

async def send_command(motor_can_id, host_id, command_byte, data):
    try:
        arbitration_id = (command_byte << 24) | (host_id << 8) | motor_can_id

        message = moteus.Command()
        message.raw = True
        message.arbitration_id = arbitration_id
        message.bus = host_id
        message.data = data
        message.reply_required = False

        await transport.cycle([message])
        time.sleep(CYBERGEAR_RESPONSE_TIME_USEC * 0.000001)

        print(f"Wrote {message} with data: {data}")
    except Exception as e:
        print(f"Error sending CAN message: {e}")
    
async def write_float_data(address, value, min, max):
    data = bytearray(8)
    data[0] = address & 0x00FF
    data[1] = address >> 8

    val = max if max < value else value
    val = min if min > value else value # if anything goes wrong double check this first
    data[4:8] = struct.pack('f', val) # equivalent of "memcpy(&data[4], &value, 4); in cpp"?

    await send_command(0x7F, mjcanusb_id, 0x12, data)

async def process_packet():
    check_update = False
    while True:
        if recieve_motor_data()

async def recieve_motor_data():
    print('wip')

######################
# DRIVER COMMANDS 
######################

async def enable_motor():
    await send_command(0x7F, mjcanusb_id, CMD_ENABLE, bytearray(8))

async def reset_motor():
    await send_command(0x7F, mjcanusb_id, CMD_RESET, bytearray(8))

async def set_run_mode(run_mode):
    data = bytearray(8)
    data[0] = ADDR_RUN_MODE & 0x00FF
    data[1] = ADDR_RUN_MODE >> 8
    data[4] = run_mode

    await send_command(0x7F, mjcanusb_id, CMD_RAM_WRITE, data)

async def set_limit_speed(speed):
    await write_float_data(ADDR_LIMIT_SPEED, speed, 0.0, V_MAX)

async def set_position_ref(position):
    await write_float_data(ADDR_LOC_REF, position, P_MIN, P_MAX)

# could totally maybee cause an infinite loop, who knows
#def process_packet():

######################
# TEST AREA
######################

async def main():
    # init_motor (includes resetting and setting mode to position)
    await reset_motor()
    await set_run_mode(MODE_POSITION) # MODE_POSITION is 0x01

    # set limit speed, then finally enable motor  
    await set_limit_speed(30.0)
    await enable_motor()

    # set speed to a not init speed
    await set_limit_speed(1.0)

    # move motor with position mode
    target_position = 3 # if somehow magically nothing screams error but no movement, increase this value
    
    while True:
        await set_position_ref(target_position)

        

    # then process packet annnd mayhbe thats supposed to be in a loop
    #process_packet()


if __name__=="__main__":
    asyncio.run(main())