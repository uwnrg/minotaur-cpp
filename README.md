# Minotaur - C++ re-implementation
[![Build Status](https://travis-ci.org/Controls-UWNRG/minotaur-cpp.svg?branch=master)](https://travis-ci.org/Controls-UWNRG/minotaur-cpp)

### c++ migration
#### why c++? why the migration?
python is situationally a great language. it feels intuitive when you're dealing with small programs. [it's all fresh in your head](http://qr.ae/rgd6jh) since you are dealing with a minimal number of custom objects/types. however, as a project gets bigger, and extends to thousands of lines of code, it becomes extremely hard to look at a codebase that you are either unfamiliar with or haven't worked with in recent times. one of the main reasons for this is that python is a [dynamically-typed language](https://en.wikipedia.org/wiki/dynamic_programming_language). and also the fact that python has terrible object-oriented programming features is just more fuel to the fire. let's not even talk about real-time performance of the language, that's a long rant.  

c++ is not an angel either. however, good structural foundations in a powerful object-oriented language would help all future collaborators when they contribute to and maintain the project, and c++ is one of the best out there. this is very important for the long term development cycle, for reasons discussed above. c++ has its fair share of downsides also, e.g. it is a very mature language which makes it difficult to understand for newer programmers unfamiliar with a lot of advanced programming concepts. Moreover, it might not feel as intuitive for people who never worked with real programming languages like C/C++ (*#rekt*). All jokes aside, for the dedicated contributors like **you**, these should not be that big of an issue :smile:. And also, the pros of using C++ over Python grandly outweighs the cons, even including all the effort required to rewrite the entire project. So here we are.

#### Goals and Requirements
* Cross-platform development and deployment
* Maintainable object-oriented architecture
* Abstracted implementation and approachable interface
* High-speed architecture for real-time data crunching (read image recognition)  

*More to come...*

#### Development
##### Setup
For now, you only need to get the [Qt Framework](http://www.qt.io/download/). You can also get Qt Creator in order to build the code with one button and use their form editor for easy GUI design. You can open the project by specifying the folder `minotaur` and then build and run the code from there.

##### Contributing
Any help would be greatly appreciated. The entire code base is pretty big, there is a lot of code to be rewritten. If you want to help out, let [me](https://github.com/sadmansk) know and we will figure something out.
