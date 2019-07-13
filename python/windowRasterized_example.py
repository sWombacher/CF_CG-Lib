import cf

window = cf.WindowRasterized(800, 600,
        "Window Rasterized example", cf.Color.WHITE)

window.show()
print("Please select a point")
point = window.waitMouseInput()

print("Selected point: ", point.x, point.y)
window.drawCircle(point, 15, -1, cf.Color.ORANGE)

maxX = window.getWidth() - 1
maxY = window.getHeight() - 1
window.drawLine(point, cf.Point(0, 0), 2, cf.Color.BLUE)
window.drawLine(point, cf.Point(0, maxY), 2, cf.Color.BLUE)
window.drawLine(point, cf.Point(maxX, 0), 2, cf.Color.BLUE)
window.drawLine(point, cf.Point(maxX, maxY), 2, cf.Color.BLUE)

window.show()
print("Press any key to continue")
window.waitKey()

# access pixels themself and draw lines by hand
for x in range(window.getWidth()):
    color = window.getColor(x, point.y)
    color = cf.Color(int(color.r * 0.5), int(color.g * 0.5), int(color.b * 0.5))
    window.setColor(x, point.y, color)

for y in range(window.getHeight()):
    color = window.getColor(point.x, y)
    color = cf.Color(int(color.r * 0.1), int(color.g * 0.1), int(color.b * 0.1))
    window.setColor(point.x, y, color)

window.show()
window.waitKey()

