# Symbolic engine

#### Use symbolic execution to solve a crackMe

Here we can see the use of the symbolic engine and how we can solve constrains:

* Passing simple `aaaaa` as argument.
* We first select the symbolic engine.
* We convert to symbolic the memory pointed by argv\[1\] \(`aaaaa`\)
* Identify the symbolic condition that make us win and solve it.
* Test the solution.

 The crackme source code can be found [here](https://github.com/illera88/Ponce/blob/master/examples/crackme_hash.cpp)

![manual\_symbolize\_and\_solve\_crackme\_hash2](https://cloud.githubusercontent.com/assets/5193128/18558235/32561cb2-7b27-11e6-846f-9fde03e88df5.gif)

#### 



