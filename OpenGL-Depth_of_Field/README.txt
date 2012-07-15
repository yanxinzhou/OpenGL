Erik Paluka. Winter 2012.

Simulating Depth-of-Field

Implemented in C++ using OpenGL.

My algorithm produces a depth of field by simulating a pin hole camera with an aperture size, 
and jittering the camera focused on a certain focal plane. By jittering the camera, it produces 
a blurring effect that is more pronounced on the images farther away from the focal plane. 
I do this by drawing the scene repeatedly with a different camera position each time. 
The size of the aperture determines how different each camera position will be. 
If it is 0.0, the camera positions will all be in the same location causing no blur. 
With an aperture size greater than 0.0, the camera positions will vary and with a larger aperture size, 
the camera positions will vary a lot more causing a lot more blur. The results of all the renderings are averaged using the accumulation buffer. 

The algorithm loops through the number of times that you want it to repeatedly draw the scene. I chose to draw the scene 8 times because 
this number creates a better quality image. Inside that loop, you clear the colour buffer, change the view, draw the 
objects, average the objects using the accumulation buffer, and then return the accumulation buffer and draw it to the 
screen. When I change the view, I set the field of view, aspect ratio, near and far planes, and then I set the camera. 
I use “gluLookAt()” to set the camera at a position that is varied slightly by an offset. The offset is calculated by multiplying 
the count of the outer loop, the aperture size and a constant that is empirically determined. How I set the camera is 
how I am jittering the camera. This creates a depth of field that depends on the aperture size of the simulated pin hole camera. 

To create this algorithm, I used my knowledge gained from the “Advanced Computer Graphics” course at UOIT, and 
reading from the OpenGL “Red book” – (Shreiner D., Woo M., Neider J., Davis T., Addison –Wesley 2006, OpenGL Programming Guide Fifth Edition).