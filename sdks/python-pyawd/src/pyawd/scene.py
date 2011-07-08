from pyawd import utils 
from pyawd import core


class AWDSceneBlockBase(core.AWDAttrElement, core.AWDBlockBase):
    def __init__(self, name='', transform=None):
        super(AWDSceneBlockBase, self).__init__()
        self.__children = []
        self.__parent = None
        self.name = name
        self.transform = transform

        if self.transform is None:
            self.transform = utils.AWDMatrix4x4()

    def get_parent(self):
        return self.__parent


    def set_parent(self, parent):
        if self.__parent is not None:
            self.__parent.remove_child(self)

        self.__parent = parent
        if self.__parent is not None:
            self.__parent.add_child(self)
            

    def add_child(self, child):
        if child not in self.__children:
            self.__children.append(child)
            child.set_parent(self)


    def remove_child(self, child):
        child.set_parent(None)
        while child in self.__children:
            self.__children.remove(child)


class AWDScene(AWDSceneBlockBase):
    def __init__(self, name='', transform=None):
        super(AWDScene, self).__init__(name, transform)


class AWDMeshInst(AWDSceneBlockBase):
    def __init__(self, data, name='', transform=None):
        self.mesh_data = data
        self.materials = []
        super(AWDMeshInst, self).__init__(name, transform)


class AWDContainer(AWDSceneBlockBase):
    def __init__(self, name='', transform=None):
        super(AWDContainer, self).__init__(name, transform)


CAM_FREE = 1
CAM_TARGET = 2
CAM_HOVER = 3

LENS_PERSPECTIVE = 1
LENS_ORTHOGRAPHIC = 2

class AWDCamera(AWDSceneBlockBase):
    def __init__(self, name='', transform=None, type=CAM_FREE, lens=LENS_ORTHOGRAPHIC):
        super(AWDCamera, self).__init__(name, transform)
        self.type = type
        self.lens = lens
        self.fov = 35.0


LIGHT_POINT = 1
LIGHT_DIR = 2

class AWDLight(AWDSceneBlockBase):
    def __init__(self, name='', transform=None, type=LIGHT_POINT):
        super(AWDLight, self).__init__(name, transform)
        self.type = type


PRIMITIVE_PLANE = 1
PRIMITIVE_CUBE = 2
PRIMITIVE_SPHERE = 3
PRIMITIVE_CYLINDER = 4
PRIMITIVE_CONE = 5

class AWDPrimitive(AWDSceneBlockBase):
    def __init__(self, name='', transform=None, type=PRIMITIVE_PLANE):
        super(AWDPrimitive, self).__init__(name, transform)
        self.type = type

class AWDPlanePrimitive(AWDPrimitive):
    def __init__(self, name='', transform=None):
        super(AWDPlanePrimitive, self).__init__(name, transform, PRIMITIVE_PLANE)
        self.width = None
        self.height = None
        self.segmentsW = None
        self.segmentsH = None
        self.yUp = None

class AWDCubePrimitive(AWDPrimitive):
    def __init__(self, name='', transform=None):
        super(AWDCubePrimitive, self).__init__(name, transform, PRIMITIVE_CUBE)
        self.width = None
        self.height = None
        self.depth = None
        self.segmentsW = None
        self.segmentsH = None
        self.segmentsD = None

class AWDSpherePrimitive(AWDPrimitive):
    def __init__(self, name='', transform=None):
        super(AWDSpherePrimitive, self).__init__(name, transform, PRIMITIVE_SPHERE)
        self.radius = None
        self.segmentsW = None
        self.segmentsH = None

class AWDCylinderPrimitive(AWDPrimitive):
    def __init__(self, name='', transform=None):
        super(AWDCylinderPrimitive, self).__init__(name, transform, PRIMITIVE_CYLINDER)
        self.radius = None
        self.topRadius = None
        self.bottomRadius = None
        self.height = None
        self.segmentsW = None
        self.segmentsH = None

class AWDConePrimitive(AWDPrimitive):
    def __init__(self, name='', transform=None):
        super(AWDConePrimitive, self).__init__(name, transform, PRIMITIVE_CONE)
        self.radius = None
        self.segmentsW = None
        self.segmentsH = None
    
    
