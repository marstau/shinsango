from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *
from PyQt4.QtGui import *

class ResizeImage(QLabel):
    def __init__(self, arg):
        QLabel.__init__(self, arg)
        self.original = None

    def scale(self, image):
        keepAspectRatio = QtCore.Qt.KeepAspectRatio
        return QPixmap.fromImage(image).scaled(self.width(), self.height(), keepAspectRatio)

    def setPixmap(self, image):
        self.original = image
        QLabel.setPixmap(self, self.scale(image))

    def minimumSizeHint(self):
        if self.original != None:
            return QSize(self.original.width(), self.original.height())
        return QSize(0, 0)

    def getPixel(self, x, y):
        return self.pixmap().toImage().pixel(x, y)

    def getPixelIndex(self, x, y):
        px = x / (self.pixmap().width() * 1.0 / self.original.width())
        py = y / (self.pixmap().height() * 1.0 / self.original.height())
        #print "Get original pixel %s, %s at %s, %s" % (x, y, px, py)
        #print "  %s,%s -> %s,%s" % (self.original.width(), self.original.height(), self.pixmap().width(), self.pixmap().height())
        return self.original.pixelIndex(int(px), int(py))

    def resizeEvent(self, resize):
        if self.original != None:
            QLabel.setPixmap(self, self.scale(self.original))
