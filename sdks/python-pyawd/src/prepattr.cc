/**
 * This file contains the preparation function for elements with attributes. It's not
 * the same as the other preparation functions, which are using a standardized signature
 * so that they can be invoked anonomously by the preparation loop. This function
 * should only be invoked on those elements (blocks or other elements) that have attributes
 * that should be transferred to the libawd representation of them.
*/

#include "AWDWriter.h"
#include "bcache.h"

void
__prepare_attr_element(PyObject *py_elem, AWD *lawd_awd, AWDAttrElement *lawd_elem)
{
    PyObject *nsdict_attr;
    PyObject *adict_attr;
    
    adict_attr = PyObject_GetAttrString(py_elem, "attributes");
    nsdict_attr = PyObject_GetAttrString(adict_attr, "_AWDAttrDict__nsdict");

    if (nsdict_attr) {
        PyObject *key;
        PyObject *val;
        Py_ssize_t ns_pos = 0;

        while (PyDict_Next(nsdict_attr, &ns_pos, &key, &val)) {
#if PYTHON_VERSION == 3
            if (val && PyDict_Check(val) && PyUnicode_Check(key)) {
#else
            if (val && PyDict_Check(val) && PyString_Check(key)) {
#endif
                PyObject *attr_key;
                PyObject *attr_val;
                Py_ssize_t attr_pos = 0;
                AWDNamespace *ns;
                const char *key_str;
                int key_len;

#if PYTHON_VERSION == 3
                key_str = PyUnicode_AS_DATA(key);
                key_len = PyUnicode_GET_DATA_SIZE(key);
#else
                key_str = PyString_AsString(key);
                key_len = PyString_Size(key);
#endif

                ns = lawd_awd->get_namespace(key_str);
                if (!ns) {
                    ns = new AWDNamespace(key_str, key_len);
                    lawd_awd->add_namespace(ns);
                }

                while (PyDict_Next(val, &attr_pos, &attr_key, &attr_val)) {
#if PYTHON_VERSION == 3
                    if (PyUnicode_Check(attr_key)) {
#else
                    if (PyString_Check(attr_key)) {
#endif
                        char *key_str;
                        awd_uint16 key_len;
                        AWD_field_ptr value;
                        awd_uint16 value_len;

#if PYTHON_VERSION == 3
                        key_str = (char *)PyUnicode_AS_DATA(attr_key);
                        key_len = PyUnicode_GET_DATA_SIZE(attr_key);
                        value.str = (char *)PyUnicode_AS_DATA(PyObject_Str(attr_val));
                        value_len = PyUnicode_GET_DATA_SIZE(PyObject_Str(attr_val));
#else
                        key_str = PyString_AsString(attr_key);
                        key_len = PyString_Size(attr_key);
                        value.str = PyString_AsString(PyObject_Str(attr_val));
                        value_len = PyString_Size(PyObject_Str(attr_val));
#endif

                        //TODO: Deal with different value types differently
                        lawd_elem->set_attr(ns, key_str, key_len, value, value_len, AWD_FIELD_STRING);
                    }
                }
            }
            /*
            TODO: Define a way for prep functions to set errors (and return NULL)
            else {
                PyErr_SetString(PyExc_ValueError, "attributes must be dictionary of namespaces.");
                return NULL;
            }
            */
        }
    }
}

