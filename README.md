## Advent of code 2022
repository of sources for my solutions

### Building
```shell
$ cmake -B build -DCMAKE_BUILD_TYPE=Release .
$ cmake --build build
```

binaries are then in `build` and inputs are stored in `input/day<NN>`

if you want debug prints:

```shell
$ cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug .
$ cmake --build build-debug
```

you can add `-DCMAKE_EXPORT_COMPILE_COMMANDS` to the debug build generation
command above. `.clangd` is set up to look for a compilation database in
`build-debug`.
