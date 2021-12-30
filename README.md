# QuadTree

[QuadTree](https://en.wikipedia.org/wiki/Quadtree) implementation with visiulization using SFML.
QuadTree supports following operations:

- [x] Insert point
- [x] Erase point
- [x] Find the provided point in the tree
- [x] Query points from the selected rectangular area
- [x] Apply visitor(can modify node) to each node in the tree
- [ ] Find point closest to the given point

<img src="https://github.com/Roout/quad-tree/blob/master/docs/quadtree.gif" width="1000" height="600" />

## Quick Start

```bash
# clone repo with submodules
git clone --recurse-submodules git@github.com:Roout/quad-tree.git quad-tree
cd quad-tree/external

# build SFML submodule with cached values
mkdir SFML-build && cd SFML-build
# you also may specify generator with -G "..." option
cmake -C ../CMakeCache.txt ../SFML
cmake --build . --config Release

# go back to the project root
cd ../.. 
# build the whole project
mkdir build && cd build
# configure
# you also may specify generator with -G "..." option
cmake ..
# build
cmake --build . --config Release
```

## Prerequisites

> - SFML 2.5
> - c++17
> - CMake 3.15

## Controls

The controls are described in the applications:

- Insert point into the tree: press right button of **MOUSE**
- Select one or more points from the workspace: use **MOUSE** (press left button and drag)
- Erase selected points: select points and press **F**
