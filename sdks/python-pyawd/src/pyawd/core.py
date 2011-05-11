class AWD:
    UNCOMPRESSED = 0
    DEFLATE = 1
    LZMA = 2

    def __init__(self, flags=0, compression=0):
        self.compression = compression
        self.flags = flags

    def flush(self, file):
        pass
        
    def add_texture(self, block):
        pass
    def add_material(self, block):
        pass
    def add_mesh_data(self, block):
        pass
    def add_mesh_inst(self, block):
        pass
    def add_uv_anim(self, blocks):
        pass


