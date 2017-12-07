#ifndef MINOTAUR_CPP_PYTHON_H
#define MINOTAUR_CPP_PYTHON_H

#ifdef slots
    #undef slots
#endif
#ifndef _hypot
    #define _hypot hypot
#endif

#include <Python.h>

#ifdef isspace
    #undef isspace
#endif
#ifdef isupper
    #undef isupper
#endif
#ifdef islower
    #undef islower
#endif
#ifdef toupper
    #undef toupper
#endif
#ifdef tolower
    #undef tolower
#endif
#ifdef isalpha
    #undef isalpha
#endif
#ifdef isalnum
    #undef isalnum
#endif

#endif //MINOTAUR_CPP_PYTHON_H
