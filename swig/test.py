import cf

interval_x = cf.Interval(-1, 20)
interval_y = cf.Interval(-1, 10)
window = cf.WindowCoordinateSystem(800, interval_x, interval_y, "Window Name")

p0 = cf.Point(0, 0)
p1 = cf.Point(10, 10)
#window.drawLine(p0, p1, cf.Color.BLUE)

window.drawLinearEquation(-1.0, 1.0, 0, cf.Color.BLUE)
window.setWindowDisplayScale(1.5)

window.drawAxis()
window.show()
window.waitKey()

