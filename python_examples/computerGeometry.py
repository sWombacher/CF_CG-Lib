from enum import Enum
import cf


points = [] # python representation of point vectors
cfPoints = [] # internal 2D representation with x and y
pointVectors = [] # internal pointVector representation with x, y, and w

def readPoints():
    data = cf.readDatFilePointVector("/home/bloodyangel/git_hub/ChaosAndFractal_Lib/geometry_files/FIRST.DAT")

    for i in range(0, data.size()):
        pv = data.get(i)
        points.append([pv.getX(), pv.getY(), pv.getW()])

    for p in range(0, len(points)):
        if points[p][2] != 1:
            raise ValueError;
        cfPoints.append(cf.Point(points[p][0], points[p][1]))
        pointVectors.append(cf.PointVector(points[p][0], points[p][1]))


def equalZero(value):
    return abs(value) < 0.000001


readPoints()

# part 1, create coordinatesystem & draw all points/lines
window = cf.WindowCoordinateSystem(800, cf.Interval(-10, 350), cf.Interval(-10, 350), "Window Name")
window.drawAxis(cf.Color.BLACK, 10, 10)

for p in points:
    print("Drawing point: ", p)
    window.drawPoint(cf.Point(p[0], p[1])) # default color is black

for i in range(0, len(cfPoints)):
    p0 = cfPoints[i]
    p1 = cfPoints[(i+1) % len(cfPoints)]
    window.drawLine(p0, p1)

window.show()

# wait for user input
print("Please set a point\n")
userPoint = window.waitMouseInput()
userPoint_pv = cf.PointVector(userPoint)
print("Set point: x=", userPoint.x, "    y=", userPoint.y)

window.drawPoint(userPoint, cf.Color.RED)
window.show()

class PointDescription(Enum):
    Left = 0,
    Right = 1,
    On = 2

def calcSide(pv0, pv1):
    innerProduct = pv0.crossProduct(pv1).innerProduct(userPoint_pv)
    if equalZero(innerProduct):
        return PointDescription.On
    if innerProduct < 0:
        return PointDescription.Right
    return PointDescription.Left

des0 = calcSide(pointVectors[0], pointVectors[1])
des1 = calcSide(pointVectors[1], pointVectors[2])
des2 = calcSide(pointVectors[2], pointVectors[0])

if des0 == des1 and des1 == des2:
    if des0 == PointDescription.Left:
        print("Point is inside of the Triangle")
    elif des0 == PointDescription.On:
        print("Point is on of the Triangle")
    else:
        print("Point is outside of the Triangle")
else:
    print("Point is outside of the Triangle")

print("Press enter to continue with part2")
window.waitKey()


# part 2
window.clear()
window.drawAxis(cf.Color.BLACK, 10, 10)
window.show()

print("Press enter (or wait 3s) to draw a line between point0 and point2\n")
window.waitKey(3000) # wait up to 3000ms until continue

normal = pointVectors[2].crossProduct(pointVectors[0])
print("Normal:   x=", normal.getX(), "   y=", normal.getY(), "   z=", normal.getW())
window.drawLinearEquation(normal)
# alternative:
# window.drawLinearEquation(normal.getX(), normal.getY(), normal.getW())
window.show()

print("Press enter (or wait 3s) to draw line between point1 and point0\n")
window.waitKey(3000)

# use point/direction vectors
direction = pointVectors[1].sub(pointVectors[0])
window.drawLinearEquation(cfPoints[1], direction, cf.Color.RED, cf.Window2D.LineType_DOT_1)
window.show()

print("Press enter to draw line between point1 and point2\n")
window.waitKey()

# draw line from point1 to point2 directly
window.drawLine(cfPoints[1], cfPoints[2], cf.Color.BLUE, cf.Window2D.LineType_DOT_DASH_0)
window.show()

print("Press enter to draw a circle around your point")
window.waitKey()

window.drawCircle(userPoint, 19, cf.Color.CYAN, -1)
window.drawCircle(userPoint, 19, cf.Color.MAGENTA, 2)
window.drawPoint(userPoint)
window.show()

window.drawCirclePart(userPoint, 23, 0, 90, cf.Color.RED, 2);
window.drawCirclePart(userPoint, 23, 90, 360, cf.Color.BLUE, 2);
window.show()

print("Press enter to continue with the last example")
window.waitKey()

# alternative circle part
# start/end have to be the same distance to center!
pv = cf.PointVector(userPoint)
direction = cf.DirectionVector(17, -23, 0)
start = cf.Point(pv.add(direction))
end = cf.Point(pv.add(direction.getVector90Degree()))

window.drawCirclePart(userPoint, start, end, cf.Color.GREEN, 3, True)
window.drawCirclePart(userPoint, start, end, cf.Color.ORANGE, 3, False)
window.show()


# end
print("Press any key to finish")
window.waitKey()

