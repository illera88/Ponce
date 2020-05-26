# Ponce limitations



Concolic execution and Ponce have some problems:

* Symbolic memory load/write: When the index used to read a memory value is symbolic like in `x = aray[symbolic_index]` some problems arise that could lead on the loose of track of the tainted/symbolized user controled input.
* Triton doesn't work very well with [floating point instructions](https://github.com/illera88/Ponce/issues/59).
* Concolic execution only analyzed the executed instructions. That means that symbolic tracking is lost in cases like the following:

  ```text
  int check(char myinput)  // Input is symbolic/tainted
  {
  int flag = 0;
  if (myinput == 'A')  //This condition is symbolic/tainted
    flag = 1
  else
    flag =- 1;
  return flag; // flag is not symbolic/tainted!
  }
  ```

