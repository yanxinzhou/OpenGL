Erik Paluka. Winter 2012.

Travelling Through a Scene - An Interactive Movie

Implemented in C++ using OpenGL.

This project creates a winter scene consisting of an infinite path, a moving lake, snowflakes falling from the sky, and trees. The journey begins at dusk and as the user travels through the scene, the sun rises (lighting up the scene) and eventually turns to dusk once again. The user is given limited control of the camera for exploration through the scene and can adjust the speed of travel.

In the first part of the movie, the trees are rendered using the billboarding technique. Before the movie begins, a tree is rendered from different angles to the back buffer. The back buffer is read after each rendering to create textures of the tree at different angles. These textures are then used to create the trees in the scene. The view direction of the user determines which texture is used to render each tree. This creates the illusion that the trees are three dimensional. In the second part of the movie, all the trees are procedurally drawn.

Features
--------------- 

-> Particle System (Snow)

-> Sky Box

-> Billboarding

-> L-Systems

-> User can control the number of trees to render, and if the path should be straight or curve back and forth

-> Scene is rendered as a giant cylinder to create an infinite path