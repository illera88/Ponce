# Negate & Inject

In the next gif we can see the use of automatic tainting and how we can negate a condition and inject it in memory while debugging:

* We select the symbolic engine and set the option to symbolize `argv`.
* We identify the condition that needs to be satisfied to win the crackMe.
* We negate an inject the solution everytime a byte of our input is checked against the key.
* Finally we get the key `elite` that has been injected in memory and therefore reach the `Win` code.

 The crackme source code can be found [here](https://github.com/illera88/Ponce/blob/master/examples/crackme_xor.cpp)

![crackmexor\_negate\_and\_inject](https://cloud.githubusercontent.com/assets/5193128/18558282/5dd1cbca-7b27-11e6-81d7-13044bfc0b59.gif)

