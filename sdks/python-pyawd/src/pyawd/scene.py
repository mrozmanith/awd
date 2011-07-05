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

