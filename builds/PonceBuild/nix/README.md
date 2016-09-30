# Build Ponce
You need to compile Ponce with GCC, so first you need to set some enviroment variables:
```
export __MAC__=1
export CC=/usr/local/Cellar/gcc/6.2.0/bin/gcc-6
export CXX=/usr/local/Cellar/gcc/6.2.0/bin/g++-6
```

After that you need to configure with the cmake the build, you can choose the IDA version:
* IDA68: for IDA 6.8
* IDA69: for IDA 6.9 or IDA 6.95

And the architecture:
* x86 or x64

These are the four options:
```
cmake .. -DIDA_ARCH=x86 -DIDA_VER=68
cmake .. -DIDA_ARCH=x64 -DIDA_VER=68
cmake .. -DIDA_ARCH=x86 -DIDA_VER=69
cmake .. -DIDA_ARCH=x64 -DIDA_VER=69
```

And to build the plugin just do a make:
```
make
```


If you want to compile the static libraries we provide, Capstone and Z3. You need to follow these instructions:

# Build Z3
You need to compile it with GCC and manually disable sse extensions

So first set GCC:
```
export CC=/usr/local/Cellar/gcc/6.2.0/bin/gcc-6
export CXX=/usr/local/Cellar/gcc/6.2.0/bin/g++-6
```

Now build the project as a staticlib, without omp support and for x86 (IDA & IDA64 are x86 binaries):
```
python scripts/mk_make.py --staticlib --noomp --x86
```

Now you need to edit the build/config.mk and modify these lines:
```
CXXFLAGS= -D_MP_INTERNAL -DNDEBUG -D_EXTERNAL_RELEASE  -fvisibility=hidden -c -D_NO_OMP_ -O0 -D _EXTERNAL_RELEASE -arch i386
LINK_EXTRA_FLAGS=-lpthread -arch i386
SLINK_FLAGS=-dynamiclib -arch i386
```

After that you can do a `make`.

# Build Capstone
Set the gcc compiler:
```
export CC=/usr/local/Cellar/gcc/6.2.0/bin/gcc-6
export CXX=/usr/local/Cellar/gcc/6.2.0/bin/g++-6
```
And make it for x86:
```
./make.sh nix32
```
