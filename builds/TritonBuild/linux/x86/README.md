# How to build Triton in linux
Execute the following commands:
```
rm -rf build/
mkdir build
cd build
cmake -DCAPSTONE_INCLUDE_DIRS=../external-libs/includes/capstone-3.0.4-linux/include/ -DBoost_INCLUDE_DIR=../external-libs/includes/boost_1_60_0/ -DZ3_INCLUDE_DIRS=../external-libs/includes/z3-4.4.2-linux86/include/ -DSTATICLIB=on -DPYTHON_BINDINGS=off -DTARGET=ia32 ../../../../../Triton/
make
```
