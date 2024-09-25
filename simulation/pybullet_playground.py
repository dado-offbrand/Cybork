import pybullet as p
import time
import pybullet_data

# set up physics client
physicsClient = p.connect(p.GUI)
p.setAdditionalSearchPath(pybullet_data.getDataPath()) #"optionally"? what does that mean

# set up test environment
p.setGravity(0, 0, -10)
planeID = p.loadURDF("plane.urdf")

# set up test object
startPos = [0, 0, 1] #vec3?
startOrientation = p.getQuaternionFromEuler([0,0,0])
boxId = p.loadURDF("r2d2.urdf",startPos, startOrientation)
# how do i make a URDF file.....

# begin simulation?
for i in range (10000):
    p.stepSimulation()
    time.sleep(1./240.)

# print results
cubePos, cubeOrn = p.getBasePositionAndOrientation(boxId)
print(cubePos,cubeOrn)

# disconnect simulation
p.disconnect()