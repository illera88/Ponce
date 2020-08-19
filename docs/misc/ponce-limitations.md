# Ponce limitations

Ponce and Triton, the framework that Ponce relies on, have some limitations due to the nature of concolic execution. It is important to understand these limitations before you start using Ponce.


## Symbolic index while doing a memory load or write

When the index used to read a memory value is symbolic, like in:
```
x = aray[symbolic_index]
``` 
Some problems arise that could lead to the loose of track of the tainted/symbolized user controled input. 

This is a common problem with symbolic/concolic execution, there are some mitigations but it's not trivial to solve it. 

One simple example where this happens:
```c
#include<stdio.h>

char * alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 

int toUpper(char *userinput) {
  for (int i=0; i < strlen(userinput); i++){
    // We loose here the symbolic tracking, when using a symbolic index to access non symbolic data, alphabet 
    userinput[i] = alphabet[userinput[i] - 'a'];
  }
}

int main(){
  char * userinput = malloc(10);
  memcpy(userinput, "aaaaaaaaa", 10); 
  // Let's image aaaaaaaaa is the userinput that is symbolic
  toUpper(userinput);
  // After the execution of the function userinput is not symbolic anymore
  return 0;
}
```

## Concolic execution only analyzed the executed instructions
It's obvious but sometimes the implications are not so easy to spot.

This means that symbolic tracking is lost in cases like the following:

```c
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

Concolic execution only see one of the branches of the condition, so it doesn't have a way to figure out that the flag value depends on the result of the symbolic condition.


## Triton, thereforce Ponce, doesn't work very well with floating instructions
As mentioned in this ticket:
https://github.com/illera88/Ponce/issues/59