import moteus

transport = moteus.Transport() # blank assumes mjcanusb?

# test enabling motor
command_to_send = 0x03 # CMD_ENABLE, equivalent to "mode" bits
mjcanusb_id = 0xFF # just can id 255, no way this conflicts with anything, right??
motor_can_id = 0x7E # targetMotorCanID bits

def enable_motor():
    try:
        arbitration_id = (command_to_send << 24) | (mjcanusb_id << 8) | motor_can_id
        data = bytes([0x00]*8) # CMD_ENABLE takes 8 empty bytes

        message = moteus.Message(arbitration_id=arbitration_id, data=data)
        transport.write(message)
        print("Wrote message, hopefully nothing explodes!")
    except Exception as e:
        print(f"Error sending CAN message: {e}")