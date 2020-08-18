# Taint engine

The taint engine is specially useful when analyzing complex binaries in order to reduce the ammount of code that the analyst needs to focus on.

In the following example we analyze the program cmake. The goal is to figure out which instructions are parsing the user input, the input file. In order to do that we use the taint engine following the next steps:

* We pass a file as argument to cmake.
* We configure Ponce to use the taint engine.
* We taint the buffer that `fread()` reads from the file.
* We resume the execution under the debugger control to see where the taint input is moved to.
* Ponce will rename the tainted functions. These are the functions that somehow the user has influence on, not simply the executed functions.

![cmake\_tainting\_fread](https://cloud.githubusercontent.com/assets/5193128/18558313/7aaa6d88-7b27-11e6-9c63-9870720d14e3.gif)

