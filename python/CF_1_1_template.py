# CF_1_1_template.py
# chaos game

from libcfcg import cf
import sys
import time
import random

i_range = 256 # 64 # 128 # 512 
j_range = 256 # 64 # 128 # 512 
points = []

def Iteration(window):
    while True:
        # your code HERE
        
        # test example for 2 pixels
        column = random.randrange(i_range - 5) # example for 'random'
        row = 1 # look at topmost line
        # how to define a Point
        point1 = cf.Point(column, row)
        window.setColor(point1.x, point1.y, cf.Color.RandomColor()) # may be invisible!!!
        window.setColor(point1.x + 2, point1.y, cf.Color.YELLOW) # visible!!!
        window.show()

        key = window.waitKey(1)          
        if key == 27: 
            break
    return

def main():
    window = cf.WindowRasterized(i_range, j_range, "CF_1_1")
    window.setWindowDisplayScale(512/i_range)
    window.show()
    
    print("Use mouse to set a point")
    sys.stdout.flush() # force output
    time.sleep(0.1) # wait for console; increase if necessary
    point2 = window.waitMouseInput()
    window.drawCircle(point2, 2, -1, cf.Color.RED) # circle
    window.show()
    points.append([point2.x, point2.y])

    print("Press ESC and keep pressed to exit iteration")
    Iteration(window) # implement your code there
    
    print("Press ENTER to exit")
    while True: 
        key = window.waitKey(1)          
        if key == 13: 
            break

main()
window = None # destroy window