from pyawd import io

class AWD:
    UNCOMPRESSED = 0
    DEFLATE = 1
    LZMA = 2

    def __init__(self, flags=0, compression=0):
        self.compression = compression
        self.flags = flags

        self.texture_blocks = []
        self.material_blocks = []
        self.mesh_inst_blocks = []
        self.mesh_data_blocks = []
        self.uvanim_blocks = []
        self.skelanim_blocks = []
        self.skelpose_blocks = []
        self.skeleton_blocks = []

    def flush(self, file):
        writer = io.AWDWriter()
        writer.write(self, file)
        
    def add_texture(self, block):
        self.texture_blocks.append(block)

    def add_material(self, block):
        self.material_blocks.append(block)

    def add_mesh_data(self, block):
        self.mesh_data_blocks.append(block)

    def add_mesh_inst(self, block):
        self.mesh_inst_blocks.append(block)

    def add_uv_anim(self, block):
        self.uvanim_blocks.append(block)

    def add_skeleton(self, block):
        self.skeleton_blocks.append(block)

    def add_skeleton_pose(self, block):
        self.skelpose_blocks.append(block)

    def add_skeleton_anim(self, block):
        self.skelanim_blocks.append(block)


