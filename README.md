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

### 1. Clone repo with SFML

```bash
git clone https://github.com/Roout/quad-tree.git
cd quad-tree/external
git submodule init SFML
git submodule update
```

### 2. Build SFML

- Configure and generate SFML project with cmake gui or command line tool.
- Set up output directory: `external/build`
- Enable `-DBUILD_SHARED_LIBS=ON -DSFML_BUILD_GRAPHICS=ON -DSFML_BUILD_WINDOW=ON`
- Build SFML with `make`, `nmake` withing the `external/build` directory

### 3. Build quad-tree target

- Note, that you should use same CMAKE_CXX_COMPILER as used for SFML build
- 
```bash
# go to project root 
cd ../.. 
# create build folder
mkdir build
cd build
# configure and generate quad-tree project with cmake gui or cmake command line tool
# using same compiler
...
# use make or nmake in build directory
make 
```

## Prerequisites

> - SFML 2.5
> - c++17
> - CMake 3.15

## Controls

The controls are described in the applications:

- Insert point into the tree: press **SPACE**
- Select one or more points from the workspace: use **MOUSE** (press left button and drag)
- Erase selected points: select points and press **F**
