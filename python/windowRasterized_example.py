# windowRasterized_example.py
import sys
import time

from libcfcg import cf

window = cf.WindowRasterized(800, 600,
        "Window Rasterized example", cf.Color.WHITE)
window.show()

# if you like to scale the output image
# change the value to any float value you like
window.setWindowDisplayScale(1.0)

print("Please select a point")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
point = window.waitMouseInput()

print("Selected point: ", point.x, point.y)
window.drawCircle(point, 15, -1, cf.Color.ORANGE)

# test access and output
color = cf.Color.BLUE
print("BLUE : ",color.r, color.g, color.b)
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary

# lines from selected point to corners
maxX = window.getWidth() - 1
maxY = window.getHeight() - 1
window.drawLine(point, cf.Point(0, 0), 2, color)
window.drawLine(point, cf.Point(0, maxY), 2, color)
window.drawLine(point, cf.Point(maxX, 0), 2, color)
window.drawLine(point, cf.Point(maxX, maxY), 2, color)
window.show()

print("Press any key to continue")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

# access pixels themself and draw lines by hand
for x in range(window.getWidth()):
    color2 = window.getColor(x, point.y)
    color2 = cf.Color(int(color2.r * 0.5), 0, 0)  # dark red
    #color = cf.Color(255,0,0) # RED
    window.setColor(x, point.y, color2)

for y in range(window.getHeight()):
    color2 = window.getColor(point.x, y)
    color2 = cf.Color(0, int(color2.g), 0) # green
    window.setColor(point.x, y, color2)
window.show()

print("Press any key to finish")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

window = None

