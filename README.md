# Ponce

<p align="center"><img src ="https://cloud.githubusercontent.com/assets/5193128/18534105/f27da220-7a9c-11e6-8d2e-a940d94b404b.png" /></p>
 
## Definition
Ponce (pronuntiated *[ 'poN θe ]* pon-they ) is an IDA Pro plugin that provides users the ability to perform taint analysis and symbolic execution over binaries in an easy and intuitive fashion. The plugin is entirely written in C/C++.
Ponce relies on  the [Triton framework](https://github.com/JonathanSalwan/Triton) to provide semantics and ... 

## Why?
Symbolic execution is not a new concept in the security community. It has been around for years but it is not until the last couple of years when the open source projects like [Triton](https://github.com/JonathanSalwan/Triton) or [Angr](http://angr.io/) have been created to address this need. Despite the availability of these projects, end users often are left to implement specific use cases themselfs. 

We addressed these needs by creating Ponce, an IDA plugin that integrates symbolic and tainting engines withing the most used dissasembler/debugger by reverse engineers.

#Triton
Ponce relies on  the [Triton framework](https://github.com/JonathanSalwan/Triton) to provide semantics, taint analysis and symbolic execution. Triton is an awesome Open Source project sponsored by Quarkslab and maintained mainly by [Jonathan Salwan]() with a rich library. 

## Use modes
- **Tainting engine**: This engine is used to know at each program point what part of memory and register are controllable by the user input.
- **Symbolic engine**: This engine maintains a symbolic states of registers and part of memory at each program point. 

## Use cases
- **Exploit development**: Ponce can help you creating an exploit since you can easily see what parts of memory and registers you control and posible addresses you can use as ROP gadgets.
- **Malware Analysis**: Other use of Ponce is related with malware code. Analyzing the commands that family of malware support can be fastly determined symbolizing a simple known command and negating all the conditions where the command is being checked. 
- **Protocol Reversing**: One of the most interesting Ponce uses is the possibility of recognizing required magic numbers, headers or even entire protocols for controlled user input. For instance, Ponce can help you to list all the accepted arguments for a given command line binary or extracting the file format required for a specific file parser.

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

## OS Support
Ponce currently works with IDA Pro in the Windows flavour for x86 and x64 binaries. 

You can still debug Linux and Mac OS X binaries with it using built-in IDA remote debugger. 
Ponce v0.2 will build native plugins for IDA Linux and IDA Mac OS X. Actually the code we wrote already takes it in consideration using macros for the different OS.

## FAQ
### Why the name of Ponce?
fdfd
### otro
## Authors
- Alberto Garcia Illera ([@algillera](https://twitter.com/algillera)) agarciaillera@gmail.com
- Francisco Oca ([@francisco_oca](https://twitter.com/francisco_oca)) francisco.oca.gonzalez@gmail.com

## Examples
### Use symbolic execution to solve a crackMe
![manual_symbolize_and_solve_crackme_hash2](https://cloud.githubusercontent.com/assets/5193128/18558235/32561cb2-7b27-11e6-846f-9fde03e88df5.gif)

### Negate and inject a condition
![crackmexor_negate_and_inject](https://cloud.githubusercontent.com/assets/5193128/18558282/5dd1cbca-7b27-11e6-81d7-13044bfc0b59.gif)

## Using the tainting engine to track user controlled input
![cmake_tainting_fread](https://cloud.githubusercontent.com/assets/5193128/18558313/7aaa6d88-7b27-11e6-9c63-9870720d14e3.gif)

## Limitations
Path explosion




IDA Script to use symbolic execution and taint analysis through IDA PRO.

Installation process

