# How to debug Ponce under Windows
If you want to debug Ponce and being able to step into functions and put breakpoints you should do the following:

- Make sure you set the `IDA_INSTALLED_DIR` parameter when building with CMake.
- Secondly you should open the IDA binary under Visual Studio. Do File -> Open -> Project and select the `C:\Program Files\IDA Pro 7.5\ida64.exe` binary.
- 