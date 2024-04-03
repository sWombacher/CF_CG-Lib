#  computerGeometry_example.py

# ACHTUNG: bitte lesen!
# =====================
# Punkt-Objekte gibt es als zwei verschiedene Typen:  
# 1.) zum Zeichnen als Typ: cf.Point  mit x- und y-Komponente 
#                                     (auch bei Maus-Eingabe)
# 2.) zum homogenen Rechnen als Typ: cf.PointVector mit x-, y- und w-Komponente
# Konvertierung:
# - Point P zu PointVector PV:     PV = cf.PointVector(P)
# - PointVector PF zu Point P:     P = cf.Point(PF)
#
# Fuer homogene Berechnungen mit Punkt- oder Richtungs-Vektoren stehen z.B. 
# zur Verfuegung:
# - Kreuz-Produkt:  PV2 = PV1.crossProduct(PV0)
# - Innen-Produkt: PV2 = PV1.innerProduct(PV0) 
# - Addition: PV2 = PV1.add(PV0)
# - Subtraktion: PV2 = PV1.sub(PV0)
# (Diese Operatoren sind leider nicht in der Doku zu finden, werden aber 
# zu Demo-Zwecken unten im Programm  verwendet.)
#
# Problem 1 (Umkopieren von Punkt-Vektoren):
#   NICHT mittels  PV2 = PV1  da Aenderungen an PV2 sich auch auf PV1 auswirken!!!
#   Sondern:  PV2 = cf.PointVector(PV1.getX(), PV1.getY(), PV1.getW())
#      oder:  PV2 = PV1.clone()
# Problem 2 (Return-Wert von normalize() bei Homogenisierung=Normalisierung):
#   Aufruf immer nur:   X.normalize()   und nicht:    Y=X.normalize()
#
import sys
import time
import math
from enum import Enum
from libcfcg import cf

pointsV = [] # python representation of point vectors
cfPoints = [] # internal 2D representation with x and y
pointVectorsV = [] # internal pointVector representation with x, y, and w

def myEqualZero(value):  # if you expect 0 it's mostly a very small number but not 0!!!
    return abs(value) < 0.001

def mySign(value):  # difference to NumPy:  mySign(0)=1    !!!
    if value < 0:
        return -1
    return 1

def readPoints():
    dataV = cf.readDatFilePointVector("geometry_files/FIRST.DAT")

    for i in range(0, dataV.size()):
        pV = dataV.get(i)
        pointsV.append([pV.getX(), pV.getY(), pV.getW()])

    for pV in range(0, len(pointsV)):
        if pointsV[pV][2] != 1:
            raise ValueError;
        cfPoints.append(cf.Point(pointsV[pV][0], pointsV[pV][1]))  # drawable points
        pointVectorsV.append(cf.PointVector(pointsV[pV][0], pointsV[pV][1])) # list of Point-Vectors

readPoints()

# part 1, create coordinatesystem & draw all points/lines
window = cf.WindowCoordinateSystem(600, cf.Interval(-10, 270), cf.Interval(-10, 270), "Window Name")
window.setWindowDisplayScale(1.0)
window.drawAxis(cf.Color.BLACK, 10, 10)

# Read points of the triangle
for pV in pointsV:
    print("Drawing point: ", pV)
    sys.stdout.flush() # force output
    time.sleep(0.1) # wait for console; increase if necessary
    window.drawPoint(cf.Point(pV[0], pV[1])) # default color is black
# Draw points of the triangle
for i in range(0, len(cfPoints)):
    p0 = cfPoints[i]
    p1 = cfPoints[(i+1) % len(cfPoints)] 
    window.drawLine(p0, p1)

window.show() # no display of drawings without this line!!!

# wait for user input
print("\nPlease use mouse to set a point")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
userPoint = window.waitMouseInput()
userPointV = cf.PointVector(userPoint) # conversion for calculations

print("point: x=", userPoint.x, "    y=", userPoint.y)
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.drawPoint(userPoint, cf.Color.RED)
window.show()

# test if point is inside or outside of triangle (border=inside)
line0 = pointVectorsV[0].crossProduct(pointVectorsV[1]) # implicit line
line1 = pointVectorsV[1].crossProduct(pointVectorsV[2]) # implicit line
line2 = pointVectorsV[2].crossProduct(pointVectorsV[0]) # implicit line
sign0 = mySign(line0.innerProduct(userPointV)) # "1" if left of line and 
sign1 = mySign(line1.innerProduct(userPointV)) # "-1" if right of line
sign2 = mySign(line2.innerProduct(userPointV))
if (sign0>=0 and sign1>=0 and sign2>=0):
    print("Point inside triangle")
else:
    print("Point outside triangle")

print("Press any key  to continue with part2")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

# *********************************************************************
# part 2
window.clear()
window.drawAxis(cf.Color.BLACK, 10, 10)
window.show()

print("Press any key (or wait 3s) to draw a line between point0 and point2")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey(3000) # wait up to 3000ms until continue

# line through 2 points (implicit form)
normalV0 = pointVectorsV[2].crossProduct(pointVectorsV[0])
print("Normal-Vector:   x=", normalV0.getX(), "   y=", normalV0.getY(), "   w=", normalV0.getW())
window.drawLinearEquation(normalV0, cf.Color.GREEN, cf.Window2D.LineType_DOT_DASH_0)
# alternative:
# window.drawLinearEquation(normalV0.getX(), normalV0.getY(), normalV0.getW())
window.show()

print("Press any key (or wait 3s) to draw line between point1 and point0")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey(3000)

# line with point and direction vector (parametric form)
direction = cf.DirectionVector(pointVectorsV[1].sub(pointVectorsV[0]))
normalV1 = pointVectorsV[1].crossProduct(direction)
window.drawLinearEquation(normalV1, cf.Color.RED, cf.Window2D.LineType_DOT_1)
# alternative for drawing only:
#window.drawLinearEquation(cfPoints[1], direction, cf.Color.RED, cf.Window2D.LineType_DOT_1)
window.show()

print("Press any key to draw a line between point1 and point2")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

# draw line from point1 to point2 directly
window.drawLine(cfPoints[1], cfPoints[2], cf.Color.BLUE, cf.Window2D.LineType_DEFAULT, 3)
window.show()

print("Press any key to draw a circle around your point")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

# draw circles and parts of circles
window.drawCircle(userPoint, 19, cf.Color.CYAN, -1)
window.drawCircle(userPoint, 19, cf.Color.MAGENTA, 2)
window.drawPoint(userPoint)
window.show()

window.drawCirclePart(userPoint, 23, 0, 90, cf.Color.RED, 2);
window.drawCirclePart(userPoint, 23, 90, 360, cf.Color.BLUE, 2);
window.show()

print("Press any key to continue with the last example")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

# alternative circle part
# start/end have to be in the same distance to center!
pv = cf.PointVector(userPoint)
direction = cf.DirectionVector(17, -23, 0)
start = cf.Point(pv.add(direction))
end = cf.Point(pv.add(direction.getVector90Degree()))

window.drawCirclePart(userPoint, start, end, cf.Color.GREEN, 3, True)
window.drawCirclePart(userPoint, start, end, cf.Color.ORANGE, 3, False)
window.show()

# end
print("Press any key to finish")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

window = None
