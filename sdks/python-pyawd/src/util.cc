#include <Python.h>

#include <awd/libawd.h>

bool
pyawdutil_has_true_attr(PyObject *o, const char *attr)
{
    PyObject *a;

    a = PyObject_GetAttrString(o, attr);
    if (a != NULL) {
        if (a == Py_True)
            return true;

        if (PyNumber_Check(a)) {
            PyObject *i;
            i = PyNumber_Int(a);
            if (PyInt_AsLong(i) > 0)
                return true;
        }
    }

    return false;
}

void
pyawdutil_get_strattr(PyObject *o, const char *attr, const char **str, int *len)
{
    PyObject *a;

    a = PyObject_GetAttrString(o, attr);
    if (a != NULL) {
#if PYTHON_VERSION == 3
        *str = PyUnicode_AS_DATA(a);
        if (len) *len = PyUnicode_GET_DATA_SIZE(a);
#else
        *str = PyString_AsString(a);
        if (len) *len = PyString_Size(a);
#endif
    }
    else {
        *str = NULL;
        if (len) *len = 0;
    }
}

awd_float64 *
pyawdutil_pylist_to_float64(PyObject *list, awd_float64 *buf, unsigned int num_items)
{
    if (PyList_Check(list)) {
        register unsigned int i;

        if (buf == NULL)
            buf = (awd_float64*)malloc(num_items * sizeof(awd_float64));

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
            buf = (awd_float32*)malloc(num_items * sizeof(awd_float32));

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
            buf = (awd_uint32*)malloc(num_items * sizeof(awd_uint32));

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
            buf = (awd_uint16*)malloc(num_items * sizeof(awd_uint16));

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

