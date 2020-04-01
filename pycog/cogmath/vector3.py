class Vector3(object):
    """ Vector3 class """

    def __init__(self, x = 0.0, y = 0.0, z = 0.0):
        """ Vector3 constructor """
        self.x = x
        self.y = y
        self.z = z

    def __add__(self, other):
        """ Vector3 addition """
        pass

    def __sub__(self, other):
        """ Vector3 subtraction """
        pass

    def __mul__(self, other):
        """ Vector3 cross product """
        pass

    @staticmethod
    def Lerp(src, dst, factor):
        """ Vector3 linear interpolation from source to destination """
        pass

    @staticmethod
    def Zero():
        """ Vector3 Zero-Vector """
        return Vector3(0.0, 0.0, 0.0)

    @staticmethod
    def Up():
        """ Vector3 Up-Vector """
        return Vector3(0.0, 1.0, 0.0)

    @staticmethod
    def Forward():
        """ Vector3 Forward-Vector """
        return Vector3(0.0, 0.0, -1.0)

    @staticmethod
    def Right():
        """ Vector3 Right-Vector """
        return Vector3(1.0, 0.0, 0.0)