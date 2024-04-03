# CF_1_4_template.py
# rectangle subdivision
# Here with "WindowRasterized" - that means (0,0) is top/left !!! 

from libcfcg import cf
import sys
import time

color = cf.Color.BLACK
i_range = 512 #64 #8
j_range = 512 #64 #8

###
# Recursive function painting the top right rectangle
# window: surface of the framework
# bot_left: bottom left corner of the rectangle to be processed
# top_right: top right corner of the rectangle to be processed
###
def paint(window, bot_left, top_right):
    width = top_right.x - bot_left.x + 1
    height = bot_left.y - top_right.y + 1
    # Example for drawing a rectangle 
    # (1. param: bottom left corner, 2. param: top right corner)
    bot_left_t_r = cf.Point(bot_left.x + width/2, bot_left.y - height/2)
    window.drawRectangle(bot_left_t_r, top_right,
                         -1, cf.Color.BLACK)  # -1 indicates 'fill rect'
    #window.show()   # show the progress after every recursive function call

    # Your code here:
    # 1.) Call the recursive function for the top left, bottom left and 
    #     bottom right square
    # 2.) Terminate process right in time
    

def main():
    window = cf.WindowRasterized(i_range, j_range, "CF_1_4")
    window.setWindowDisplayScale(512/i_range)    
    window.clear(cf.Color.RED)
    # Define initial corners
    bot_left = cf.Point(0, window.getHeight() - 1)
    top_right = cf.Point(window.getWidth() - 1, 0)
    # Call the recursive function
    paint(window, bot_left, top_right)
    window.show()
    print("Press any key to finish")
    sys.stdout.flush()
    time.sleep(0.1)
    window.waitKey()

main()

window = None