1. How to build the project

- Please set "Physics_Project01" project as a starter project and build with x64 configuration / Debug or Release mode. Or you can execute with "Physics_Project01.exe" file in x64\Debug(or Release) folder.

- GitHub Link:
https://github.com/kanious/Physics2_Project01



2. User Input Option

< Targeting >
* Tab : Change controllable target in order.
        If current target is the last, the camera will look at the starting point (0, 0, 0).
* F1 : Removes target and make the camera look at the starting point.

< Moving >
* WASD : If there is a target object, move it. (look/right direction of camera)
* Space : Move the target to upside (0, 1, 0).
* F2 : Reset all Rigidbodies status.
* F3 : Apply random force to all Rigidbodies.

< Looking >
* Move Mouse : Rotate camera angle by Y axis.
* Roll Mouse scroll : Zoom In/Out.

< Etc >
* ESC : Close the program



3. Description
- Physics related code location :
  PumpkinEngine > 05.IndependantFunctions > Physics filter
  Physics_Project01 > 01.Scene / 02.GameObject filters (SceneDungeon / BGObject / DefaultCamera)

- A collision sound only plays when a sphere collides with a sphere.


