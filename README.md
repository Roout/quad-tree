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
git clone --recurse-submodules git@github.com:Roout/quad-tree.git quadtree
mkdir build && cd build
# configure
# you also may specify generator with -G "..." option
cmake ../quadtree
# build
cmake --build . --config Release
```

## Prerequisites

> - SFML 2.5
> - c++17
> - CMake 3.17

## Controls

The controls are described in the applications:

- Insert point into the tree: press right button of **MOUSE**
- Select one or more points from the workspace: use **MOUSE** (press left button and drag)
- Erase selected points: select points and press **F**
