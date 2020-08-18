# Symbolize/Taint data

Typically, the first step in Ponce is to Symbolize or Taint, depending of the engine you are using, a user input you want to analyze, for example an input file.

For now on we are going to use the symbolic engine. But the same steps would apply for the taint engine.

You can symbolize two things, memory or registers.

In order to symbolize memory you can do it from the Dissassembly view or directly from the hexdump. 

The advantage of doing it from the dissasembly view is that you can jump directly to the address using the Ponce context menu over an address.

The first time you symbolize an item Ponce enables the symbolic engine.