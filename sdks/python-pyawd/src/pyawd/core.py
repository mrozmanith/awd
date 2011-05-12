from pyawd import io

class AWD:
    UNCOMPRESSED = 0
    DEFLATE = 1
    LZMA = 2

    def __init__(self, flags=0, compression=0):
        self.compression = compression
        self.flags = flags

        self.mesh_inst_blocks = []
        self.mesh_data_blocks = []

    def flush(self, file):
        writer = io.AWDWriter()
        writer.write(self, file)
        
    def add_texture(self, block):
        pass
    def add_material(self, block):
        pass
    def add_mesh_data(self, block):
        self.mesh_data_blocks.append(block)

    def add_mesh_inst(self, block):
        self.mesh_inst_blocks.append(block)

    def add_uv_anim(self, blocks):
        pass


