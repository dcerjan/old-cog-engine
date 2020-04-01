from vector3 import *

class Quaternion(object):

    def __init__(self, x = 1.0, y = 0.0, z = 0.0, w = 1.0):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

    def __repr__(self):
        return "<Quaternion (" + self.x + ", " + self.y + ", " + self.z + ", " + self.w + ")>"

    def __mul__(self, other):
        pass

    def slerp(self, other, amount):
        pass

    def toMatrix4(self):
        pass