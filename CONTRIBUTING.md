Contributing Guidelines
=======================

It is expected that you have basic C++, STL, and Object-Oriented knowledge before diving into Minotaur. Wherever you may lack, Google and Stack Overflow are your friends (so are your fellow members!). 

Table of Contents
-----------------

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

### Distinguish Private Class Members

Private class members should be prefixed with `m_`.

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

Unless you want to indicate a non-public type.

### Use `nullptr` instead of `NULL`

`nullptr` is more portable and nicer on the eyes.

### Comments

Single and multi-line comments should always begin with `//`, be followed with a space, and be capitalized: `// Add the x and y components`. Periods should only be used for multi-line comments:
```c++
// This is a multi-line comment inside a block
// of code, which uses double slashes.
```

Block comments with `/* */` should be avoided inside code. Javadoc-style comments are encouraged for functions and classes:
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

### Never Use `using namespace` in a Header File

This causes the namespace you are `using` to be pulled into the namespace of all files that include the header file.
It pollutes the namespace and it may lead to name collisions in the future.
Writing `using namespace` in an implementation file is fine though.


### Include Guards

Header files must contain a distinctly-named include guard to avoid problems with including the same header multiple times and to prevent conflicts with headers from other projects.

```cpp
#ifndef MYPROJECT_MYCLASS_H_
#define MYPROJECT_MYCLASS_H_

namespace MyProject {
  class MyClass {
  };
}

#endif
```

### {} Are Required for Blocks.
Leaving them off can lead to semantic errors in the code.

```cpp
// Bad Idea
// This compiles and does what you want, but can lead to confusing
// errors if modification are made in the future and close attention
// is not paid.
for (int i = 0; i < 15; ++i)
  std::cout << i << std::endl;

// Bad Idea
// The cout is not part of the loop in this case even though it appears to be.
int sum = 0;
for (int i = 0; i < 15; ++i)
  ++sum;
  std::cout << i << std::endl;


// Good Idea
// It's clear which statements are part of the loop (or if block, or whatever).
int sum = 0;
for (int i = 0; i < 15; ++i) {
  ++sum;
  std::cout << i << std::endl;
}
```

Also note that it's better to keep the opening parenthesis, `{` *not* on a new line. That helps save vertical space and makes the code easier to read.

### Keep Lines a Reasonable Length

```cpp
// Bad Idea
// hard to follow
if (x && y && myFunctionThatReturnsBool() && caseNumber3 && (15 > 12 || 2 < 3)) {
}

// Good Idea
// Logical grouping, easier to read
if (x && y && myFunctionThatReturnsBool()
    && caseNumber3
    && (15 > 12 || 2 < 3)) {
}
```

Many projects and coding standards have a soft guideline that one should try to use less than about 80 or 100 characters per line.
Such code is generally easier to read.
It also makes it possible to have two separate files next to each other on one screen without having a tiny font.


