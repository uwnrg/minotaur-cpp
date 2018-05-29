Contributing Guidelines
=======================

It is expected that you have basic C++, STL, and Object-Oriented knowledge before diving into Minotaur. Wherever you may lack, Google and Stack Overflow are your friends (so are your fellow members!). 

Style Guide
-----------
Consistent coding style across Minotaur is essential for a readable and understandable codebase. We don't follow a strict style but these are basic guidelines for coding in Minotaur.

### Naming Conventions
 * Types begin capitalized and follow camel-case: `RowLineHighlighter`, `QSlider`
   * Generic types are snake-case: `point`, `rect`, `unordered_map`
 * Functions and variables are snake-case: `reorder_path()`, `row_count`
   * `override` functions from Qt may be camel-case: `timerEvent(...)`
 * Constants and macros are all-caps with underscores: `FIND_MIN`
   * `constexpr` variables follow normal variable naming
 * Template type parameters are capitalized and camel-case: `typename Element`
   * None-type or generic template parameters are snake-case: `int block_size`
 * Files names are all lower-case, no underscores: `simulatorwindow.cpp`
 * Header files use `.h` and source files use `.cpp`

### Use Four Spaces for Indentation

Tabs are for losers. Also mixing tabs and spaces gives Git indigestion.

### Distinguish Private Class Members

Private class members should be prefixed with `m_`,

```c++
class Rectangle {
public:
    Rectangle(int width, int height);

private:
    int m_width;
    int m_height;
};
```


### Don't Start and End Names with Underscores

Unless you want to indicate a non-public type,

```c++
namespace det_ {
    struct __private_helper_type { ... };
}
```

### Use `nullptr` instead of `NULL`

`nullptr` is more portable and nicer on the eyes.

### Comments

Single and multi-line comments should always begin with `//`, be followed with a space, and be capitalized: `// Add the x and y components`. Periods should only be used for multi-line comments,
```c++
// This is a multi-line comment inside a block
// of code, which uses double slashes.
```

Block comments with `/* */` should be avoided inside code. Javadoc-style comments are encouraged for functions and classes,
```c++
/**
 * Element-wise addition of vectors.
 * 
 * @param a    first vector to add
 * @param b    second vector to add
 * @param res  vector to store result
 * @param size number of elements
 */
void vector_add(float *a, float *b, float *res, int size);
```

### Never Put `using` in a Header File 

Headers are copy-pasted with the `#include` directive so including a header with `using` will pollute the namespace.


### Always Put Include Guards in Headers

Prevents a header from being included twice, causing multiple definitions issues. Most IDEs generate them but ours should follow the format `MINOTAUR_CPP_FILENAME_H`. For instance `highlighter.h` would have

```cpp
#ifndef MINOTAUR_CPP_HIGHLIGHTER_H
#define MINOTAUR_CPP_HIGHLIGHTER_H
...
#endif
```

### Always Use Braces `{}` with Blocks
Makes code less confusing to read. Inline braces are acceptable for repeatable logic. The open brace `{` should **NOT** be on a new line,

```cpp
int function_with_logic(int a, int b, int c) {
    if (a > b) { return a - b; }
    if (b < c && c < a) { return c + b; }
    return a + b + c;
}
```

### Line Continuation

Lines should be much less than 120 characters long. Continuation indents should be at least four tabs and align to the previous line, and operators should tail indentations,

```c++
int function_with_logic(int a, int b, int c) {
    if (a + b < c &&
        abs(c - a) < abs(a - b) &&
        a + b + c < 0) { ... }
}
```


### Use `""` to Include Local Files
Angle brackets `<>` should only be used for library or system includes.

```cpp
// Library includes
#include <string>
#include <cmath>
#include <QSlider>

// Local includes
#include "highligher.h"
#include "../utility/rect.h"
```

### Use Constructor Initializer List

It is more performant and cleaner,

```c++
class IntBox {
public: 
    IntBox(int value) :
        m_value(value) {}
    
private:
    int m_value;
};
```

### Put Default Values in Source File

Most class definitions are in header files, and we'd like to minimize modifications to those. Instead of

```c++
// intbox.h
class IntBox {
public:
    IntBox();
private:
    int m_value{0}; // or `int m_value = 0;`
};
```

Do this

```c++
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
```

