# Negate, Inject & Restore

In the next example we are using the snapshot engine:

* Passing a file as argument.
* We select we want to use the symbolic engine.
* We taint the buffer that `fread()`reads from the file.
* We create a snapshot in the function that parses the buffer read from the file.
* When a condition is evaluated we negate it, inject the solution in memory and restore the snapshot with it.
* The solution will be "valid" so we will satisfy the existent conditions.

![x64\_fread\_test\_negate\_restore\_inject.gif](https://github.com/illera88/Ponce/blob/780e1992a935d310f5a956e6ece6b8f630a853a7/media/x64_fread_test_negate_restore_inject.gif?raw=true)