### Use "" for Including Local Files
... `<>` is [reserved for system includes](http://blog2.emptycrate.com/content/when-use-include-verses-include).

```cpp
// Bad Idea. Requires extra -I directives to the compiler
// and goes against standards.
#include <string>
#include <includes/MyHeader.hpp>

// Worse Idea
// Requires potentially even more specific -I directives and
// makes code more difficult to package and distribute.
#include <string>
#include <MyHeader.hpp>


// Good Idea
// Requires no extra params and notifies the user that the file
// is a local file.
#include <string>
#include "MyHeader.hpp"
```

### Initialize Member Variables
...with the member initializer list.

```cpp
// Bad Idea
class MyClass {
public:
    MyClass(int t_value) {
        m_value = t_value;
    }

private:
    int m_value;
};


// Good Idea
// C++'s member initializer list is unique to the language and leads to
// cleaner code and potential performance gains that other languages cannot
// match.
class MyClass {
public:
    MyClass(int t_value)
        : m_value(t_value) {
    }

private:
    int m_value;
};
```

In C++11 you may consider always giving each member a default value, e.g. by writing
```cpp
// ... //
private:
    int m_value = 0;
// ... //
```
inside the class body. This makes sure that no constructor ever "forgets" to initialize a member object.

Use brace initialization; it does not allow narrowing at compile-time:
```cpp
// Best Idea

// ... //
private:
  int m_value{ 0 };
// ... //
```
Prefer {} initialization over alternatives unless you have a strong reason not to.

Forgetting to initialize a member is a source of undefined behavior bugs which are often extremely hard to find.


### Use the Correct Integer Type for Standard Library Features

The standard library generally uses `std::size_t` for anything related to size. The size of `size_t` is implementation defined.

In general, using `auto` will avoid most of these issues, but not all.

Make sure you stick with the correct integer types and remain consistent with the C++ standard library. It might not warn on the platform you are currently using, but it probably will when you change platforms.

### Use .h and .cpp for Your File Extensions

Ultimately this is a matter of preference, but .h and .cpp are widely recognized by various editors and tools. So the choice is pragmatic. Specifically, Visual Studio only automatically recognizes .cpp and .cxx for C++ files, and Vim doesn't necessarily recognize .cc as a C++ file.

### Never Mix Tabs and Spaces

Some editors like to indent with a mixture of tabs and spaces by default. This makes the code unreadable to anyone not using the exact same tab indentation settings. Configure your editor so this does not happen. Tabs with a width of four spaces is a **MUST** for indentation for this project.

### Never Put Code with Side Effects Inside an assert()

```cpp
assert(registerSomeThing()); // make sure that registerSomeThing() returns true
```

The above code succeeds when making a debug build, but gets removed by the compiler when making a release build, giving you different behavior between debug and release builds.
This is because `assert()` is a macro which expands to nothing in release mode.

### Don't Be Afraid of Templates

They can help you stick to [DRY principles](http://en.wikipedia.org/wiki/Don%27t_repeat_yourself).
They should be preferred to macros, because macros do not honor namespaces, etc.

### Use Operator Overloads Judiciously

Operator overloading was invented to enable expressive syntax. Expressive in the sense that adding two big integers looks like `a + b` and not `a.add(b)`. Another common example is std::string, where it is very common to concatenate two strings with `string1 + string2`.

However, you can easily create unreadable expressions using too much or wrong operator overloading. When overloading operators, there are three basic rules to follow as described [on stackoverflow](http://stackoverflow.com/questions/4421706/operator-overloading/4421708#4421708).

Specifically, you should keep these things in mind:

* Overloading `operator=()` when handling resources is a must. See [Consider the Rule of Zero](03-Style.md#consider-the-rule-of-zero) below.
* For all other operators, only overload them when they are used in a context that is commonly connected to these operators. Typical scenarios are concatenating things with +, negating expressions that can be considered "true" or "false", etc.
* Always be aware of the [operator precedence](http://en.cppreference.com/w/cpp/language/operator_precedence) and try to circumvent unintuitive constructs.
* Do not overload exotic operators such as ~ or % unless implementing a numeric type or following a well recognized syntax in specific domain.
* [Never](http://stackoverflow.com/questions/5602112/when-to-overload-the-comma-operator?answertab=votes#tab-top) overload `operator,()` (the comma operator).
* Use non-member functions `operator>>()` and `operator<<()` when dealing with streams. For example, you can overload `operator<<(std::ostream &, MyClass const &)` to enable "writing" your class into a stream, such as `std::cout` or an `std::fstream` or `std::stringstream`. The latter is often used to create a string representation of a value.
* There are more common operators to overload [described here](http://stackoverflow.com/questions/4421706/operator-overloading?answertab=votes#tab-top).

More tips regarding the implementation details of your custom operators can be found [here](http://courses.cms.caltech.edu/cs11/material/cpp/donnie/cpp-ops.html).

### Avoid Implicit Conversions

#### Single Parameter Constructors

Single parameter constructors can be applied at compile time to automatically convert between types. This is handy for things like `std::string(const char *)` but should be avoided in general because they can add to accidental runtime overhead.

Instead mark single parameter constructors as `explicit`, which requires them to be explicitly called.

#### Conversion Operators

Similarly to single parameter constructors, conversion operators can be called by the compiler and introduce unexpected overhead. They should also be marked as `explicit`.


### Consider the Rule of Zero

The Rule of Zero states that you do not provide any of the functions that the compiler can provide (copy constructor, assignment operator, move constructor, destructor, move constructor) unless the class you are constructing does some novel form of ownership.

The goal is to let the compiler provide optimal versions that are automatically maintained when more member variables are added.

The [original article](http://flamingdangerzone.com/cxx11/rule-of-zero/) provides the background, while a [follow up article](https://turingtester.wordpress.com/2015/06/27/cs-rule-of-zero/) explains techniques for implementing nearly 100% of the time.


Collaboration Guide
-------------------
The basic steps to starting to work on a change and submitting it:

1. Fork the repo
2. Clone your fork from your local machine
3. Add the main repo as a remote (e.g. you can name it upstream)
4. `git checkout -b <branch_name>` (make sure to use a descriptive name for your branch)
5. Make your changes and commit them under the new branch
6. `git pull --rebase upstream develop`
7. `git push origin <branch_name>`
8. Open a PR with a nice description.
9. Make sure the build passes before asking for a review

### Additional Notes:
Make sure that every single commit message reflects the purpose of the associated changes. The pull requests should also have relevant names with added description as necessary.

### Submitting a PR and reviewing:
The use of proper labels is highly encouraged, as well as using the assignee field
for tracking task ownership. For example, assign yourself while the issue/PR is
being worked on, as well as label it with `DO NOT MERGE`. When it's ready for
review, assign the reviewer, and remove the `DO NOT MERGE` label. After back and
forths (by changing assignments between the reviewer and reviewee) and once the changes
are approved, the reviewer labels the PR as reviewed and assigns the PR to the
maintainer so they can merge the PR. After the merge is done, the maintainer will
assign it back to the author to keep track of ownership in the Kanban board (which
is found under the Projects tab on Github).

# Configuring Vagrant Development Environment
These will describe the necessary steps to setup a virtual development
environment on macOS Sierra 10.12.6 using Vagrant. The steps on other operating 
systems should be the exact same except for installing vagrant.

### Installing Vagrant
1. Download and install Vagrant from `https://www.vagrantup.com/downloads.html`
2. Download and install Oracle VirtualBox from `https://www.virtualbox.org/wiki/Downloads`

### Initializing Virtual Environment
1. Create a directory for Vagrant boxes `mkdir vagrant-boxes`
2. Setup the official box `vagrant init hashicorp/precise64`
3. Download and install XQuartz `https://www.xquartz.org/`
4. Edit the `Vagrantfile` created by `vagrant init` to have the line at the end

```config.ssh.forward_x11 = true```

5. Run `vagrant up`
6. Run `vagrant ssh-config`

### Automatically Configure the virtual environment
1. Access the box using `vagrant ssh`
2. Install git with `sudo apt-get install git`
3. Clone the repo and configure with 

```
git clone https://github.com/uwnrg/minotaur-cpp.git
cd minotaur-cpp
chmod 755 setup.sh
./vagrant_setup.sh
```

4. To configure and run in the future, make sure to add Qt to your path

```
export PATH=/home/vagrant/Qt5.7.0/5.7/gcc_64/bin:$PATH

```

5. Then use the commands

```
mkdir build
cd build
cmake ../CMakeLists.txt
make
./minotaur-cpp
```

### Manually Configure the virtual environment
1. Access the box using `vagrant ssh`

#### Installing Qt
1. Download the Qt 5.7 installer `wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run`
2. Adjust permissions `chmod +x qt-opensource-linux-x64-5.7.0.run`
3. We must now install the required dependecies to run the installer

```
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libfontconfig1
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa-dev
```

4. Run the installer with `./qt-opensource-linux-x64-5.7.0.run`, and if all goes 
well, the installer should open in a new window through `XQuartz`
5. You can login, or click `Skip`
6. Hit `Deselect All` and select only `Desktop gcc 64-bit`
7. Accept and install; do not launch Qt Creator
8. After install add Qt to the path with `export PATH=/home/vagrant/Qt5.7.0/5.7/gcc_64/bin:$PATH`

#### Installing Python
1. We require minimum Python 3.4, however Ubuntu 12.04 only goes up to Python 3.2
2. Run `sudo apt-get install python-software-properties` so that 
we have `add-apt-repository`
3. Add the PPA with Python 3.4 by running `sudo add-apt-repository ppa:fkrull/deadsnakes`
4. Run `sudo apt-get update`
5. Run `sudo apt-get install python3.4`
6. Run `sudo apt-get install python3.4-dev`

#### Install cmake
1. We require minimum cmake 3.1.0, so we will build cmake 3.2.2 from source
2. Download the source with `wget http://www.cmake.org/files/v3.2/cmake-3.2.2.tar.gz --no-check-certificate`
3. The `--no-check-certificate` is necessary because of Vagrant, and now install with

```
tar xf cmake-3.2.2.tar.gz
cd cmake-3.2.2
./configure
make
sudo make install
```

4. You can verify the installation with `cmake --version`

#### Cloning the repo
1. Install git with `sudo apt-get install git`
2. Clone the repo with `git clone https://github.com/mogball/minotaur-cpp.git`
3. Inside the repo, run `cmake CMakeLists.txt`
4. If all goes well, cmake will generate build files successfully

#### Installing gcc
1. We require gcc 4.8 to support C++11
2. Add the PPA `sudo add-apt-repository ppa:ubuntu-toolchain-r/test`
3. Install gcc 4.8 by running

```
sudo apt-get update
sudo apt-get install gcc-4.8 g++-4.8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
```

4. You can verify that gcc is at version 4.8 by running `gcc --version`

#### Installing additional dependencies
1. We need to install a few more things before we can build and run minotaur-cpp

```
sudo apt-get install libudev-dev
sudo apt-get install libxi6
sudo apt-get install libsm6
sudo apt-get install libxrender1
sudo apt-get install libegl1-mesa
```

#### Running minotaur
1. Build minotaur with `make`
2. Run minotaur with `./minotaur-cpp`

