#include <Python.h>

#include <awd/libawd.h>

awd_float64 *
pyawdutil_pylist_to_float64(PyObject *list, awd_float64 *buf, unsigned int num_items)
{
    if (PyList_Check(list)) {
        register unsigned int i;

        if (buf == NULL)
            buf = malloc(num_items * sizeof(awd_float64));

        for (i=0; i<num_items; i++) {
            PyObject *item;

            item = PyList_GetItem(list, i);

            if (PyNumber_Check(item)) {
                buf[i] = (awd_float64)PyFloat_AsDouble(PyNumber_Float(item));
            }
            else return NULL;
        }

        return buf;
    }

    return NULL;
}

awd_float32 *
pyawdutil_pylist_to_float32(PyObject *list, awd_float32 *buf, unsigned int num_items)
{
    if (PyList_Check(list)) {
        register unsigned int i;

        if (buf == NULL)
            buf = malloc(num_items * sizeof(awd_float32));

        for (i=0; i<num_items; i++) {
            PyObject *item;

            item = PyList_GetItem(list, i);

            // Check if float or long int, or bail out
            // if none of these.
            if (PyNumber_Check(item)) {
                buf[i] = (awd_float32)PyFloat_AsDouble(PyNumber_Float(item));
            }
            else return NULL;
        }

        return buf;
    }

    return NULL;
}


awd_uint32 *
pyawdutil_pylist_to_uint32(PyObject *list, awd_uint32 *buf, unsigned int num_items)
{
    if (PyList_Check(list)) {
        register unsigned int i;

        if (buf == NULL)
            buf = malloc(num_items * sizeof(awd_uint32));

        for (i=0; i<num_items; i++) {
            PyObject *item;

            item = PyList_GetItem(list, i);

            // Check if long int, or bail out
            if (PyNumber_Check(item)) {
                buf[i] = (awd_uint32)PyLong_AsLong(item);
            }
            else return NULL;
        }

        return buf;
    }

    return NULL;
}


awd_uint16 *
pyawdutil_pylist_to_uint16(PyObject *list, awd_uint16 *buf, unsigned int num_items)
{
    if (PyList_Check(list)) {
        register unsigned int i;

        if (buf == NULL)
            buf = malloc(num_items * sizeof(awd_uint16));

        for (i=0; i<num_items; i++) {
            PyObject *item;

            item = PyList_GetItem(list, i);

            // Check if long int, or bail out
            if (PyNumber_Check(item)) {
                buf[i] = (awd_uint16)PyLong_AsLong(item);
            }
            else return NULL;
        }

        return buf;
    }

    return NULL;
}

