# -*- coding: utf-8 -*-
"""
Simple examples demonstrating the use of GLMeshItem.

"""


from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph as pg
import pyqtgraph.opengl as gl
import re
import math
import sys

app = QtGui.QApplication([])
w = gl.GLViewWidget()
w.show()
w.setWindowTitle('pyqtgraph example: GLMeshItem')
w.setCameraPosition(distance=40)

g = gl.GLGridItem()
g.scale(0.5,0.5,1)
w.addItem(g)

import numpy as np

def GLMeshBox(length, width, height, color):
	verts = np.array([
		[-length/2, -width/2, height],
		[-length/2, -width/2, 0],
		[-length/2, width/2, height],
		[-length/2, width/2, 0],
		[length/2, width/2, height],
		[length/2, width/2, 0],
		[length/2, -width/2, height],
		[length/2, -width/2, 0],
	])
	faces = np.array([
		[1, 2, 0],
		[1, 2, 3],
		[2, 5, 3],
		[2, 5, 6],
		[4, 7, 5],
		[4, 7, 6],
		[0, 7, 6],
		[0, 7, 1],
		[0, 4, 2],
		[0, 4, 6],
		[1, 5, 3],
		[1, 5, 7],
	])
	box = gl.GLMeshItem(vertexes=verts, faces=faces, smooth=False)
	box.setColor(color)
	return box

## Example 1:
## Array of vertex positions and array of vertex indexes defining faces
## Colors are specified per-face



## Mesh item will automatically compute face normals.
#m1 = gl.GLMeshItem(vertexes=verts, faces=faces, faceColors=colors, smooth=False)
m1 = GLMeshBox(0.44, 0.01, 0.01, (0.7, 0.7,0.7,1))
m2 = GLMeshBox(0.01, 0.44, 0.01, (0.7, 0.7,0.7,1))
prop1 = gl.GLMeshItem(meshdata=gl.MeshData.cylinder(rows=2,cols=20, radius=[0.01,0.127], length = 0.01), smooth = False)
prop2 = gl.GLMeshItem(meshdata=gl.MeshData.cylinder(rows=2,cols=20, radius=[0.01,0.127], length = 0.01), smooth = False)
prop3 = gl.GLMeshItem(meshdata=gl.MeshData.cylinder(rows=2,cols=20, radius=[0.01,0.127], length = 0.01), smooth = False)
prop4 = gl.GLMeshItem(meshdata=gl.MeshData.cylinder(rows=2,cols=20, radius=[0.01,0.127], length = 0.01), smooth = False)
m2.setParentItem(m1)
prop1.translate(-0.2, 0, 0.01)
prop2.translate(0, 0.2, 0.01)
prop3.translate(0.2, 0, 0.01)
prop4.translate(0, -0.2, 0.01)
prop1.setColor((1,0,0,1))
prop2.setColor((0,1,0,1))
prop3.setColor((0,1,0,1))
prop4.setColor((0,1,0,1))
prop1.setParentItem(m1)
prop2.setParentItem(m1)
prop3.setParentItem(m1)
prop4.setParentItem(m1)
heightline = gl.GLLinePlotItem(pos=np.array([[1, 2, 0],[1, 2, 3]]), color = (0,0,1,1), width=0.1)
#m1.translate(5, 5, 0)
m1.setGLOptions('opaque')
w.addItem(m1)
w.addItem(heightline)

f = open('/tmp/fifoCopter', 'r')

posX = 0
posY = 0
posZ = 0
angX = 0
angY = 0
angZ = 0

def update():
	global posX
	global posY
	global posZ
	global angX
	global angY
	global angZ
#	col = color.red
#	if(motorState==1):
#		col = color.green
#	copter = box(pos=(posY,-posZ,-posX), length = 5, height = 1, width = 10, color=col)
#	copter.rotate(angle=angX, axis=(0,0,-1))
#	copter.rotate(angle=angY, axis=(1,0,0))
#	copter.rotate(angle=angZ, axis=(0,-1,0))
	line = f.readline()
#	print line
#	print line
	if(len(re.findall('CState', line))>0):
		print line
		values = re.findall(r'[0-9-.]+', line)
		posX = (float)(values[0])
		posY = (float)(values[1])
		posZ = (float)(values[2])
		angX = (float)(values[3])
		angY = (float)(values[4])
		angZ = (float)(values[5])
		motorState = (int)(values[6])
#		print 'Position X:', (float)(values[0])
#		print 'Position Y:', (float)(values[1])
#		print 'Position Z:', (float)(values[2])
#		print 'Yaw:', (float)(values[5])
#	copter.visible = False
#	del copter

		if(motorState):
			prop1.setColor((1,0,0,1))
			prop2.setColor((0,1,0,1))
			prop3.setColor((0,1,0,1))
			prop4.setColor((0,1,0,1))
		else:
			prop1.setColor((0.1,0,0,1))
			prop2.setColor((0,0.1,0,1))
			prop3.setColor((0,0.1,0,1))
			prop4.setColor((0,0.1,0,1))

		m1.resetTransform()
		m1.rotate(angX*180/math.pi, -1, 0, 0, False)
		m1.rotate(angY*180/math.pi, 0, 1, 0, False)

		m1.rotate(angZ*180/math.pi, 0, 0, -1, False)
		m1.translate(-posX,posY,-posZ)
		heightline.setData(pos=np.array([[-posX,posY,0],[-posX,posY,-posZ]]))
		m1.update()

t = QtCore.QTimer()
t.timeout.connect(update)
t.start(20)
    


## Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()

