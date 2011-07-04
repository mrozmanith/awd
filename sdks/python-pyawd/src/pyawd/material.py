from pyawd import core

TEX_EXTERNAL = 0
TEX_EMBED_JPG = 1

class AWDTexture(core.AWDAttrElement, core.AWDBlockBase):

    def __init__(self, type=0, name='', url=None):
        super(AWDTexture, self).__init__()
        self.type = type
        self.name = name
        self.url = url

    def embed_file(self, path):
        with open(path, 'rb') as f:
            self.__data = f.read()

class AWDMaterial(core.AWDAttrElement, core.AWDBlockBase):
    COLOR = 1
    BITMAP = 2

    def __init__(self, type=0, name='', texture=None):
        super(AWDMaterial, self).__init__()
        self.type = type
        self.name = name
        self.texture = texture


