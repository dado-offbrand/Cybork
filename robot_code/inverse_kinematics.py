import math

upper_length =  5.038
lower_length = 5.523267

def calc_angles(x, y):
    D = math.sqrt(x ** 2 + y ** 2)

    # Knee
    cos_theta2 = (upper_length ** 2 + lower_length ** 2 - D ** 2) / (2 * upper_length * lower_length)
    theta2 = math.acos(cos_theta2)  # θ2 in radians

    phi = math.atan2(x, y)

    cos_alpha = (upper_length ** 2 + D ** 2 - lower_length ** 2) / (2 * upper_length * D)
    alpha = math.acos(cos_alpha)  # α in radians

    # Hip
    theta1 = phi - alpha  # θ1 in radians

    # Convert rad-->rev
    #theta1_revolutions = theta1 / (2 * math.pi)
    #theta2_revolutions = theta2 / (2 * math.pi)

    # Covnert rad-->deg
    theta1_deg = math.degrees(theta1)
    theta2_deg = math.degrees(theta2)

    #return theta1_revolutions, theta2_revolutions
    return theta1_deg, theta2_deg