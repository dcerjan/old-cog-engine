##################################
#  Cog.Vector2 Native Interface  #
##################################
import libcog

class Vector2:

    __x = 0.0
    __y = 0.0

    def __init__(self, x = 0.0, y = 0.0):
        self.__x = x
        self.__y = y

    def __add__(self, other):
        self.__x += other.x
        self.__y += other.y

    def __sub__(self, other):
        self.__x -= other.x
        self.__y -= other.y

    def __repr__(self):
        return "<Cog.Vector2 Object (" + self.__x + ", " + self.__y + ")>"

    def x(self):
        return self.__x

    def y(self):
        return self.__y

    def length(self):
        return libcog.math_vector2_length( self.__x, self.__y )

    def dot(self, other):
        return libcog.math_vector2_dot( self.x(), self.y(), other.x(), other.y() )

    def cross(self):
        self.__x, self.__y = -self.__y, self.__x

help(libcog)

vec = Vector2(1, 3)
bla = Vector2(3, 7)


print vec.length()
print vec.dot(bla)
