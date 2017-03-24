Contributing Guidelines
=======================

Before diving into the codebase, it's highly recommended that you have some basic knowledge
about how OOP works in C++. Some concepts that you should know include inheritance, polymorphism,
virtual members, etc. Knowledge of the C++ Standard Template Library (STL) is also helpful. [Effective
STL](https://www.amazon.ca/Effective-STL-Specific-Standard-Template/dp/0201749629) is a great book
that covers useful STL concepts in depth.

Table of Contents
-----------------
* [Style Guide](#style-guide)
  - [Common C++ Naming Conventions](#common-c-naming-conventions)
  - [Distinguish Private Object Data](#distinguish-private-object-data)
  - [Don't name anything starting with `_`](#dont-name-anything-starting-with-_)
  - [Enable out-of-source-directory builds](#enable-out-of-source-directory-builds)
  - [Use `nullptr`](#use-nullptr)
  - [Comments](#comments)
  - [Never use `using namespace` in a header file](#never-use-using-namespace-in-a-header-file)
  - [Include guards](#include-guards)
  - [{} are required for blocks](#-are-required-for-blocks)
  - [Keep lines a reasonable length](#keep-lines-a-reasonable-length)
  - [Use "" for including local files](#use--for-including-local-files)
  - [Initialize member variables](#initialize-member-variables)
  - [Use the correct integer type for standard library features](#use-the-correct-integer-type-for-standard-library-features)
  - [Use .h and .cpp for your file extensions](#use-h-and-cpp-for-your-file-extensions)
  - [Never mix tabs and spaces](#never-mix-tabs-and-spaces)
  - [Don't be afraid of templates](#dont-be-afraid-of-templates)
  - [Use operator overloads judiciously](#use-operator-overloads-judisciously)
  - [Avoid implicit conversions](#avoid-implicit-conversions)
  - [Consider the rule of zero](#consider-the-rule-of-zero)

* [Collaboration Guide](#collaboration-guide)

---

Style Guide
-----------
> An slightly modified excerpt from [C++ Best Practices](https://www.gitbook.com/book/sadmansk/cpp-best-practices/details)


Consistency is the most important aspect of style. The second most important aspect is following a style that the average C++ programmer is used to reading.

C++ allows for arbitrary-length identifier names, so there's no reason to be terse when naming things. Use descriptive names, and be consistent in the style.

 * `CamelCase`
 * `snake_case`

are common examples. *snake_case* has the advantage that it can also work with spell checkers, if desired.

### Common C++ Naming Conventions

 * Types start with upper case: `MyClass`.
 * Functions and variables start with lower case: `myMethod`.
 * Constants are all upper case: `const double PI = 3.14159265358979323;`.

C++ Standard Library (and other well-known C++ libraries like [Boost](http://www.boost.org/)) use these guidelines:

 * Macro names use upper case with underscores: `INT_MAX`.
 * Template parameter names use camel case: `InputIterator`.
 * All other names use snake case: `unordered_map`.

### Distinguish Private Object Data

Name private data with a `m_` prefix to distinguish it from public data. `m_` stands for "member" data.

```cpp
class PrivateSize
{
  public:
    int width() const { return m_width; }
    int height() const { return m_height; }
    PrivateSize(int width, int height) : m_width(width), m_height(height) {}

  private:
    int m_width;
    int m_height;
};
```




### Don't Name Anything Starting With `_`

If you do, you risk colliding with names reserved for compiler and standard library implementation use:

http://stackoverflow.com/questions/228783/what-are-the-rules-about-using-an-underscore-in-a-c-identifier


### Well-Formed Example

```cpp
class MyClass
{
public:
    MyClass(int t_data)
        : m_data(t_data) {
    }

    int getData() const {
        return m_data;
    }

private:
    int m_data;
};
```


### Enable Out-of-Source-Directory Builds

Make sure generated files go into an output folder that is separate from the source folder.


### Use `nullptr`

C++11 introduces `nullptr` which is a special value denoting a null pointer. This should be used instead of `0` or `NULL` to indicate a null pointer.

### Comments

Comment blocks should use `//`, not `/* */`. Using `//` makes it much easier to comment out a block of code while debugging.

```cpp
// this function does something
int myFunc() {

}
```

To comment out this function block during debugging we might do:

```cpp
/*
// this function does something
int myFunc() {

}
*/
```

which would be impossible if the function comment header used `/* */` (since the comment would only go until the first `*/`
that is encountered.

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
2. Clone your fork
3. Add the main repo as a remote (e.g. you can name it upstream)
3. `git checkout -b branch_name` (make sure to use a descriptive name for your branch)
4. Make your changes and commit them under the new branch
5. `git pull --rebase upstream develop`
6. `git push origin branch_name`
7. Open a PR with a nice description.
8. Make sure the build passes before asking for a review

### Additional Notes:
Make sure that every single commit messages reflect the purpose of the associated changes. The pull requests should also have relevant names with added description as necessary.

### Submitting a PR and reviewing:
The use of proper labels is highly encouraged, as well as using the assignee field
for tracking task ownership. For example, assign yourself while the issue/PR is
being worked on, as well as label it with `DO NOT MERGE`. When it's ready for
review, assign the reviewer, and remove the `DO NOT MERGE` label. After back and
forths (by changing assignments between the reviewer and reviewee), once the changes
are approved, the reviewer labels the PR as reviewed and assigns the PR to the
maintainer so they can merge the PR. After the merge is done, the maintainer will
assign it back to the author to keep track of ownership in the Kanban board.
