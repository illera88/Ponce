# Enable/Disable Ponce

Ponce doesn't do anything, is disabled, until you symbolize or taint some data. In this way Ponce doesn't have a impact in the program you are debugging until you decide to use Ponce to symbolically analize the instructions.

Once Ponce is enabled is going to execute instructions step by step, even if you continue the execution, analyze it with Triton and visualize the result.

But you can manually change this behaviour. Maybe you want to enable Ponce at the beginning of your program, just to get the symbolic instructions associated with some instructions.

ToDo: Add image

You can also disable Ponce at any given point. 

This is useful when you know that there is a are with a lot of instructions that don't interact with your input data. In that case you can disable Ponce until all those instructions have been executed and then Enable it again to analyze the instructions that interact with the symbolic/tainted data.

Ponce internally Disables and Enables itself when some library functions are called, like `printf`. More information in [Blacklisting library functions](usage/blacklist.md).