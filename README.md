# Pass for removing optnone function annotations

## Build
```
mkdir build
cd build
cmake ..
make
cd ..
```

## Test

```
clang -O0 -c -emit-llvm test/hello.c
llvm-dis hello.bc -o -
```

(Make sure optnone attributes appear in the output)

```
opt -load build/RemoveOptnone/RemoveOptnone.so -RemoveOptnone hello.bc -o hello2.bc
```

(On OS X use RemoveOptnone.dylib instead of RemoveOptnone.so)

```
llvm-dis hello2.bc -o -
```

(Make sure no optnone attributes appear in the output)

```
opt -O1 hello2.bc -S -o -
```

(Check that main() returns the constant 0)
