import pybullet as p
import time
import pybullet_data

# set up physics client
physicsClient = p.connect(p.GUI)
p.setAdditionalSearchPath(pybullet_data.getDataPath()) #"optionally"? what does that mean

# set up test environment
p.setGravity(0, 0, -9.8)
planeID = p.loadURDF("plane.urdf")

# set up test object
startPos = [0, 0, 1] #vec3?
startOrientation = p.getQuaternionFromEuler([0,0,0])
robotId = p.loadURDF("example_quad.urdf", startPos, startOrientation)

# begin simulation?
while True:
    p.stepSimulation()

# disconnect simulation
#p.disconnect()