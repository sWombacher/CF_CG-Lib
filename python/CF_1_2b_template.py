# CF_1_2b_template.py
# AND with LUT

from libcfcg import cf
import sys
import time
import csv

i_range = 64 # 128 # 512 
j_range = 64 # 128 # 512 
lutFile = 'chaos_files/Multcol4.pal'

def readLut():
    lut = []
    f = open(lutFile, 'r')
    reader = csv.reader(f, delimiter=',', quotechar='\n')
    for color in reader:
        lut.append(color)
    return lut

def setPixels(window, lut):
    # your code
    # test example for 1 pixel
    index = 4;
    color_i_j = cf.Color(int(lut[index][0]), int(lut[index][1]), 
                                 int(lut[index][2]))
    window.setColor(1, 1, color_i_j)

    return

def main():
    window = cf.WindowRasterized(i_range, j_range, "CF_1_2b")
    window.setWindowDisplayScale(512/i_range)
    window.show()
    lut = readLut()
    setPixels(window, lut) # implement your code there
    window.show()
    print("Press any key to finish")
    sys.stdout.flush() # force output
    time.sleep(0.1) # wait for console; increase if necessary
    window.waitKey()

main()

window = None # destroy window