#!/usr/bin/env python

from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *
from PyQt4.QtGui import *

class Signals:
    pass

Signals.Triggered = "triggered()"
Signals.Close = "close()"
Signals.Clicked = "clicked()"

class Color(QtGui.QPixmap):
    def __init__(self, value):
        QtGui.QPixmap.__init__(self, 20, 20)
        #self.color = QtGui.QColor.fromHsv(value, 255, 255)
        # print value
        self.value = value
        self.fill(QtGui.QColor.fromRgb(value))

    def rotateHue(self, much):
        (h, s, v, a) = self.getColor().toHsv().getHsv()
        # print "Old hue %s %s" % (h, self.getColor().rgb())
        new = QtGui.QColor.fromHsv((h + much) % 360, s, v)
        self.setColor(new.toRgb())
        # self.setColor(QtGui.QColor.fromRgb(255, 255, 255))
        # print "New hue %s %s" % (self.getColor().toHsv().hue(), self.getColor().rgb())

    def setColor(self, color):
        self.value = color.rgb()
        self.fill(color)

    def getColor(self):
        return QtGui.QColor.fromRgb(self.value)

class BorderWidget(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        self.setLayout(QtGui.QGridLayout())
        self.layout().setMargin(4)
        self.layout().setSpacing(1)
        self.highlight = False

    def setHighlight(self, what):
        self.highlight = what

    def paintEvent(self, event):
        if self.highlight:
            painter = QtGui.QPainter(self)
            #QtGui.QLabel.paintEvent(label, event)
            # painter.setPen(QtGui.QColor.blue)
            # painter.setPen(QtGui.QPen(QtCore.Qt.black, 1, QtCore.Qt.SolidLine))
            painter.fillRect(0, 0, self.width() - 1, self.height() - 1, QtGui.QBrush(QtCore.Qt.black))
            # painter.setPen(QtGui.QPen(QtCore.Qt.green, 1, QtCore.Qt.SolidLine))
            painter.fillRect(1, 1, self.width() - 3, self.height() - 3, QtGui.QBrush(QtCore.Qt.green))

def border(widget):
    frame = BorderWidget()
    frame.layout().addWidget(widget)
    return frame

def property(name, args):
    def isv4_4():
        import re
        v4_re = re.compile("4\.4.*")
        return v4_re.match(QtCore.PYQT_VERSION_STR) != None
    if isv4_4():
        return "%s" % name
    else:
        return "%s(%s)" % (name, ("Object," * args)[0:-1])

class Palette(QWidget):
    colorChanged = property("colorchanged", 1)
    def __init__(self, colors):
        QWidget.__init__(self)
        self.setLayout(QtGui.QGridLayout())
        self.colors = {}
        self.boxes = {}
        self.populate(colors)
        self.highlighted = None

    def boxIndex(self, index):
        return self.boxes[index]

    def doHighlight(self, box):
        if self.highlighted != None:
            self.highlighted.setHighlight(False)
            self.highlighted.update()
        box.setHighlight(True)
        box.update()
        self.highlighted = box

    def palette(self):
        # convert rgb color back to 8-bit number
        def convert(x):
            # return (x & 0xffffff) - 0x1000000
            # return x & 0xffffff
            return x
        return [convert(self.colors[index].getColor().rgb()) for index in sorted(self.colors.keys())]

    def color(self, index):
        return self.colors[index]

    def highlight(self, index):
        self.doHighlight(self.boxIndex(index))
        #if self.highlighted != None:
        #    self.highlighted.setFrameStyle(QtGui.QFrame.Panel | QtGui.QFrame.Sunken)
        #label = self.color(index)
        #label.setFrameStyle(QtGui.QFrame.Panel | QtGui.QFrame.Raised)
        #self.highlighted = label

    def populate(self, colors):
        def add(what, x, y):
            self.layout().addWidget(what, y, x)
            self.boxes[y*16 + x] = what

        x = 0
        y = 0
        for color in colors:
            def make(x, y):
                index = color
                label = QtGui.QLabel()
                realColor = Color(color)
                # print "Color %s at %s" % (index, y*16 + x)
                self.colors[y*16 + x] = realColor
                label.setPixmap(realColor)
                def do_update_pixmap():
                    label.setPixmap(realColor)
                realColor.updatePixmap = do_update_pixmap
                #label.setFrameStyle(QtGui.QFrame.Panel | QtGui.QFrame.Sunken)
                #label.setLineWidth(4)
                box = border(label)
                def editColors(*args):
                    start = realColor.getColor()
                    mine = QtGui.QColorDialog.getColor(start, self)
                    realColor.setColor(mine)
                    # print "Set color to %s - %s original %s" % (mine, mine.rgb(), start.rgb())
                    # Q: Why do I have to reset the pixmap? Does QLabel cache
                    # the pixmap when you set it?
                    label.setPixmap(realColor)
                    self.emit(SIGNAL(Palette.colorChanged), self.palette())
                    # self.emit(SIGNAL(Palette.colorChanged))

                    # I don't seem to need to update things.. just setting
                    # the pixmap again redraws the label.
                    #label.update()
                    #box.update()

                def do_click(*args):
                    self.doHighlight(box)
                label.mousePressEvent = do_click
                label.mouseDoubleClickEvent = editColors
                return box
            add(make(x, y), x, y)
            x += 1
            if x > 15:
                x = 0
                y += 1

def run(app):
    # Set the look and feel
    QtGui.QApplication.setStyle(QtGui.QStyleFactory.create("Cleanlooks"))
    main = uic.loadUi("main.ui")

    #def open_palette(colors):
    #    palette = uic.loadUi("palette.ui")
    #    main.layoutx.addWidget(palette)
    #    populate_palette(palette.layout, colors)

    # main.connect(main.actionOpen_raw_palette, SIGNAL(Signals.Triggered), open_palette)

    def open_picture():
        file = str(QtGui.QFileDialog.getOpenFileName())
        image = QtGui.QImage(file)
        # print "Image depth is %s" % image.depth()

        def updatePicture():
            # print "label %s, %s" % (main.picture.width(), main.picture.height())
            # show = QtGui.QPixmap.fromImage(image).scaled(main.picture.width(), main.picture.height())
            #show = QtGui.QPixmap.fromImage(image)
            #print "Show depth is %s" % show.depth()
            main.picture.setPixmap(image)

        
        updatePicture()
        # main.connect(main.picture, SIGNAL(Signals.Clicked), getPixel)
        # print image.colorTable()
        #print "0 is %s %s" % (image.color(0), image.color(0) & 0xffffff)
        # open_palette([x & 0xffffff for x in image.colorTable()])
        def nq(x):
            # return int((x & 0xffffff) - 0x1000000)
            # return x & 0xffffff
            return x
        colors = [nq(x) for x in image.colorTable()]
        palette = Palette(colors)
        paletteWidget = uic.loadUi("palette.ui")
        main.palette.layout().addWidget(paletteWidget)
        paletteWidget.scrolly.layout().addWidget(palette)
        # populate_palette(palette.scrolly.layout(), colors)

        def changed(newPalette):
            # print "New palette is %s" % ["%x" % x for x in newPalette]
            image.setColorTable(newPalette)
            updatePicture()

        # print "Palette is %s" % ["%x" % x for x in image.colorTable()]
        main.connect(palette, SIGNAL(Palette.colorChanged), changed)

        def do_change_colors(indicies):
            for index in indicies:
                # print "Rotate color %s" % index
                color = palette.color(index)
                color.rotateHue(15)
                color.updatePixmap()
                palette.boxIndex(index).update()
            image.setColorTable(palette.palette())
            updatePicture()

        def color1(*args):
            do_change_colors([16*0 + i for i in xrange(0, 16)])

        def color2(*args):
            do_change_colors([16*1 + i for i in xrange(0, 16)])

        def color3(*args):
            do_change_colors([16*2 + i for i in xrange(0, 16)])

        main.connect(paletteWidget.color1, SIGNAL(Signals.Clicked), color1)
        main.connect(paletteWidget.color2, SIGNAL(Signals.Clicked), color2)
        main.connect(paletteWidget.color3, SIGNAL(Signals.Clicked), color3)

        def pressMouse(*args):
            def work(object, event):
                index = main.picture.getPixelIndex(event.x(), event.y())
                color = palette.boxIndex(index)
                palette.highlight(index)
                paletteWidget.scrollArea.ensureWidgetVisible(color)

            # work around pyqt 4.4 / 4.6 incompatibility
            def v4_4(event):
                work(None, event)
            def v4_6(object, event):
                work(object, event)
            
            if len(args) == 1:
                apply(v4_4, args)
            else:
                apply(v4_6, args)

            # print "color %s" % color
            # print "clicked %s at %s, %s = %s, %s, %s" % (event, event.x(), event.y(), QtGui.qRed(pixel), QtGui.qGreen(pixel), QtGui.qBlue(pixel))
            # print "clicked %s at %s, %s = %s" % (event, event.x(), event.y(), pixel)

        main.picture.mousePressEvent = pressMouse

    main.connect(main.actionQuit, SIGNAL(Signals.Triggered), SLOT(Signals.Close))
    main.actionQuit.setShortcut('Ctrl+Q')
    main.connect(main.actionOpen, SIGNAL(Signals.Triggered), open_picture)
    main.actionOpen.setShortcut('Ctrl+O')

    screen = QtGui.QDesktopWidget().screenGeometry()
    size = main.geometry()
    main.move((screen.width()-size.width())/2, (screen.height()-size.height())/2)

    main.show()
    app.exec_()

import sys
print "Running"
run(QtGui.QApplication(sys.argv))
