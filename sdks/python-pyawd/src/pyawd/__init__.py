__all__ = [ 'core', 'anim', 'geom', 'material', 'scene', 'utils' ]

version = ('PyAWD', 2, 0, 0, 'a')

try:
    import pyawd.cio as io

    # Ignore release letter for final releases
    release = chr(io.LIBAWD_VERSION_RELEASE)
    if release == 'f':
        release = ''

    backend = ('libawd',
        io.LIBAWD_VERSION_MAJOR,
        io.LIBAWD_VERSION_MINOR,
        io.LIBAWD_VERSION_BUILD,
        release)

    backend_str = '%s v%d.%d.%d%s' % backend

except:
    backend = ('python', None, None)
    backend_str = 'python'

version_str = '%s v%d.%d.%d%s (%s)' % (
    version[0], version[1], version[2], version[3], version[4], backend_str)

