<p align="center">
  <img src="https://cloud.githubusercontent.com/assets/5193128/18534105/f27da220-7a9c-11e6-8d2e-a940d94b404b.png">
</p>

<p align="center">
  <a href="https://github.com/illera88/Ponce/actions?query=workflow%3A%22Build+for+Windows%22"><img alt="Build for Windows" src="https://github.com/illera88/Ponce/workflows/Build%20for%20Windows/badge.svg"></a>
  <a href="https://github.com/illera88/Ponce/actions?query=workflow%3A%22Build+for+OSX%22"><img alt="Build for OSX" src="https://github.com/illera88/Ponce/workflows/Build%20for%20OSX/badge.svg"></a>
  <a href="https://github.com/illera88/Ponce/actions?query=workflow%3A%22Build+for+Linux%22"><img alt="Build for Linux" src="https://github.com/illera88/Ponce/workflows/Build%20for%20Linux/badge.svg"></a>
</p>

# Ponce

Ponce \(pronounced _\[ 'poN θe \]_ pon-they \) is an IDA Pro plugin that provides users the ability to perform taint analysis and symbolic execution over binaries in an easy and intuitive fashion. With Ponce you are one click away from getting all the power from cutting edge symbolic execution. Entirely written in C/C++.

### Why?

Symbolic execution is not a new concept in the security community. It has been around for many years but it is not until around 2015 that open source projects like [Triton](https://github.com/JonathanSalwan/Triton) and [Angr](http://angr.io/) have been created to address this need. Despite the availability of these projects, end users are often left to implement specific use cases themselves.

We addressed these needs by creating Ponce, an IDA plugin that implements symbolic execution and taint analysis within the most used disassembler/debugger for reverse engineers.

### Installation

Ponce works with both x86 and x64 binaries in any IDA version &gt;= 7.0. Installing the plugin is as simple as copying the appropiate files from the [latest builds](https://github.com/illera88/Ponce/releases/latest) to the `plugins\` folder in your IDA installation directory.

Make sure you use the Ponce binary compiled for your IDA version to avoid any incompatibilities.

### OS Support

Ponce works on Windows, Linux and OSX natively!

### Use cases

* **Exploit development**: Ponce can help you create an exploit in a far more efficient manner as the exploit developer may easily see what parts of memory and which registers you control, as well as possible addresses which can be leveraged as ROP gadgets.
* **Malware Analysis**: Another use of Ponce is related to malware code. Analyzing the commands a particular family of malware supports is easily determined by symbolizing a simple known command and negating all the conditions where the command is being checked. 
* **Protocol Reversing**: One of the most interesting Ponce uses is the possibility of recognizing required magic numbers, headers or even entire protocols for controlled user input. For instance, Ponce can help you to list all the accepted arguments for a given command line binary or extract the file format required for a specific file parser.
* **CTF**: Ponce speeds up the process of reverse engineer binaries during CTFs. As Ponce is totally integrated into IDA you don't need to worry about setup timing. It's ready to be used!

The plugin will automatically run, guiding you through the initial configuration the first time it is run. The configuration will be saved to a configuration file so you won't have to worry about the config window again.

### Use modes

* **Tainting engine**: This engine is used to determine at every step of the binary's execution which parts of memory and registers are controllable by the user input.
* **Symbolic engine**: This engine maintains a symbolic state of registers and part of memory at each step in a binary's execution path. 

### Examples

#### Negate and inject a condition

In the next gif we can see the use of automatic tainting and how we can negate a condition and inject it in memory while debugging:

* We select the symbolic engine and set the option to symbolize `argv`.
* We identify the condition that needs to be satisfied to win the crackMe.
* We negate an inject the solution everytime a byte of our input is checked against the key.
* Finally we get the key `elite` that has been injected in memory and therefore reach the `Win` code.

 The crackme source code can be found [here](https://github.com/illera88/Ponce/blob/master/examples/crackme_xor.cpp)

![crackmexor\_negate\_and\_inject](https://cloud.githubusercontent.com/assets/5193128/18558282/5dd1cbca-7b27-11e6-81d7-13044bfc0b59.gif)

#### Using the tainting engine to track user controlled input

In this example we can see the use of the tainting engine with cmake. We are:

* Passing a file as argument to cmake to have him parsing it.
* We select we want to use the tainting engine
* We taint the buffer that \`\`\`fread\(\)\`\`\`\` reads from the file.
* We resume the execution under the debugger control to see where the taint input is moved to.
* Ponce will rename the tainted functions. These are the functions that somehow the user has influence on, not the simply executed functions.

![cmake\_tainting\_fread](https://cloud.githubusercontent.com/assets/5193128/18558313/7aaa6d88-7b27-11e6-9c63-9870720d14e3.gif)

#### Use Negate, Inject & Restore

In the next example we are using the snapshot engine:

* Passing a file as argument.
* We select we want to use the symbolic engine.
* We taint the buffer that \`\`\`fread\(\)\`\`\`\` reads from the file.
* We create a snapshot in the function that parses the buffer read from the file.
* When a condition is evaluated we negate it, inject the solution in memory and restore the snapshot with it.
* The solution will be "valid" so we will satisfy the existent conditions.

![fread\_test\_negate\_restore\_inject](https://github.com/illera88/Ponce/tree/780e1992a935d310f5a956e6ece6b8f630a853a7/media/x64_fread_test_negate_restore_inject.gif?raw=true) The example source code can be found [here](https://github.com/illera88/Ponce/blob/master/examples/fread_SAGE.cpp)

### Usage

In this section we will list the different Ponce options as well as keyboard shortcuts:

* Access the configuration and taint/symbolic windows: Edit &gt; Ponce &gt; Show Config \(Ctl+Shift+P and Ctl+Alt+T\)

![2016-09-15 11\_39\_08-configuracion](https://cloud.githubusercontent.com/assets/5193128/18563366/44a8c698-7b3c-11e6-8802-efb3fe4a5a2d.png)

* Enable/Disable Ponce tracing \(Ctl+Shift+E\)

![2016-09-15 11\_31\_34-](https://cloud.githubusercontent.com/assets/5193128/18563294/fd2cf992-7b3b-11e6-911f-c91c76804b5a.png)

* Symbolize/taint a register \(Ctl+Shift+R\)

![2016-09-15 11\_32\_32-](https://cloud.githubusercontent.com/assets/5193128/18563447/7e4db840-7b3c-11e6-813b-868bdae515bc.png)

* Symbolize/taint memory. Can be done from the IDA View or the Hex View \(Ctl+Shift+M\)

![2016-09-15 11\_32\_52-ida - testproject idb testproject exe c\_\_users\_default default-pc\_documents\_vi](https://cloud.githubusercontent.com/assets/5193128/18563458/88c5bb7e-7b3c-11e6-8b4e-f4a694cad5a8.png)![2016-09-15 11\_33\_10-taint \_ symbolize memory range](https://cloud.githubusercontent.com/assets/5193128/18563460/8adbb8f0-7b3c-11e6-886f-02441bff63a4.png)

![2016-09-15 12\_09\_11-inicio](https://cloud.githubusercontent.com/assets/5193128/18563642/45860a7a-7b3d-11e6-9f95-e7aed529cc85.png)

* Solve formula \(Ctl+Shift+S\)

![2016-09-15 11\_35\_11-](https://cloud.githubusercontent.com/assets/5193128/18563556/e093f0c8-7b3c-11e6-9b37-b3b2c7111d57.png)

* Negate & Inject \(Ctl+Shift+N\)

![2016-09-15 11\_34\_44-](https://cloud.githubusercontent.com/assets/5193128/18563423/6db81160-7b3c-11e6-94a2-698ff334c024.png)

* Negate, Inject & Restore Snaphot \(Ctl+Shift+I\)

![2016-09-15 11\_47\_19-](https://cloud.githubusercontent.com/assets/5193128/18563350/34e0fd20-7b3c-11e6-8040-7e5899fc200f.png)

* Create Execution Snapshot \(Ctl+Shift+C\)

![2016-09-15 11\_37\_40-](https://cloud.githubusercontent.com/assets/5193128/18563529/cfc599c2-7b3c-11e6-84e1-5dd5c7b27537.png)

* Restore Execution Snapshot \(Ctl+Shift+S\)

![2016-09-15 11\_38\_10-](https://cloud.githubusercontent.com/assets/5193128/18563411/63cfeb50-7b3c-11e6-8f56-255bb27bc8f2.png)

* Delete Execution Snapshot \(Ctl+Shift+D\)

![2016-09-15 11\_38\_23-](https://cloud.githubusercontent.com/assets/5193128/18563385/53df1d42-7b3c-11e6-8c2f-f1bd16369f79.png)

* Execute Native \(Ctl+Shift+F9\)

![2016-09-15 12\_07\_10-](https://cloud.githubusercontent.com/assets/5193128/18563579/fc95339a-7b3c-11e6-9947-971e0510eba4.png)

### Triton

Ponce relies on the [Triton framework](https://github.com/JonathanSalwan/Triton) to provide semantics, taint analysis and symbolic execution. Triton is an awesome Open Source project sponsored by Quarkslab and maintained mainly by [Jonathan Salwan](http://shell-storm.org/) with a rich library. We would like to thank and endorse Jonathan's work with Triton. You rock! :\)

### Building

Since Ponce v0.3 we have moved the building compilation process to use `CMake`. Doing this we unify the way that configuration and building happens for Linux, Windows and OSX. We now support providing feedback on the pseudocode about symbolic or taint instructions. For this feature to work you need to add `hexrays.hpp` to your IDA SDK include folder. `hexrays.hpp` can be found on `plugins/hexrays_sdk/` on your IDA installation path. If you have not purchased the hex-rays decompiler you can still build Pnce by using `-DBUILD_HEXRAYS_SUPPORT=OFF`. We use Github actions as our CI environment. Check the [action files](https://github.com/illera88/Ponce/tree/master/.github/workflows) if you want to understand how the building process happens.

### FAQ

#### Why the name of Ponce?

Juan Ponce de León \(1474 – July 1521\) was a Spanish explorer and conquistador. He discovered Florida in the United States. The IDA plugin will help you discover, explore and hopefully conquer the different paths in a binary.

#### Can Ponce be used to analyze Windows, OS X and Linux binaries?

Yes, you can natively use Ponce in IDA for Windows or remotely attach to a Linux or OS X box and use it. In the next Ponce version we will natively support Ponce for Linux and OS X IDA versions.

#### How many instructions per second can handle Ponce?

In our tests we reach to process 3000 instructions per second. We plan to use the PIN tracer IDA offers to increase the speed.

#### Something is not working!

Open an [issue](https://github.com/illera88/Ponce/issues), we will solve it ASAP ;\)

#### I love your project! Can I collaborate?

Sure! Please do pull requests and work in the opened issues. We will pay you in beers for help ;\)

### Limitations

Concolic execution and Ponce have some problems:

* Symbolic memory load/write: When the index used to read a memory value is symbolic like in `x = aray[symbolic_index]` some problems arise that could lead on the loose of track of the tainted/symbolized user controled input. 
* Triton doesn't work very well with [floating point instructions](https://github.com/illera88/Ponce/issues/59).
* Concolic execution only analyzed the executed instructions. That means that symbolic tracking is lost in cases like the following:

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

### Authors

* Alberto Garcia Illera \([@algillera](https://twitter.com/algillera)\) agarciaillera@gmail.com
* Francisco Oca \([@francisco\_oca](https://twitter.com/francisco_oca)\) francisco.oca.gonzalez@gmail.com

