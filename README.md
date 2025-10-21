# gammaShade
## Table of contents
**1.** [Dependencies]()  
**2.** [Build]()  

## Dependencies
### Windows
Everything should work fine with just xmake

### Linux
Before building anything, you must first install some dependencies for SDL3 : https://wiki.libsdl.org/SDL3/README-linux#build-dependencies.

### MacOS
TODO

## Build
```bash
# Debug 
# Not that by default, the debug mode runs WITHOUT OPTIMIZATIONS to keep variables' contents intact
xmake config --mode=debug    # or xmake f -m debug
xmake build    # or just xmake
xmake run    # or to run with the debugger, xmake run --debug or xmake run -d

# Release
xmake config --mode=release    # or xmake f -m release
xmake build    # or just xmake
xmake run
```
