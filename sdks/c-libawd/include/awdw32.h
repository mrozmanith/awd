#ifndef UNISTD_WIN_H
#define UNISTD_WIN_H

#include <io.h>

#define open _open
#define lseek _lseek
#define mkstemp(name) _mktemp_s(name, sizeof(name))

#define off_t int

#endif