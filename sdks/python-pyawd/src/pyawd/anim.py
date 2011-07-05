from pyawd import utils 
from pyawd import core

class GenericAnimFrame(object):
    def __init__(self, data=None, duration=0):
        self.duration = duration
        self.data = data


class GenericAnim(object):
    def __init__(self, frames):
        super(GenericAnim, self).__init__()
        self.__frames = frames

    def __len__(self):
        return len(self.__frames)

    def __getitem__(self, key):
        idx = int(key)
        return self.__frames[idx]

    def __setitem__(self, key, val):
        idx = int(key)
        if isinstance(val, GenericAnimFrame):
            self.__frames[idx] = val
        else:
            raise ValueError('value must be GenericAnimFrame instance')

    def __contains__(self, item):
        return item in self.__frames


class AWDSkeleton(core.AWDBlockBase, core.AWDAttrElement):
    def __init__(self, name=''):
        super(AWDSkeleton, self).__init__()
        self.name = name
        self.root_joint = None
        

class AWDSkeletonAnimation(GenericAnim, core.AWDAttrElement, core.AWDBlockBase):
    def __init__(self, name=''):
        self.name = name
        self.__frames = []
        super(AWDSkeletonAnimation, self).__init__(self.__frames)

    def add_frame(self, pose, duration):
        dur = int(duration)
        self.__frames.append(GenericAnimFrame(data=pose, duration=dur))


class AWDSkeletonJoint(core.AWDAttrElement):
    def __init__(self, name='', inv_bind_mtx=None):
        super(AWDSkeletonJoint, self).__init__()
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


class AWDSkeletonPose(core.AWDBlockBase, core.AWDAttrElement):
    def __init__(self, name=''):
        super(AWDSkeletonPose, self).__init__()
        self.name = name
        self.transforms = []

    def add_joint_transform(self, transform=None):
        self.transforms.append(transform)
        

class AWDUVAnimation(GenericAnim, core.AWDAttrElement, core.AWDBlockBase):
    def __init__(self, name=''):
        self.name = name
        self.__frames = []
        super(AWDUVAnimation, self).__init__(self.__frames)

    def add_frame(self, transform, duration):
        dur = int(duration)
        self.__frames.append(GenericAnimFrame(data=transform, duration=dur))



