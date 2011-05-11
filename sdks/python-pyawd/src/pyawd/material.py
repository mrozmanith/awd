class AWDTexture:
    EXTERNAL = 0

    def __init__(self, type, name):
        self.type = type
        self.name = name

class AWDMaterial:
    COLOR = 1
    BITMAP = 2

    def __init__(self, type, name=''):
        self.textures = []

    def add_texture(self, texture):
        self.textures.append(texture)
        

