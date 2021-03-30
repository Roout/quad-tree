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

## Install

```bash
# 1. clone repo with sfml
git clone https://github.com/Roout/quad-tree.git
git submodule init SFML
git submodule update

# 2. configure and generate sfml project with output directory `external/build` with CMake
# by providing generator with enabled shared library flag

# 3. build sfml with `make`
cd external/build
make # nmake

# 4. build quad-tree target
cd ../.. # go to project root 
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Prerequisites

> - SFML 2.5.1 (older ones can be used too so just set up dependencies and libraries)
> - c++17

## Controls

The controls are described in the applications:

- Insert point into the tree: press **SPACE**
- Select one or more points from the workspace: use **MOUSE** (press left button and drag)
- Erase selected points: select points and press **F**
