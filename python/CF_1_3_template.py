# CF_1_3_template.py
# Pascal triangle 
from libcfcg import cf
import sys
import time

xrange= 513 # 129 # 65 # 9 # ATTENTION: 1 additional column
yrange= 512 # 128 # 64 # 8

def calculateMatrix():
    # your code
    matrix = [[]]
    return matrix

def setOddValues(window, matrix):
    # your code
    return

def main():

    window = cf.WindowRasterized(xrange-1, yrange, "CF_1_3", cf.Color.BLACK)
    window.setWindowDisplayScale(512/yrange)
    window.show()
    matrix = calculateMatrix() # implement your code there
    setOddValues(window, matrix) # implement your code there
    window.show()
    print("Press any key to finish")
    sys.stdout.flush()
    time.sleep(0.1)
    window.waitKey()

main()

window = None # destroy window