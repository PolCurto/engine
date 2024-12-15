# Assignment 1 Engine
In-house engine made from scratch with C++ in the UPC School Advanced Programming for AAA games master's degree.

## How to use
#### Move the camera around
* Mouse wheel to zoom in/out.
* Mouse wheel press to drag around the scene.
* [ALT] + Left Click to orbit around the current geometry with mouse motion.
* [ALT] + Right Click to zoom in/out with mouse vertical motion.
* [F] to focus on the current geometry.
* Right click + [W][A][S][D] to move around, [Q][E] to move up/down in the Y axis. In this mode the mouse motion moves the camera around

#### Menus
In the main menu bar there are three sections:
* **General:** In this section you can toggle the properties of the loaded geometry, the console and quit the program.
* **Settings:** This secton toggles the settings window. In this window there is a graph with the ms and fps, there are settings for different modules and there is information about the hardware.
* **Help:** In this section you can toggle the ImGui demo window, there is a link to the engine documentation, release and report a bug pages, and there is also an About window which shows information about the engine and the versions of the external libraries.

## Additional functionality
I added a function in the Model class to read the nodes section of the .gltf file, which includes the model matrix and the children of each mesh. I have only added the functionality to read the matrices and apply it to the meshes in my engine.
Using this, I have also added a section in the modules configuration window to move the loaded model around the scene.

## Additional comments
For each mesh I calculate its bounding box by saving its maximum and minimum position in each axis. This way when an object is focused it knows at which distance to place the camera depending of its size. 
Also it is saved the position of its pivot(which is the transform part of the model matrix) and its local position, which is the center of the bounding box.