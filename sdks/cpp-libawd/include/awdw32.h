#ifndef _AWDW32_H
#define _AWDW32_H

#include <io.h>
#include <windows.h>
#include <sys/stat.h>
#include <fcntl.h>

#define open _open
#define lseek _lseek
#define write _write
#define read _read
#define close _close
#define unlink _unlink

#define off_t int

#endif