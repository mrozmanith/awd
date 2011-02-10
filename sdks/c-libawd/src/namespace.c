#include <stdlib.h>
#include <string.h>

#include "namespace.h"

AWD_namespace *
awd_add_namespace(AWD *awd, const char *uri, awd_bool check_dups)
{
    AWD_namespace *ns;

    if (check_dups == AWD_TRUE)
        ns = awd_get_namespace_by_uri(awd, uri);

    if (!ns) {
        awd->_last_ns_id++;
        ns = _awd_create_namespace(uri);
        ns->id = awd->_last_ns_id;
    }

    return ns;
}


AWD_namespace *
awd_get_namespace_by_uri(AWD *awd, const char *uri)
{
    unsigned int len;
    unsigned int i;
    AWD_namespace *cur_ns;

    len = awd->_last_ns_id + 1;
    for (i=0; i<len; i++) {
        cur_ns = awd->namespaces[i];
        if (strcmp(uri, cur_ns->uri)==0)
            return cur_ns;
    }

    return NULL;
}


AWD_namespace *
_awd_create_namespace(const char *uri)
{
    size_t len;
    AWD_namespace *ns;

    len = strlen(uri);

    ns = malloc(sizeof(AWD_namespace));
    ns->uri = malloc(len);
    strncpy(ns->uri, uri, len);

    return ns;
}

