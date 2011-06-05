__all__ = [ 'core', 'anim', 'geom', 'material', 'scene', 'utils' ]

version = ('PyAWD', 0, 1, 'a')

try:
    import pyawd.cio as io

    backend = ('libawd',
        io.LIBAWD_MAJOR_VERSION,
        io.LIBAWD_MINOR_VERSION)
    backend_str = '%s v%d.%d' % backend

except:
    backend = ('python', None, None)
    backend_str = 'python'

version_str = '%s v%d.%d%s (%s)' % (
    version[0], version[1], version[2], version[3], backend_str)

