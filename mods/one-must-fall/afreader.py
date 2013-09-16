#!/usr/bin/env python

# Reads One Must Fall 2097 figher files: fightr0.af

verbose_debug = 0
def debug(str, verbose):
    if verbose_debug >= verbose:
        print str

def num_lsb(stuff):
    t = 0
    for i in range(0,len(stuff)):
        t += ord(stuff[i]) << (8 * i)
    return t

def num_msb(stuff):
    t = 0
    for i in range(0,len(stuff)):
        t = (t<<8) + ord(stuff[i])
    return t

def read_word(file):
    return file.read(2)

def read_dword(file):
    return file.read(4)

# convert unsigned to signed
def twos_complement(number, bytes):
    return number - (1 << (bytes * 8))

def read_header(file):
    number = num_lsb(read_word(file))
    print "Fighter number %d" % number
    print "Signature: %s" % [ord(x) for x in file.read(3)]
    print "Endurance: %d" % num_lsb(file.read(3))
    print "???: %d" % num_lsb(file.read(1))
    print "Power: %d" % num_lsb(read_word(file))
    print "Forward movement: %d" % num_lsb(read_dword(file))
    print "Backward movement: %d" % num_lsb(read_dword(file))
    print "Up movement: %d" % twos_complement(num_lsb(read_dword(file)), 4)
    print "Down movement: %d" % num_lsb(read_dword(file))
    print "End ???: %s" % [ord(x) for x in read_word(file)]

def as_words(data):
    for i in range(0, len(data)/2):
        yield num_lsb(data[i*2:i*2+1])

def read_palette(path):
    file = open(path, 'r')
    data = file.read()
    file.close()
    out = []
    for i in range(0, 256):
        out.extend((ord(data[i*3]), ord(data[i*3+1]), ord(data[i*3+2])))
    return out

class Sprite:
    def __init__(self, index, frame, width, height, data, debugit = False):
        self.index = index
        self.frame = frame
        debug(" Read sprite frame %d index %d data length %d" % (frame, index, len(data)), debugit == False)
        self.read(width, height, data, debugit == False)

    def save(self, dir):
        self.sprite.save('%s/sprite-frame-%d-index-%d.png' % (dir, self.frame, self.index))

    def read(self, width, height, data, verbose = 1):
        import Image
        self.sprite = Image.new("P", (width, height))
        self.palette = read_palette('palettes/xac')
        if False:
            palette = []
            # crappy default palette
            for i in range(0, 15):
                palette.extend((i*256/15, 0, 0))
            for i in range(0, 15):
                palette.extend((0, i*256/15, 0))
            for i in range(0, 15):
                palette.extend((0, 0, i*256/15))
            while len(palette) < 256:
                palette.extend((0,0,0))
            #for i in range(256):
            #    palette.extend((i, i, i))
        self.sprite.putpalette(self.palette)

        class Environment:
            pass
        environment = Environment()
        environment.x = 0
        environment.y = 0
        cursor = [0]
        #x = 0
        #y = 0

        def read_sprite_word():
            out = num_lsb(data[cursor[0]:cursor[0]+2])
            cursor[0] += 2
            return out

        def read_sprite_bytes(num):
            debug("  Read bytes %d" % num, verbose)
            if cursor[0] + num > len(data):
                raise Exception("Need more data than available")
            out = data[cursor[0]:cursor[0]+num]
            cursor[0] += num
            return out

        def setXLength(environment, length):
            environment.x = length

        def setYLength(environment, length):
            environment.y = length

        def readPixels(environment, length):
            pixels = read_sprite_bytes(length)
            x = environment.x
            y = environment.y
            debug("  Pixels %s" % [ord(q) for q in pixels], verbose)

            # This loop is kind of slow, optimize it
            for start_x in range(x, x + length):
                if start_x >= width or y >= height:
                    print "Putpixel out of range %d, %d" % (start_x, y)
                else:
                    color = num_lsb(pixels[start_x - x])
                    # print "Putpixel %s at (%d,%d)" % (color, start_x, y)
                    self.sprite.putpixel((start_x, y), color)
            # print "   Draw at (%d, %d) %d bytes %s" % (x, y, length, [ord(z) for z in pixels])
            environment.x = 0

        operators = {0 : setXLength,
                     1 : readPixels,
                     2 : setYLength,
                     3 : lambda env, data: 0}

        while cursor[0] < len(data):
            word = read_sprite_word()
            length = word / 4
            kind = word % 4
            debug("   Word %d Kind %d length %d" % (word, kind, length), verbose)
            operators[kind](environment, length)

