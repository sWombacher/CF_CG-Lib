# small_examples.py

import math
from libcfcg import cf

# read 'ant' files
value = cf.readAntString('chaos_files/Ant_10.ant')
print("String in ant file: ", value)

# iterate over multiple colors
colors = [ cf.Color.RED, cf.Color.GREEN, cf.Color.BLUE, cf.Color.ORANGE ]
for color in colors:
    print("Current color: ", color.r, color.g, color.b)

# calculation with big numbers is built into python
value = 12345678901234567890
print(value, " modulo 13 is: ", value % 13)
value = 2**1024 # 2 to the power of 1024
print(value, " modulo 13 is: ", value % 13)

# convert between degree and radians
value = 180.0
print(value, " in radiants is: ", cf.degree2radian(value))
value = math.pi * 1.5
print(value, " in degree is: ", cf.radian2degree(value))

# direction example
west = cf.Direction.AbsoluteDirection_WEST
right = cf.Direction.RelativeDirection_RIGHT
westTurnRight = cf.Direction.getNextiDirection(west, right)
print("If your current direction is ",
        cf.Direction_toString(west), "\n",
        "and you take a right turn you are looking: ",
        cf.Direction_toString(westTurnRight))

# read integer/float values
value1 = float(input('Enter a float value: '))
value2 = int(input('Enter an int value: '))
print("float value:", value1, "   int value:", value2)

window = None # destroy window
