# Pass for removing optnone function annotations

## Build
```
mkdir build
cd build
cmake ..
```

## Test

```
clang -O0 -c -emit-llvm test/hello.c
llvm-dis test/hello.bc -o -
```

(Make sure optnone attributes appear in the output)

```
opt -load build/RemoveOptnone/RemoveOptnone.so -remove-optnone test/hello.bc -o test/hello2.bc
```

(On OS X use RemoveOptnone.dylib instead of RemoveOptnone.so)

```
llvm-dis test/hello2.bc -o -
```

(Make sure no optnone attributes appear in the output)

```
opt -O1 test/hello2.bc -S -o -
```

(Check that main() returns the constant 0)
