# Taint engine

In this example we can see the use of the tainting engine with cmake. We are:

* Passing a file as argument to cmake to have him parsing it.
* We select we want to use the tainting engine
* We taint the buffer that \`\`\`fread\(\)\`\`\`\` reads from the file.
* We resume the execution under the debugger control to see where the taint input is moved to.
* Ponce will rename the tainted functions. These are the functions that somehow the user has influence on, not the simply executed functions.

![cmake\_tainting\_fread](https://cloud.githubusercontent.com/assets/5193128/18558313/7aaa6d88-7b27-11e6-9c63-9870720d14e3.gif)

