---
description: IDA 2016 plugin contest winner! Symbolic Execution just one-click away!
---

# Ponce

![](https://cloud.githubusercontent.com/assets/5193128/18534105/f27da220-7a9c-11e6-8d2e-a940d94b404b.png)

[![Build for Windows](https://github.com/illera88/Ponce/workflows/Build%20for%20Windows/badge.svg)](https://github.com/illera88/Ponce/actions?query=workflow%3A%22Build+for+Windows%22) [![Build for OSX](https://github.com/illera88/Ponce/workflows/Build%20for%20OSX/badge.svg)](https://github.com/illera88/Ponce/actions?query=workflow%3A%22Build+for+OSX%22) [![Build for Linux](https://github.com/illera88/Ponce/workflows/Build%20for%20Linux/badge.svg)](https://github.com/illera88/Ponce/actions?query=workflow%3A%22Build+for+Linux%22)

## Ponce

Ponce \(pronounced _\[ 'poN θe \]_ pon-they \) is an IDA Pro plugin that provides users the ability to perform taint analysis and symbolic execution over binaries in an easy and intuitive fashion. With Ponce you are one click away from getting all the power from cutting edge symbolic execution. Entirely written in C/C++.

For a quick overview of how Ponce works checkout the examples on the left panel, we have some animated gifs so you can see what Ponce is capable of!

### Why?

Symbolic execution is not a new concept in the security community. It has been around for many years but it is not until around 2015 that open source projects like [Triton](https://github.com/JonathanSalwan/Triton) and [Angr](http://angr.io/) have been created to address this need. Despite the availability of these projects, end users are often left to implement specific use cases themselves.

We addressed these needs by creating Ponce, an IDA plugin that implements symbolic execution and taint analysis within the most used disassembler/debugger for reverse engineers.

### Use cases

* **Exploit development**: Ponce can help you create an exploit in a far more efficient manner as the exploit developer may easily see what parts of memory and which registers you control, as well as possible addresses which can be leveraged as ROP gadgets.
* **Malware Analysis**: Another use of Ponce is related to malware code. Analyzing the commands a particular family of malware supports is easily determined by symbolizing a simple known command and negating all the conditions where the command is being checked. 
* **Protocol Reversing**: One of the most interesting Ponce uses is the possibility of recognizing required magic numbers, headers or even entire protocols for controlled user input. For instance, Ponce can help you to list all the accepted arguments for a given command line binary or extract the file format required for a specific file parser.
* **CTF**: Ponce speeds up the process of reverse engineer binaries during CTFs. As Ponce is totally integrated into IDA you don't need to worry about setup timing. It's ready to be used!

The plugin will automatically run, guiding you through the initial configuration the first time it is run. The configuration will be saved to a configuration file so you won't have to worry about the config window again.

### Installation

Ponce works with both x86 and x64 binaries in any IDA version &gt;= 7.0 for Windows, Linux and MacOs. Installing the plugin is as simple as copying the appropiate files from the [latest builds](https://github.com/illera88/Ponce/tree/master/latest_builds) to the `plugins\` folder in your IDA installation directory.

Make sure you use the Ponce binary compiled for your IDA version to avoid any incompatibilities.

### How to use Ponce?

The examples section is a good way to see how Ponce works. If you want a more detailed information check out the Usage section, we explained there the different options.

Also make sure to review the [limitations](misc/ponce-limitations.md) before you start using Ponce.

### Triton

Ponce relies on the [Triton framework](https://github.com/JonathanSalwan/Triton) to provide semantics, taint analysis and symbolic execution. Triton is an awesome Open Source project sponsored by Quarkslab and maintained mainly by [Jonathan Salwan](http://shell-storm.org/) with a rich library. We would like to thank and endorse Jonathan's work with Triton. You rock! :)

### Authors

* Alberto Garcia Illera ([@algillera](https://twitter.com/algillera)) agarciaillera@gmail.com
* Francisco Oca ([@francisco\_oca](https://twitter.com/francisco_oca)) francisco.oca.gonzalez@gmail.com