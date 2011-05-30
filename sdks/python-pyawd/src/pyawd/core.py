import struct

class AWDBlockBase(object):
    def __init__(self, type):
        self.__type = type

    def write_block(self, file, addr):
        file.write(struct.pack('>IBBI', addr, 0, self.__type, 0))

    def write_body(self, file):
        pass # To be overridden


class AWD(object):
    UNCOMPRESSED = 0
    DEFLATE = 1
    LZMA = 2

    def __init__(self, flags=0, compression=0):
        self.compression = compression
        self.flags = flags

        self.texture_blocks = []
        self.material_blocks = []
        self.mesh_data_blocks = []
        self.uvanim_blocks = []
        self.skelanim_blocks = []
        self.skelpose_blocks = []
        self.skeleton_blocks = []
        self.scene_blocks = []

    def flush(self, file):
        try:
            from pyawd import cio as io
        except:
            from pyawd import pyio as io

        writer = io.AWDWriter()
        writer.write(self, file)
        
    def add_texture(self, block):
        self.texture_blocks.append(block)

    def add_material(self, block):
        self.material_blocks.append(block)

    def add_mesh_data(self, block):
        self.mesh_data_blocks.append(block)

    def add_scene_block(self, block):
        self.scene_blocks.append(block)

    def add_uv_anim(self, block):
        self.uvanim_blocks.append(block)

    def add_skeleton(self, block):
        self.skeleton_blocks.append(block)

    def add_skeleton_pose(self, block):
        self.skelpose_blocks.append(block)

    def add_skeleton_anim(self, block):
        self.skelanim_blocks.append(block)


