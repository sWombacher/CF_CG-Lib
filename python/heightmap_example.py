import sys
import time
from libcfcg import cf
import math


window = cf.WindowRasterized(128, 128)
window.clear(cf.Color.BLACK)
window.show()

# use distance to center as an grey value
center = [window.getWidth() / 2, window.getHeight() / 2]
maxDistancePossible = math.sqrt(center[0] * center[0] + center[1] * center[1])

def getNormalizedDistance(point):
    vec = []
    vec.append(center[0] - point[0])
    vec.append(center[1] - point[1])
    vecLen = math.sqrt(vec[0] * vec[0] + vec[1] * vec[1])
    return vecLen / maxDistancePossible

for y in range(window.getHeight()):
    for x in range(window.getWidth()):
        dist = getNormalizedDistance([x, y])
        greyValue = int(dist * 255) # 255 is max value for colors
        color = cf.Color(greyValue, greyValue, greyValue)
        window.setColor(x, y, color)

window.show()
window.saveImage("chaos_files/heightmap.png")

print("\nPress any key to finish")
sys.stdout.flush() # force output
time.sleep(0.1) # wait for console; increase if necessary
window.waitKey()

window = None

