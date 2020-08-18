# Blacklisting library functions

Symbolic execution consumes a lot of resources, so limiting the scope of what functions are symbolic executed and which ones not helps to increase the speed of the analysis and decrease the amount of resources consumed.

Many functions from libc or kernel32 don't modify the user input, so we can disable the taint/symbolic engine during the execution of those functions.

One example is the function `printf`. It is usually a pretty complex and big function, that doesn't modify the input. You can see a list of all the functions that by default are blacklisted [here](https://github.com/illera88/Ponce/blob/master/src/blacklist.cpp#L31).
