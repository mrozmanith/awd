class AWDSubMesh:
    VERTICES = 1
    TRIANGLES = 2
    UVS = 3
    VERTEX_NORMALS = 4
    VERTEX_TANGENTS = 5
    FACE_NORMALS = 6
    JOINT_INDICES = 7
    JOINT_WEIGHTS = 8


    def __init__(self):
        self.__data_streams = []

    def add_stream(self, type, data):
        self.__data_streams.append((type,data))


class AWDMeshData:
    def __init__(self, name=''):
        self.name = name
        self.__sub_meshes = []

    def add_sub_mesh(self, sub):
        self.__sub_meshes.append(sub)

    def __len__(self):
        return len(self.__sub_meshes)

    def __getitem__(self, index):
        return self.__sub_meshes[index]

