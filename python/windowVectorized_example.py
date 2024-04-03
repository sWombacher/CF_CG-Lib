# windowVectorized_example.py
#
# for applications in 2D float intervals
# are mapped to a correspondig pixel-image

import math
import sys
import time

from libcfcg import cf

xInterval = [-0.5, 10.0] # float interval that is mapped to the image
yInterval = [-5.0, 5.0]  # defined below in cf.WindowVectorized(...)
print("x in Interval: ", xInterval)
print("y in Interval: ", yInterval)
cfIntervalX = cf.Interval(xInterval[0], xInterval[1])
cfIntervalY = cf.Interval(yInterval[0], yInterval[1])

# image corresponding to the 2D float interval
window = cf.WindowVectorized(512, cfIntervalX, cfIntervalY,
        "Window Vectorized example", cf.Color.ORANGE)
window.drawAxis() # buffered and not displayed until show()

# if you like to scale the output image set the desired float value
window.setWindowDisplayScale(1.0)

window.show() # all drawing instructions buffered so far are executed 

print("Use mouse to set a point")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
point = window.waitMouseInput()
window.drawCircle(point, 3, -1, cf.Color.GREY) # buffered and not displayed until show()
window.show() # all drawing instructions buffered so far are executed
#transforming points from interval to image and vice versa
pixelPoint = window.transformPoint_fromInterval_toImage(point)
print("Point in interval coordinates: ", point.x, point.y)
print("Point in image coordinates: ", pixelPoint.x, pixelPoint.y)
# test: transform image position back to interval coordinates
pointRe = window.transformPoint_fromImage_toInterval (pixelPoint)
print("Image coordinates transformed back to interval: ", pointRe.x, pointRe.y)

print("Use mouse to set one more point")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
p0 = window.waitMouseInput()
window.drawCircle(p0, 3, -1, cf.Color.BLUE)

# how to define and draw a point of Point
p1 = cf.Point((point.x + p0.x)/2.0, (point.y + p0.y)/2.0)
window.setColor(p1.x, p1.y, cf.Color.WHITE) # nearly invisible point
window.drawCircle(p1, 13, 1, cf.Color.WHITE) # circle around point
window.show() # all drawing instructions buffered so far are executed

print(">>> The white point in the middle of the circle is nearly invisible!")
print("Press any key to draw a line and a rectangle")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()
window.drawLine(p0, p1, 2, cf.Color.BLACK)
window.drawRectangle(p0, p1, 2, cf.Color.GREEN)
window.show() # all drawing instructions buffered so far are executed

print("Press any key to draw a circle")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()
# calculate point distance
diffX = p0.x - p1.x
diffY = p0.y - p1.y
distInInterval = math.sqrt(diffX * diffX + diffY * diffY)
# unfortunately dist has to be converted into pixel length
distInPixel = int(window.convert_intervalLength_to_pixelLength(distInInterval))
window.drawCircle(p0, distInPixel, 2, cf.Color.BLUE)
window.show() # all drawing instructions buffered so far are executed

print("Press any key to draw a part of a circle")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()
# calculate slope and angels
slope = diffY / diffX
baseAngle = math.atan(slope)
degree = cf.radian2degree(baseAngle)
window.drawCirclePart(p1, distInPixel, degree + 60, degree - 60, 2, cf.Color.GREY)
window.show() # all drawing instructions buffered so far are executed

print("Press any key to finish")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

window = None # destroy window