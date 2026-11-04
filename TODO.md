
# TODO
- Rendering
 
  - [ ] add Instanced GPU draw calls
  - [x] add Camera
  - [ ] add Instance Vertex Buffer Object
  - [ ] add Uniform Buffer Object
  - [ ] add multiples viewports
  - [ ] add a renderer that takes care of all the rendering stuff
  - [ ] add framebuffer objects
  - [x] Find why the camera feels off
  - [x] fix the mousedeltas not being initialized at first and causing flickering at the start
  - [x] fix the window crashing when minimizing (height = 0 so crashes aspectratio)

- Core
    - [ ] add a logger
    - [ ] eventually add a single background worker thread for asset loading.
    - [ ] remove the std::runtime errors where not needed to avoid closing the program

- QoL
  - [ ] write some documentation for the classes and objects
  - [ ] verify the code for each class ( for my future sanity )
  - [x] rewrite code with new naming conventions ///25% done