### Use `std::size_t` with STL Sizes

The Standard Template Library uses `std::size_t` for things to do with size, such as `my_vector.size()`, `my_map.size()`, and is preferred for indexing.

### Never Put Code with Side Effects inside `assert()`

Assert statements are empty when running a release build, so side-effects will not occur.

```c++
// different things will occur if running debug or release builds!
void my_function(int x) {
    assert(function_with_side_effect(x));
    ...
}
```

### Don't Be Afraid of Templates

But use them judiciously.

### Overload Operators Insofar as it Makes Code More Readable

So you can write `point1 + point2` instead of `point1.add(point2)`. Arithmetic operators can be overloaded when they make sense, access operators `*`, `[]`, and `->` should be used for pointer-like types, stream operators `<<` and `>>` can be overloaded for log or debug streams, comparison operators for types with order, and so on. More exotic operators can be overloaded as appropriate. Only overload the comma operator if you're edgy.

### Mark Single Argument Constructors as `explicit`

Such as

```c++
class IntBox {
public:
    explicit IntBox(int value);
};
```

To avoid implicit conversion attempts by the compiler. Sometimes implicit conversion operators and constructors are useful, however.

### Speeding Up Compile Times of Headers

Include directives tell the compiler to essentially copy-paste header files. In C++ this can lead hundreds and even thousands of header files to be loaded into memory and copied per source file. 

#### Include only what you need

Instead of including `<cstdio>` you can include only `<iostream>`.

#### Use Class Forward Declarations

If you don't need a class definition, that is, you have only weak references or pointers to `MyClass`, use a forward declaration to avoid an additional inclusion (in the header).

```c++
// header file
class MyClass;
bool verify_object(MyClass *obj);

// source file
#include "myclass.h"
bool verify_object(MyClass *obj) { ... } // object definition available
```

#### Put as much in source files as possible

Especially if you need to change them frequently. This can lead to long recompile times in a header file. These include constants, macros, and functions.

### Use `static` Functions When Possible

For instance, if you want helper functions in your source files. Marking files as `static` makes them "local" to that source file and can aid compiler optimizations.

```c++
// file1.cpp
static int get_a_value() { ... } // only visible in this file
int get_b_value() { ... } // might be visible in other files

// file2.cpp
extern int get_a_value(); // undefined symbol error
extern int get_b_value(); // can be used in this file
```

Though avoid usage of `extern` in Minotaur.

### Be Mindful of Initialization Order

Class member variables are initialized in the order they are declared in the header, regardless of the initialization list. For instance,

```c++
class DoubleBox {
public:
    DoubleBox(double value) :
        m_first(value),
        m_second(m_first * 2) {}
    
private:
    double m_second;
    double m_first;
};
```

Here, `m_second` will initialize first, when `m_first` has not been initialized. The result is that `m_second` will have some junk value and `m_first` will be set to `value`. In general it is good practice that class members be initialized in their declaration order.

### Use Smart Pointers

C++ provides `std::unique_ptr` and `std::shared_ptr` that manage dynamically allocated pointers. In most cases `unique_ptr` suffices, and a weak reference can be passed around with `get()`; due to the hierarchical structure of Qt, it is easy to predict when a pointer is deleted. Use `std::make_unique` and `std::make_shared`,

```c++
int value = 12;
auto box_uptr = std::make_unique<IntBox>(value);
auto box_sptr = std::make_shared<IntBox>(value);
```

### Usage of `auto`

Type deduction can be useful to reduce code bloat in a few instances, but over-usage of `auto` can make code difficult to read and debug. `auto` should be used when casting, making a smart pointer, using an iterator, or dealing with complex (tuple) types,

```c++
auto integer_value = static_cast<int>(12.64);

auto map_it = my_unordered_map.begin();
// instead of
std::unordered_map<int, double>::iterator map_it = ...

auto three_tuple = std::make_tuple(10.5, 10, "hello world");
// instead of
std::tuple<double, int, std::string> three_tuple = ...
```

### Using Appropriate Casts

Never use the basic C-type cast `int integer = (int) decimal`. Always use one of the specific cast types `static_cast`, `reinterpret_cast`, `dynamic_cast`, `const_cast`. A dynamic cast makes use of run-time type info, and returns `nullptr` if the cast is invalid.

### Avoid Lambdas

