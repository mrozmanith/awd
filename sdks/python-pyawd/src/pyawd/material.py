class AWDTexture:
    EXTERNAL = 0

    def __init__(self, type=0, name='', url=None):
        self.type = type
        self.name = name
        self.url = url

class AWDMaterial:
    COLOR = 1
    BITMAP = 2

    def __init__(self, type=0, name='', texture=None):
        self.type = type
        self.name = name
        self.texture = texture


