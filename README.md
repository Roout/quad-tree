# QuadTree
[QuadTree](https://en.wikipedia.org/wiki/Quadtree) implementation with visiulization using SFML.
QuadTree supports following operations:

- [x] insert point
- [x] erase point
- [x] find the provided point in the tree
- [x] query points from the selected rectangular area
- [x] apply visitor(can modify node) to each node in the tree
- [ ] find point closest to the given point

<img src="https://github.com/Roout/quad-tree/blob/master/about/quadtree.gif" width="800" height="600" />

## Prerequisites

> * SFML 2.5.1 (older ones can be used too so just set up dependencies and libraries)
> * c++17

## Controls

The controls are described in the applications:
- Insert point into the tree: press **SPACE**
- Select one or more points from the workspace: use **MOUSE** (press left button and drag)
- Erase selected points: select points and press **F**
