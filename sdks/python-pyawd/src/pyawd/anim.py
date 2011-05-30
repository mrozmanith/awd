import utils 
import core

class AWDSkeleton(core.AWDBlockBase):
    def __init__(self, name=''):
        self.name = name
        self.root_joint = None
        

class AWDSkeletonAnimation(core.AWDBlockBase):
    def __init__(self, name=''):
        self.name = name
        self.frames = []

    def add_frame(self, pose):
        self.frames.append(pose)


class AWDSkeletonJoint(object):
    def __init__(self, name='', inv_bind_mtx=None):
        self.name = name
        self.inv_bind_mtx = inv_bind_mtx

        self.__children = []
        self.__parent = None

        if self.inv_bind_mtx is None:
            self.inv_bind_mtx = utils.AWDMatrix4x4()

    def remove_child_joint(self, child):
        self.__children.remove(child)
        
    def add_child_joint(self, child):
        if child.__parent is not None:
            child.__parent.remove_child_joint(child)

        child.__parent = self
        self.__children.append(child)


class AWDSkeletonPose(core.AWDBlockBase):
    def __init__(self, name=''):
        self.name = name
        self.transforms = []

    def add_joint_transform(self, transform=None):
        self.transforms.append(transform)
        

class AWDUVAnimation(core.AWDBlockBase):
    def __init__(self, name=''):
        self.name = name
        self.frames = []
