#ifndef _NAMESPACE_H
#define _NAMESPACE_H

#include "awd.h"

AWD_namespace * awd_add_namespace(AWD *, const char *, awd_bool);
AWD_namespace * awd_get_namespace_by_uri(AWD *, const char *);

AWD_namespace * _awd_create_namespace(const char *);

#endif
