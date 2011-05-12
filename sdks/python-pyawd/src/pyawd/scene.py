class AWDMeshInst:
    def __init__(self, data, name='', transform=None):
        self.name = name
        self.mesh_data = data
        self.materials = []
        self.transform = transform