Lambda expressions are supported in C++11, Minotaur's standard, but they will not be allowed in Minotaur's code-base. Use a functor instead,

```c++
struct functor_type {
    bool operator()(int arg) const { ... }
};
```

### Pass by Reference and Avoid Copying

To avoid unnecessary copying. Consider the case,

```c++
class ListHolder {
...
public:
    std::vector<int> get_list() { return m_list; }
};

...
std::vector<int> my_list = holder.get_list();
```

The function `get_list()` will return a copy of `m_list`, then `list = holder.get_list()` will make another
copy of that list into `my_list`. Instead, we can do

```c++
std::vector<int> &get_list() { return m_list; }
...
std::vector<int> &my_list = holder.get_list();
```

This way `my_list` is a reference to the list in `holder`, so no copying is done. If you don't want the list to be 
modified, return `const std::vector<int> &` instead.

The same is for function arguments,

```c++
int append_and_sum(int v, std::vector<int> list) {
    list.push_back(v);
    ...
    return sum;
}

...
int list_sum = append_and_sum(5, my_list);
```

Here, `my_list` is copied into the function before the summing occurs. Passing by reference, 
`int append_and_sum(int v, std::vector<int> &list)` removes the copy operation, but side effects
will now occur on `list`.

### Make Use of Move Operations

Consider the function which creates a returns list,

```c++
std::vector<int> make_consecutive_list(std::size_t max) {
    std::vector<int> list(max);
    for (std::size_t i = 0; i < max; ++i) { list[i] = i; }
    return list;
}

...
std:vector<int> filled_list = make_consecutive_list(128);
```

In C++11 no copying or destruction of the list is done; the return value is "moved" to `filled_list` using a move constructor:
`template<typename U> vector(vector<U> &&list)`. This is a bit nicer than having the caller first allocate the list as in
`void make_consecutive_list(std::size_t max, std::vector<int> &list)`.

Qt Guidelines
-----------

The Qt framework tends to be rather massive and in-depth. Many Qt features overlap with STL, such as `QList`. This section provides some basic tips on Qt.

### `ui` Files

Files ending with `.ui` are XML files that describe a `QWidget`. These can be edited manually or opened with a GUI editor in Qt Creator. When compiling a Qt project, the Qt Meta-Object Compiler (MOC) reads these XML files and generates a `ui_*.h` file. For instance, `ui_imageviewer.h` which is used with `imageviewer.h`. These headers should only be included in the source files.

```c++
// imageviewer.h
#ifndef MINOTAUR_CPP_IMAGEVIEWER_H
#define MINOTAUR_CPP_IMAGEVIEWER_H

#include <QWidget>

namespace Ui {
    class ImageViewer;
}

class ImageViewer : public QWidget {
Q_OBJECT

public:
    ...
private:
    Ui::ImageViewer *m_ui;
};

#endif

// imageviewer.cpp
#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ImageViewer) { // usable because ui_imageviewer.h is included
	
    m_ui->setup(this);
}
```

The MOC will also generate a `moc_imageviewer.cpp` based on `imageviewer.h` which controls the `Q_SIGNAL`s and `Q_SLOT`s. 

### Use Signal and Slot References

In Minotaur we avoid usage of `SIGNAL(...)` and `SLOT(...)`. Instead do,

```c++
connect(pButton, &QPushButton::clicked, pWidget, &MyWidget::clicked_slot);
```

### Connect Signals to Signals

You can connect signals to signals if you need to bounce them around the widget tree,

```c++
connect(pButton, &QPushButton::clicked, pParent, &ParentWidget::clicked_signal);
connect(pParent, &ParentWidget::clicked_signal, pChild, &ChildWidget::click_slot);
```

### Always Pass a Parent

If there is one available. Qt default constructors will have a `nullptr` parent, so we should
try to pass one if there is one available.

### Sometimes You Don't Need More Slots

Signals and slots are used by Qt with its event and thread management system. Signals and slots are
recommended for dealing with events or communicating across threads by allowing Qt to handle
synchronization. But you don't need a series of signals and slots if you're only working in one thread,

```c++
Q_SIGNAL void clicked_signal(); // connected to post_clicked

Q_SLOT void button_clicked(); // does something and emits clicked_signal
Q_SLOT void post_clicked(); // just call post_clicked() without a slot
```
