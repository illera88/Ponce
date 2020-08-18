### Symbolic and Taint engines

Ponce has two different engines:

* **Taint engine**: This engine is used to determine at every step of the binary's execution which parts of memory and registers are controllable by the user input.    
    
    This engine is more useful when you just want to see how user controlled data moves through a program.

    For example during exploit development you can use the taint engine to narrow down which functions of the binary are parsing the user controlled data. 
    
    The taint engine is slighly faster and lightweight than the symbolic one.

* **Symbolic engine**: This engine maintains a symbolic state of registers and part of memory at each step in a binary's execution path. And allows a user to get a solution for a specific condition.

    This engine is useful when there is a non straigh forward condition to solve in order to reach an area of code we are interested in.

    One example is CTF challenges that do some complex calculations and checks for a specific result.

    
