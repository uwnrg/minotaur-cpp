Contributing Guidelines

It is expected that you have basic C++, STL, and Object-Oriented knowledge before diving into Minotaur. Wherever you may lack, Google and Stack Overflow are your friends (so are your fellow members!). 

Table of Contents

Style Guide

Consistent coding style across Minotaur is essential for a readable and understandable codebase. We don't follow a strict style but these are basic guidelines for coding in Minotaur.

Naming Conventions

- Types begin capitalized and follow camel-case: RowLineHighlighter, QSlider
  - Generic types are snake-case: point, rect, unordered_map
- Functions and variables are snake-case: reorder_path(), row_count
  - override functions from Qt may be camel-case: timerEvent(...)
- Constants and macros are all-caps with underscores: FIND_MIN
  - constexpr variables follow normal variable naming
- Template type parameters are capitalized and camel-case: typename Element
  - None-type or generic template parameters are snake-case: int block_size
- Files names are all lower-case, no underscores: simulatorwindow.cpp
- Header files use .h and source files use .cpp

Use Four Spaces for Indentation

Tabs are for losers. Also mixing tabs and spaces gives Git indigestion.

Distinguish Private Class Members

Private class members should be prefixed with m_,

    class Rectangle {
    public:
        Rectangle(int width, int height);
    
    private:
        int m_width;
        int m_height;
    };

Don't Start and End Names with Underscores

Unless you want to indicate a non-public type,

    namespace det_ {
        struct __private_helper_type { ... };
    }

Use nullptr instead of NULL

nullptr is more portable and nicer on the eyes.

Comments

Single and multi-line comments should always begin with //, be followed with a space, and be capitalized: // Add the x and y components. Periods should only be used for multi-line comments,

    // This is a multi-line comment inside a block
    // of code, which uses double slashes.

Block comments with /* */ should be avoided inside code. Javadoc-style comments are encouraged for functions and classes,

    /**
     * Element-wise addition of vectors.
     * 
     * @param a    first vector to add
     * @param b    second vector to add
     * @param res  vector to store result
     * @param size number of elements
     */
    void vector_add(float *a, float *b, float *res, int size);

Never Put using in a Header File

Headers are copy-pasted with the #include directive so includes a header with using will pollute the namespace.

Always Put Include Guards in Headers

Prevents a header from being included twice, causing multiple definitions issues. Most IDEs generate them but ours should follow the format MINOTAUR_CPP_FILENAME_H. For instance highlighter.h would have

    #ifndef MINOTAUR_CPP_HIGHLIGHTER_H
    #define MINOTAUR_CPP_HIGHLIGHTER_H
    ...
    #endif

Always Use Braces {} with Blocks

Makes code less confusing to read. Inline braces are acceptable for repeatable logic. The open brace { should NOT be on a new line,

    int function_with_logic(int a, int b, int c) {
        if (a > b) { return a - b; }
        if (b < c && c < a) { return c + b; }
        return a + b + c;
    }

Line Continuation

Lines should be much less than 120 characters long. Continuation indents should be at least four tabs and align to the previous line, and operators should lead indentations,

    int function_with_logic(int a, int b, int c) {
        if (a + b < c
            && abs(c - a) < abs(a - b)
            && a + b + c < 0) { ... }
    }

Use "" to Include Local Files

Angle brackets <> should only be used for library or system includes.

    // Library includes
    #include <string>
    #include <cmath>
    #include <QSlider>
    
    // Local includes
    #include "highligher.h"
    #include "../utility/rect.h"

Use Constructor Initializer List

It is more performant and cleaner,

    class IntBox {
    public: 
        IntBox(int value) :
            m_value(value) {}
        
    private:
        int m_value;
    };

Put Default Values in Source File

Most class definitions are in header files, and we'd like to minimize modifications to those. Instead of

    // intbox.h
    class IntBox {
    public:
        IntBox();
    private:
        int m_value{0}; // or `int m_value = 0;`
    };

Do this

    // intbox.h
    class IntBox {
    public:
        IntBox();
    private:
        int m_value;
    };
    
    // intbox.cpp
    #define INTBOX_DEFAULT_VALUE	0
    IntBox::IntBox() :
    	m_value(INTBOX_DEFAULT_VALUE) {}

Use std::size_t with STL Sizes

The Standard Template Library uses std::size_t for things to do with size, such as my_vector.size(), my_map.size(), and is preferred for indexing.

Never Put Code with Side Effects inside assert()

Assert statements are empty when running a release build, so side-effects will not occur.

    // different things will occur if running debug or release builds!
    void my_function(int x) {
        assert(function_with_side_effect(x));
        ...
    }

Don't Be Afraid of Templates

But use them judiciously.

Overload Operators Insofar as it Makes Code More Readable

So you can write point1 + point2 instead of point1.add(point2). Arithmetic operators can be overloaded when they make sense, access operators *, [], and -> should be used for pointer-like types, stream operators << and >> can be overloaded for log or debug streams, comparison operators for types with order, and so on. More exotic operators can be overloaded as appropriate. Only overload the comma operator if you're edgy.

Mark Single Argument Constructors as explicit

Such as

    class IntBox {
    public:
        explicit IntBox(int value);
    };

To avoid implicit conversion attempts by the compiler. Sometimes implicit conversion operators and constructors are useful, however.
