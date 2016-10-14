# How to build libtriton in mac:

You need to use the gcc compiler. We want to build it statically and without the python bindings.

So we need this configuration:
```
mkdir build
cd build
mkdir x86
cd x86
cmake ../../../../../Triton/ -DBOOST_ROOT=../external-libs/includes/boost_1_60_0/ -DCMAKE_C_COMPILER=/usr/local/Cellar/gcc/6.2.0/bin/gcc-6 -DCMAKE_CXX_COMPILER=/usr/local/Cellar/gcc/6.2.0/bin/g++-6 -DPYTHON_BINDINGS=off -DCMAKE_OSX_ARCHITECTURES=i386 -DZ3_INCLUDE_DIRS=../external-libs/includes/z3-4.4.2-linux86/include/ -DCAPSTONE_INCLUDE_DIRS=../external-libs/includes/capstone-3.0.4-linux/include/ -DSTATICLIB=on
```
And after that you can do a make:
```
make
```

Remember, run this command from build.

More info here: https://github.com/JonathanSalwan/Triton/issues/243#issuecomment-239086935
