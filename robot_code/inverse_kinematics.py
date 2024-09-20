import math

upper_length =  5.038
lower_length = 5.523267

def calc_angles(x, y):
    D = math.sqrt(x ** 2 + y ** 2)

    # Knee
    cos_theta2 = (upper_leg ** 2 + lower_leg ** 2 - D ** 2) / (2 * upper_leg * lower_leg)
    theta2 = math.acos(cos_theta2)  # θ2 in radians

    phi = math.atan2(x, y)

    cos_alpha = (upper_leg ** 2 + D ** 2 - lower_leg ** 2) / (2 * upper_leg * D)
    alpha = math.acos(cos_alpha)  # α in radians

    # Hip
    theta1 = phi - alpha  # θ1 in radians

    # Convert rad-->rev
    theta1_revolutions = theta1 / (2 * math.pi)
    theta2_revolutions = theta2 / (2 * math.pi)

    return theta1_revolutions, theta2_revolutions