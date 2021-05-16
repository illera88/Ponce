# How to debug Ponce under Windows
If you want to debug Ponce and being able to step into functions and put breakpoints you should do the following:

- Make sure you set the `IDA_INSTALLED_DIR` parameter when building with CMake.
- Secondly you should open the IDA binary under Visual Studio. Do File -> Open -> Project and select the `C:\Program Files\IDA Pro 7.5\ida64.exe` binary.
- In the properties of the project you can set the arguments to the binary you IDA to run
- Now right click on the solution and go to Add -> Exixting Project and select the `Ponce64` project.
- Remember to compile Ponce on Debug so the breakpoints get hit