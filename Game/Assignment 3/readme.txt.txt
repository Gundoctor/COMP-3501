COMP 3501A
Assignment 3
Yugo Brunet
100853306


Environment:
Windows 8.1 Pro
Visual Studio 2013 Ultimate

Template:
Windows Developer Content  DirectX game learning template

Requirements:
All lines of code refer to Sample3DSceneRenderer.cpp

1. Collision detection, screen flashes red when ship collides with debris: lines 136, 558, 897
2. Laser beam added to system. Replaced with Bonus 1.
3. Missile collision detection: Replaced with Bonus 2
4. Hierarchical missle orientation: lines 496-557 
   Targeting cube: lines 588-632, 635-764

Bonus:
1. Dual Missiles: lines 447-484, 496-528
2. Missile Fire with collision detection: lines 442-556, 152-194


Instructions:
W - Pitch up
A - Yaw left
S - Pitch down
D - Yaw right
Q - Roll left
E - Roll right

Left Arrow Key - Move Backwards
Right Arrow Key - Move Forwards

Spacebar - Fire missiles


Features:
Spaceship flies backwards when hit.
Missiles can only be fired at a rate based on the coolDownTime variable in Sample3DSceneRenderer.h


Sources:
Code based off Dr. Mould's "quaternion camera demo" from the cuLearn webpage.