from pyawd import core

class AWDTexture(core.AWDBlockBase):
    EXTERNAL = 0

    def __init__(self, type=0, name='', url=None):
        self.type = type
        self.name = name
        self.url = url

class AWDMaterial(core.AWDBlockBase):
    COLOR = 1
    BITMAP = 2

    def __init__(self, type=0, name='', texture=None):
        self.type = type
        self.name = name
        self.texture = texture


