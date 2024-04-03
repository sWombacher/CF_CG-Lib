# windowRasterized_example.py
#
# for applications in pixel-space

import sys
import time
from libcfcg import cf
import csv

# read LUT and copy to array 'colors' 
file = open('chaos_files/Multcol4.pal')
reader = csv.reader(file, delimiter=',', quotechar='\n')
color_array = []
for color in reader:
    #print(color) # characters! !!
    #print(int(color[0]), int(color[1]), int(color[2]))# RGB-triple
    color_array.append(color)
    
# window for graphic output (pixel space)
window = cf.WindowRasterized(600, 600,
        "Window Rasterized example", cf.Color.WHITE)
window.show() # all drawing instructions buffered so far are executed
# change the float value to scale the output window
window.setWindowDisplayScale(1.0)
window.show() # show empty white window

# selection of a point via mouse; draw and print
print("Use mouse to set a point")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
point = window.waitMouseInput()
window.drawCircle(point, 15, -1, cf.Color.ORANGE) # buffered and not displayed until show()
#print("Orange : ",cf.Color.ORANGE.r, cf.Color.ORANGE.g, cf.Color.ORANGE.b)
print("point coordinates: ", point.x, point.y)

# use predefined colors
color = cf.Color.BLUE
#print("BLUE : ",color.r, color.g, color.b)

# 4 lines from selected point to corners of the window in BLUE
maxX = window.getWidth() - 1
maxY = window.getHeight() - 1
window.drawLine(point, cf.Point(0, 0), 2, color) # buffered and not displayed until show()
window.drawLine(point, cf.Point(0, maxY), 2, color)
window.drawLine(point, cf.Point(maxX, 0), 2, color)
window.drawLine(point, cf.Point(maxX, maxY), 2, color)
window.show() # all drawing instructions buffered so far are executed

print("Press any key to continue")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

# get the color of a pixel and draw lines
# horizontal line
for x in range(window.getWidth()):
    color = window.getColor(x, point.y)
    color_row = cf.Color(0, int(color.r * 0.9), 0)  # light green
    if ((color != cf.Color.ORANGE) and (color != cf.Color.BLUE)):
       window.setColor(x, point.y, color_row) # draw line behind the other objects

# vertical line; use color from LUT:  
index = 1 # index to read from LUT - lowest index is 0 !!!
color_column = cf.Color(int(color_array[index][0]), 
                        int(color_array[index][1]), int(color_array[index][2])) 
print("\nColor from LUT: ", int(color_column.r), int(color_column.g), 
      int(color_column.b))
for y in range(window.getHeight()):
    window.setColor(point.x, y, color_column) # draw line in front of all
window.show() # all drawing instructions buffered so far are executed
 
print("\nPress any key to finish")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

window = None # destroy window

