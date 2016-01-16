# opengl-hdr
A simple framework for running shaders & their supporting classes. 
Also, there's a sample implementation of HDR tone-mapping with Bloom.

Some screenshots of the HDR pipeline:
![Screen Shot 2015-10-18 at 11.58.36 AM.png](/Screen Shot 2015-10-18 at 11.58.36 AM.png)

The same framework has also been used to implement tone-mapping in the HDR color space. Tone mapping uses an exposure variable to map 64-bit color to 32-bit color. Slowly changing exposure over several frames gives the effect of the eye adjusting to bright light.( The resolution is recursively halved using an under sampling shader to get the average illumination value ).

The famous Perlin noise generator has also been implemented using shaders. It's like the model is dipped into a 'soup'. The noise is recursively added to itself to form 'fractal noise' which when used to modulate common functons like 'sin' or 'cos' gives realistic textures.

Example of the polyhedron dipped in fractal noise modulated by a sine function.
![Screen Shot 2015-10-18 at 11.51.55 AM.png](/Screen Shot 2015-10-18 at 11.51.55 AM.png)

All images are HDR and tone-mapped ( I've tried out several HDR -> LDR functions with interesting results but finally used the one in the original paper. )

In addition to that the image sports the Bloom effect: 
This involves passing the image through a light pass filter and applying a Gaussian blur to the resultant.
Here's a scene without Bloom ( Notice the light patches are sharp and don't bleed )
![Screen Shot 2016-01-16 at 2.46.24 PM.png](/Screen Shot 2016-01-16 at 2.46.24 PM.png)

Here's one with Bloom( Note the color bleeding that gives a cool glare effect )
![Screen Shot 2016-01-16 at 2.43.54 PM.png](/Screen Shot 2016-01-16 at 2.43.54 PM.png)

The same shaders can be extended to make simple horizontal/vertical glare by changing the blur vector.

An algorithm for smooth shadowing has also been implemented. 
This copies lit areas into a new binary buffer( shadows in black, lit areas in white )

![Screen Shot 2016-01-16 at 2.31.27 PM.png](/Screen Shot 2016-01-16 at 2.31.27 PM.png)

Now this image is blurred and projected onto the object:
![Screen Shot 2016-01-16 at 2.34.23 PM.png](/Screen Shot 2016-01-16 at 2.34.23 PM.png)

The blurring of shadows near object boundary is prevented by performing a per-object shadowing and removing the erroneous shadow pixels with a stencil buffer( containing the rendered object boundary ).

Final image with diffuse, specular and soft shadowing:
![Screen Shot 2016-01-16 at 2.41.17 PM.png](/Screen Shot 2016-01-16 at 2.41.17 PM.png)


Weighting the blur intensity with the Z-buffer value gives an effect similar to PCSS( Percentage Closer Smooth Shadowing ) used by Dunia Engine 2( FarCry Series ).
