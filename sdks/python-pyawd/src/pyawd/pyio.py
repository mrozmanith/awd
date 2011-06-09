class AWDWriter(object):
    def write(self, awd, file):
        def write_blocks(blocks):
            for b in blocks:
                b.write_block(file)
        
        write_blocks(awd.texture_blocks)
        write_blocks(awd.material_blocks)
        write_blocks(awd.mesh_data_blocks)
        write_blocks(awd.uvanim_blocks)
        write_blocks(awd.skelanim_blocks)
        write_blocks(awd.skelpose_blocks)
        write_blocks(awd.skeleton_blocks)
        write_blocks(awd.scene_blocks)

