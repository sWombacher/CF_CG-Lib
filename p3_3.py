import math
import numpy as np

import helper
import cf

# parameter
depth = 2

# solution
window = cf.WindowLin3D()
cylinders = cf.LinCylinderVec()

lsystem = cf.LindenmayerSystem()
lsystem.read("chaos_files/Baum_3d_1.lin")

string = cf.LSystem_Controller(depth, lsystem).getCompleteString()
print("String:")
print(string)

def addCylinder(position, direction):
    c = cf.Lin3DCylinder()
    c.color = cf.Color.ORANGE
    c.diameter = 1
    c.position = helper.convertArrayToVec3(position)
    c.direction = helper.convertArrayToVec3(direction)
    cylinders.push_back(c)


def createMatrix_helper(angle, cosPositions, sinPositions):
    mat = np.zeros((4, 4))
    angle = cf.degree2radian(angle)
    for pair in cosPositions:
        mat[pair[0]][pair[1]] = math.cos(angle)
    for pair in sinPositions:
        mat[pair[0]][pair[1]] = math.sin(angle)
    return mat

def createRotationMatrix_x(angle):
    mat = createMatrix_helper(angle, [(1,1), (2,2)], [(1,2), (2,1)])
    mat[0][0] = 1.0
    mat[1][2] *= -1.0
    return mat

def createRotationMatrix_y(angle):
    mat = createMatrix_helper(angle, [(0,0), (2,2)], [(0,2),(2,0)])
    mat[1][1] = 1.0
    mat[2][0] *= -1.0
    return mat

def createRotationMatrix_z(angle):
    mat = createMatrix_helper(angle, [(0,0), (1,1)], [(0,1),(1,0)])
    mat[2][2] = 1.0
    mat[1][0] *= -1.0
    return mat


transformation = createRotationMatrix_z(cf.degree2radian(lsystem.getStartAngle()))
position = np.matrix([0.0, 0.0, 0.0, 1.0])
angle = lsystem.getAdjustmentAngle()
stack_transformation = []
stack_position = []

for symbol in string:
    if symbol == 'X': continue

    elif symbol == '*': transformation *= createRotationMatrix_x(+angle)
    elif symbol == '/': transformation *= createRotationMatrix_x(-angle)

    elif symbol == '&': transformation *= createRotationMatrix_y(+angle)
    elif symbol == '^': transformation *= createRotationMatrix_y(-angle)

    elif symbol == '+': transformation *= createRotationMatrix_z(+angle)
    elif symbol == '-': transformation *= createRotationMatrix_z(-angle)

    elif symbol == '[':
        stack_position.append(position)
        stack_transformation.append(transformation)

    elif symbol == ']':
        position = stack_position.pop()
        transformation = stack_transformation.pop()

    else:
        direction = np.matrix([1.0, 0.0, 0.0, 0.0]) * transformation
        if symbol >= 'A' and symbol <= 'Z':
            pos = position.tolist()[0][:-1]
            dir = direction.tolist()[0][:-1]
            addCylinder(pos, dir)

        position += direction


window.setCylinders(cylinders)
window.waitKeyPressed();

# sorry about this but here probably comes an error
# -> please ignore it

