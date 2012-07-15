Erik Paluka. Winter 2012.

Midpoint Displacement Algorithm

Implemented in C++ using OpenGL.

I read in the file, which includes the starting position, step size between each sample, number of samples, and the height values for those samples. Using the starting position and step size, I create a vertex structure for each sample, which encapsulate its own y-coordinate, x-coordinate and its height (z-coordinate). Then (repeat this for the number of midpoint algorithm steps given), I iterate through those vertices, and create another vertex in between each vertex and its neighbour. (The midpoint between each vertex and its neighbour becomes a new vertex). The height of the new vertex is the sum of the heights of the neighbouring vertices divided by the number of neighbouring vertices. I also add a small random value to the height for more randomness.

For shading and drawing, I calculate the normal of each vertex by finding the normal of each triangle face attached to the vertex. I add these face normals together and then divide them by the number of face normals. After that, I draw each vertex using triangles starting at the starting position given by the file.