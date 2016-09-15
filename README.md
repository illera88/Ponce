# Ponce

<p align="center"><img src ="https://cloud.githubusercontent.com/assets/5193128/18534105/f27da220-7a9c-11e6-8d2e-a940d94b404b.png" /></p>
 
## Definition
Ponce (pronuntiated *[ 'poN θe ]* pon-they ) is an IDA Pro plugin that provides users the ability to perform taint analysis and symbolic execution over binaries in an easy and intuitive fashion. The plugin is entirely written in C/C++.

## Why?
Symbolic execution is not a new concept in the security community. It has been around for years but it is not until the last couple of years when the open source projects like [Triton](https://github.com/JonathanSalwan/Triton) or [Angr](http://angr.io/) have been created to address this need. Despite the availability of these projects, end users often are left to implement specific use cases themselfs.

We addressed these needs by creating Ponce, an IDA plugin that integrates symbolic and taint analysis within the most used dissasembler/debugger by reverse engineers.

#Triton
Ponce relies on  the [Triton framework](https://github.com/JonathanSalwan/Triton) to provide semantics, taint analysis and symbolic execution. Triton is an awesome Open Source project sponsored by Quarkslab and maintained mainly by [Jonathan Salwan]() with a rich library. We would like to thank and endorse Jonathan's work with Triton. It rocks! :)

## Use modes
- **Tainting engine**: This engine is used to know at each program point what part of memory and register are controllable by the user input.
- **Symbolic engine**: This engine maintains a symbolic states of registers and part of memory at each program point. 

## Use cases
- **Exploit development**: Ponce can help you creating an exploit since you can easily see what parts of memory and registers you control and posible addresses you can use as ROP gadgets.
- **Malware Analysis**: Other use of Ponce is related with malware code. Analyzing the commands that family of malware support can be fastly determined symbolizing a simple known command and negating all the conditions where the command is being checked. 
- **Protocol Reversing**: One of the most interesting Ponce uses is the possibility of recognizing required magic numbers, headers or even entire protocols for controlled user input. For instance, Ponce can help you to list all the accepted arguments for a given command line binary or extracting the file format required for a specific file parser.
- **CTF**: Ponce speed up the process of reverse engineer binaries during CTFs. Since it is totally integrated in IDA you don't need to worry about setup timing. It's ready to be used!

## Installation
Ponce works with both x86 and x64 binaries. Installing the plugin is as simple as copying [Ponce.plw]() and [Ponce.p64]() to the ```plugins\``` folder in your IDA installation directory. 

The plugin will automatically run asking you for the initial configuration the fist time you use it and the configuration will be saved to a configuration file so you won't have to worry about the config window again.

## Building
We provide compiled binaries for Ponce but if you want to build your own plugin you can do it using Visual Studio 2013. We tried to make the building process as easy as possible:
- Clone the project with submodules: ```git clone --recursive https://github.com/illera88/PonceProject.git```
- Open ```Build\PonceBuild\Ponce.sln```: The project configuration is ready to use the includes and libraries shipped with the project that reside in ```external-libs\```.
- The VS project has a ```Post-Build Event``` that will move the created binary plugin to the IDA plugin folder for you. ```copy /Y $(TargetPath) "C:\Program Files (x86)\IDA 6.9\plugins"```. NOTE: use your IDA installation path.

You can find that the project has 4 build configurations:
- x86ReleaseStatic: will create the 32 bits version statically linking every third party library into a whole large plugin file.
- x86ReleaseZ3dyn: will create the 32 bits version statically linking every third party library but z3.lib.
- x64ReleaseStatic: will create the 64 bits version statically linking every third party library into a whole large plugin file.
- x64ReleaseZ3dyn: will create the 64 bits version statically linking every third party library but z3.lib.

The static version of ```z3.lib``` is ~ 1.1Gb and the linking time is considerable. That's the main reason why we have a building version that uses z3 dynamically (as dll).

## Usage
In this section we will list the different Ponce options and his shortcuts:
- Access the configuration and taint/symbolic windows: Edit > Ponce > Show Config (Ctl+Shift+P and Ctl+Alt+T)

![2016-09-15 11_39_08-configuracion](https://cloud.githubusercontent.com/assets/5193128/18563366/44a8c698-7b3c-11e6-8802-efb3fe4a5a2d.png)

- Enable/Disable Ponce tracing (Ctl+Shift+E)

![2016-09-15 11_31_34-](https://cloud.githubusercontent.com/assets/5193128/18563294/fd2cf992-7b3b-11e6-911f-c91c76804b5a.png)

- Symbolize/taint a register (Ctl+Shift+R)

![2016-09-15 11_32_32-](https://cloud.githubusercontent.com/assets/5193128/18563447/7e4db840-7b3c-11e6-813b-868bdae515bc.png)

- Symbolize/taint memory. Can be done from the IDA View or the Hex View (Ctl+Shift+M)

![2016-09-15 11_32_52-ida - testproject idb testproject exe c__users_default default-pc_documents_vi](https://cloud.githubusercontent.com/assets/5193128/18563458/88c5bb7e-7b3c-11e6-8b4e-f4a694cad5a8.png)![2016-09-15 11_33_10-taint _ symbolize memory range](https://cloud.githubusercontent.com/assets/5193128/18563460/8adbb8f0-7b3c-11e6-886f-02441bff63a4.png)

![2016-09-15 12_09_11-inicio](https://cloud.githubusercontent.com/assets/5193128/18563642/45860a7a-7b3d-11e6-9f95-e7aed529cc85.png)

- Solve formula (Ctl+Shift+S)

![2016-09-15 11_35_11-](https://cloud.githubusercontent.com/assets/5193128/18563556/e093f0c8-7b3c-11e6-9b37-b3b2c7111d57.png)

- Negate & Inject (Ctl+Shift+N)

![2016-09-15 11_34_44-](https://cloud.githubusercontent.com/assets/5193128/18563423/6db81160-7b3c-11e6-94a2-698ff334c024.png)

- Negate, Inject & Restore Snaphot (Ctl+Shift+I)

![2016-09-15 11_47_19-](https://cloud.githubusercontent.com/assets/5193128/18563350/34e0fd20-7b3c-11e6-8040-7e5899fc200f.png)
- Create Execution Snapshot (Ctl+Shift+C)

![2016-09-15 11_37_40-](https://cloud.githubusercontent.com/assets/5193128/18563529/cfc599c2-7b3c-11e6-84e1-5dd5c7b27537.png)

- Restore Execution Snapshot (Ctl+Shift+S)

![2016-09-15 11_38_10-](https://cloud.githubusercontent.com/assets/5193128/18563411/63cfeb50-7b3c-11e6-8f56-255bb27bc8f2.png)

- Delete Execution Snapshot (Ctl+Shift+D)

![2016-09-15 11_38_23-](https://cloud.githubusercontent.com/assets/5193128/18563385/53df1d42-7b3c-11e6-8c2f-f1bd16369f79.png)

- Execute Native (Ctl+Shift+F9)

![2016-09-15 12_07_10-](https://cloud.githubusercontent.com/assets/5193128/18563579/fc95339a-7b3c-11e6-9947-971e0510eba4.png)


## OS Support
Ponce currently works with IDA Pro in the Windows flavour for x86 and x64 binaries. 

You can still debug Linux and Mac OS X binaries with it using built-in IDA remote debugger. 
Ponce v0.2 will build native plugins for IDA Linux and IDA Mac OS X. Actually the code we wrote already takes it in consideration using macros for the different OS.

## FAQ
### Why the name of Ponce?
Juan Ponce de León (1474 – July 1521) was a Spanish  explorer and conquistador. He discovered Florida in the United States. The IDA plugin help you out to discover, explore and hopfully conquer the the different path options in a binary. 
### Can Ponce be used to analyze Windows, OS X and Linux binaries?
Yes, you can natively use Ponce in IDA for Windows or remotely attach to a Linux or OS X box and use it. In the next Ponce version we will support natively Ponce for Linux and OS X IDA versions. 
### I love your project! Can I collaborate?
Sure! Please do pull requests and work in the opened issues. We will pay beers for help ;)

## Authors
- Alberto Garcia Illera ([@algillera](https://twitter.com/algillera)) agarciaillera@gmail.com
- Francisco Oca ([@francisco_oca](https://twitter.com/francisco_oca)) francisco.oca.gonzalez@gmail.com

## Examples
### Use symbolic execution to solve a crackMe
Here we can see the use of the symbolic engine and how we can solve constrains:
- Passing simple ```aaaaa``` as argument.
- We fist select the symbolic engine.
- We convert to symbolic the memory pointed by argv[1] (```aaaaa```)
- Identify the symbolic condition that make us win and solve it.
- Test the solution.

![manual_symbolize_and_solve_crackme_hash2](https://cloud.githubusercontent.com/assets/5193128/18558235/32561cb2-7b27-11e6-846f-9fde03e88df5.gif)

### Negate and inject a condition
In the next gif we can see the use of automatic tainting and how we can negate a condition and inject it in memory while debugging:
- We select the symbolic engine and set the option to taint ```argv```.
- We identify the condition that needs to be satisfied to win the crackMe.
- We negate an inject the solution everytime a byte of our input is checked against the key.
- Finally we get the key ```elite``` that has been injected in memory and therefore reach the ```Win``` code.

![crackmexor_negate_and_inject](https://cloud.githubusercontent.com/assets/5193128/18558282/5dd1cbca-7b27-11e6-81d7-13044bfc0b59.gif)

## Using the tainting engine to track user controlled input
In this example we can see the use of the tainting engine with cmake. We are:
- Passing a file as argument to cmake to have him parsing it.
- We select we want to use the tainting engine
- We taint the buffer that ```fread````reads from the file.
- We resume the execution under the debugger control to see where the taint input is moved to.
- Ponce will rename the tainted functions. These are the functions that somehow the user has influence on, not the simply executed functions.

![cmake_tainting_fread](https://cloud.githubusercontent.com/assets/5193128/18558313/7aaa6d88-7b27-11e6-9c63-9870720d14e3.gif)

## Use Negate, Inject & Restore
In the next example we are using the snapshot engine:
- Passing a file as argument.
- We select we want to use the symbolic engine.
- We taint the buffer that ```fread````reads from the file.
- We create a snaphot in the function that parses the buffer read from the file.
- When a condition is evaluted we negate it, inject the solution in memory and restore the snapshot with it.
- The solution will be "valid" so we will satisfy the existent conditions.

![fread_test_negate_restore_inject](https://github.com/illera88/Ponce/blob/master/examples/animated_videos/x64_fread_test_negate_restore_inject.gif?raw=true)

## Limitations
Symbolic execution has some inherent problems:
- [Path explosion](https://en.wikipedia.org/wiki/Symbolic_execution#Path_Explosion)
- Symbolic indexing