class Animation:
    def __init__(self, motion, file):
        self.motion = motion
        self.sprites = []
        self.read(file)

    def save(self, dir):
        for sprite in self.sprites:
            sprite.save(dir)

    def read(self, file, verbose = 1):
        debug(" Zero padding: %s" % [ord(x) for x in file.read(8)], verbose)
        overlays = num_lsb(read_word(file))
        debug(" Overlay: %d" % overlays, verbose)
        frames = num_lsb(file.read(1))
        debug(" Frames: %d" % frames, verbose)
        if overlays != 0:
            debug(" Overlay table: %s" % [ord(x) for x in file.read(4 * overlays)], verbose)
        string_length = num_lsb(read_word(file))
        debug(" String length %d" % string_length, verbose)
        string = file.read(string_length)
        debug(" String %s" % string, verbose)
        debug(" Zero %d" % num_lsb(file.read(1)), verbose)
        extra_strings = num_lsb(file.read(1))
        debug(" Extra strings %d" % extra_strings, verbose)
        for i in range(0, extra_strings):
            length = num_lsb(read_word(file))
            debug(" Extra length %d" % length, verbose)
            stuff = file.read(length)
            debug(" Extra string %s" % stuff, verbose)
            debug(" Zero %d" % num_lsb(file.read(1)), verbose)
        for frame in range(0, frames):
            debug(" Frame %d" % frame, verbose)
            sprite_length = num_lsb(read_word(file))
            debug("  Sprite length %d" % sprite_length, verbose)
            debug("  X offset %d" % twos_complement(num_lsb(read_word(file)), 2), verbose)
            debug("  Y offset %d" % twos_complement(num_lsb(read_word(file)), 2), verbose)
            width = num_lsb(read_word(file))
            debug("  Sprite width %d" % width, verbose)
            height = num_lsb(read_word(file))
            debug("  Sprite height %d" % height, verbose)
            index = num_lsb(file.read(1))
            debug("  Index %d" % index, verbose)
            exists = num_lsb(file.read(1))
            debug("  Picture exists %d" % exists, verbose)
            if exists == 0:
                data = file.read(sprite_length)
                # sprite = read_sprite(width, height, data)
                # sprite = Sprite(index, frame, 300, 300, data, self.motion == 7)
                sprite = Sprite(index, frame, 300, 300, data)
                self.sprites.append(sprite)
                # sprite.save('%s/sprite-%d.png' % (dir, frame))
        unknown = file.read(21)
        debug(" Movement string: %s" % file.read(21), verbose)
        footer = num_lsb(read_word(file))
        debug(" Footer length %d" % footer, verbose)
        footer_string = file.read(footer)
        debug(" Footer string %s" % footer_string, verbose)

def read_animation(file, verbose = 0):
    while True:
        motion = num_lsb(file.read(1))
        if motion == 250:
            # print "End of animations"
            break
        print "New animation: %d" % motion
        # print " Motion %d" % motion
        yield Animation(motion, file)

def read(path):
    print "Reading file " + path
    file = open(path, 'r')
    read_header(file)
    for animation in read_animation(file, 1):
        import os
        dir = 'out/%s/animation-%d' % (path.replace('/', '-'), animation.motion)
        try:
            os.makedirs(dir)
        except OSError:
            pass

        animation.save(dir)

    file.close()

import sys
if len(sys.argv) < 2:
    print "Give an argument"
else:
    for arg in sys.argv[1:]:
        read(arg)
