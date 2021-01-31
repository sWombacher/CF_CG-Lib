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

window.show() # no display of drawings without this lien!!!

# wait for user input
print("Please set a point\n")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
userPoint = window.waitMouseInput()
userPointV = cf.PointVector(userPoint) # conversion for calculations

print("point: x=", userPoint.x, "    y=", userPoint.y)
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.drawPoint(userPoint, cf.Color.RED)
window.show()

class PointDescription(Enum):
    Left = 0,
    Right = 1,
    On = 2

def calcSide(pVA, pVB, userPV): # calculate position of userpoint in relation to a line
    line = pVA.crossProduct(pVB) # implicit form of line through both points

    hnfFactor = 1/(math.sqrt(line.getX()**2 + line.getY()**2)) # HNF-Factor
    line = line*hnfFactor   # HNF of line but without consideration of sign!!!
    
    innerProduct = line.innerProduct(userPV) 
    if abs(innerProduct) < 1 :  # "1" due to discrete coordinates!!!
        return PointDescription.On
    if innerProduct < 0:
        return PointDescription.Right
    return PointDescription.Left

# calculation of the position of the userpoint in relation to all lines
des0 = calcSide(pointVectorsV[0], pointVectorsV[1], userPointV)
des1 = calcSide(pointVectorsV[1], pointVectorsV[2], userPointV)
des2 = calcSide(pointVectorsV[2], pointVectorsV[0], userPointV)

# message whether point is inside, outside or on the border of the triangle
if des0 == des1 and des1 == des2:
    if des0 == PointDescription.Left:
        print("Point is inside the triangle")
        sys.stdout.flush() # force output
        time.sleep(0.1) # wait for console; increase if necessary
    else:
        sys.stdout.flush() # force output
        time.sleep(0.1) # wait for console; increase if necessary
        print("Point is outside the triangle")
else:
    if des0 == PointDescription.On or des1 == PointDescription.On or des2 == PointDescription.On:
        print("Point is on the border of the triangle")
        sys.stdout.flush() # force output
        time.sleep(0.1) # wait for console; increase if necessary
    else:
        print("Point is outside the triangle")
        sys.stdout.flush() # force output
        time.sleep(0.1) # wait for console; increase if necessary

print("Press any key  to continue with part2")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()


# part 2
window.clear()
window.drawAxis(cf.Color.BLACK, 10, 10)
window.show()

print("Press any key (or wait 3s) to draw a line between point0 and point2\n")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey(3000) # wait up to 3000ms until continue

# line through 2 points
normalV1 = pointVectorsV[2].crossProduct(pointVectorsV[0])
print("Normal-Vector:   x=", normalV1.getX(), "   y=", normalV1.getY(), "   z=", normalV1.getW())
window.drawLinearEquation(normalV1)
# alternative:
# window.drawLinearEquation(normalV.getX(), normalV.getY(), normalV.getW())
window.show()

print("Press any key (or wait 3s) to draw line between point1 and point0\n")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey(3000)

# line with point and direction vector
direction = cf.DirectionVector(pointVectorsV[1].sub(pointVectorsV[0]))
normalV2 = direction.crossProduct(pointVectorsV[1])
window.drawLinearEquation(normalV2, cf.Color.RED, cf.Window2D.LineType_DOT_1)
# alternative for drawing only:
#window.drawLinearEquation(cfPoints[1], direction, cf.Color.RED, cf.Window2D.LineType_DOT_1)
window.show()

print("Press any key to draw line between point1 and point2\n")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

# draw line from point1 to point2 directly
window.drawLine(cfPoints[1], cfPoints[2], cf.Color.BLUE, cf.Window2D.LineType_DOT_DASH_0)
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
