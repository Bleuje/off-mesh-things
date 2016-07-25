# off-mesh-conversion
### tri-vert-to-off
Convert a list of triangles and a list of vertices (.tri and .vert files) to OFF file format. It should work on TOSCA datasets given there : http://tosca.cs.technion.ac.il/book/resources_data.html

I used this to try on TOSCA shapes the shape matching descriptor learning method described here :
[learning.pdf](http://www.lix.polytechnique.fr/~corman/Papers/learning.pdf)

The code is downloadable here : [learning.zip](http://www.lix.polytechnique.fr/~corman/Code/learning.zip)

This code uses C++11 features
### meshlab-scriptmaker
This is a little script maker in C++ to convert FAUST registrations from .ply to .off using MeshLab.

This is also in order to have new shapes to train the previous algorithm. [FAUST dataset](http://faust.is.tue.mpg.de/)

The created batch file created once the C++ code is run should then be run next to 'tr_reg_xxx.ply' files. Make sure you the MeshLab path is correct.
