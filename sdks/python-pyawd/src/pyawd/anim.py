class AWDSkeleton:
    def __init__(self, name=''):
        self.name = name
        

class AWDSkeletonAnimation:
    def __init__(self):
        self.frames = []

    def add_frame(self, pose):
        self.frames.append(pose)

class AWDSkeletonJoint:
    def __init__(self, name='', transform=None):
        self.name = name
        self.transform = transform
        self.__children = []
        self.__parent = None

    def remove_child_joint(self, child):
        self.__children.remove(child)
        
    def add_child_joint(self, child):
        if child.__parent is not None:
            child.__parent.remove_child_joint(child)

        child.__parent = self
        self.__children.append(child)


class AWDSkeletonPose:
    def __init__(self, name=''):
        self.name = name
        self.transforms = []

    def add_joint_transform(self, transform=None):
        self.transforms.append(transform)
        

class AWDUVAnimation:
    def __init__(self, name=''):
        self.name = name
