import cf
import math


xInterval = [-0.5, 10.0]
yInterval = [-5.0, 5.0]
print("Using a X Interval of: ", xInterval)
print("Using a Y Interval of: ", yInterval)

cfIntervalX = cf.Interval(xInterval[0], xInterval[1])
cfIntervalY = cf.Interval(yInterval[0], yInterval[1])

window = cf.WindowVectorized(700, cfIntervalX, cfIntervalY,
        "Window Vectorized example", cf.Color.ORANGE)
window.drawAxis()
window.show()

print("Please set a point on the window")

point = window.waitMouseInput()
pixelPoint = window.transformPoint_fromInterval_toImage(point)

print("Point pressed: ", point.x, point.y)
print("Point pixel position pressed: ", pixelPoint.x, pixelPoint.y)

window.drawCircle(point, 3, -1, cf.Color.WHITE)
window.show()

print("Please set 2 more points")
p0 = window.waitMouseInput()
window.drawCircle(p0, 3, -1, cf.Color.BLUE)
window.show()
p1 = window.waitMouseInput()
window.drawCircle(p1, 3, -1, cf.Color.BLUE)
window.show()

window.drawLine(p0, p1, 2, cf.Color.BLACK)
print("Press enter to draw a rectangle")
window.waitKey()

window.drawRectangle(p0, p1, 2, cf.Color.GREEN)
window.show()
print("Press enter to draw a circle")
window.waitKey()

# calculate point distance
diffX = p0.x - p1.x
diffY = p0.y - p1.y
dist = math.sqrt(diffX * diffX + diffY * diffY)

# unfortunately dist has to be converted into pixel length
dist = int(window.convert_intervalLength_to_pixelLength(dist))
window.drawCircle(p0, dist, 2, cf.Color.BLUE)
window.show()
print("Press enter to draw a part of a circle")
window.waitKey()

# calculate slope and angels
slope = diffY / diffX
baseAngle = math.atan(slope)
degree = cf.radian2degree(baseAngle)

window.drawCirclePart(p1, dist, degree + 60, degree - 60, 2, cf.Color.GREY)

window.show()
window.waitKey()

