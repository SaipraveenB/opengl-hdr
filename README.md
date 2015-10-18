# opengl-hdr
A simple framework for running shaders & their supporting classes. 
Also, there's a sample implementation of HDR tone-mapping with Bloom.

Some screenshots of the HDR pipeline:
![Screen Shot 2015-10-18 at 11.58.36 AM.png]({{site.baseurl}}/Screen Shot 2015-10-18 at 11.58.36 AM.png)

The same framework has also been used to implement tone-mapping in the HDR color space. Tone mapping uses an exposure variable to map 64-bit color to 32-bit color. Slowly changing exposure over several frames gives the effect of the eye adjusting to bright light.( The resolution is recursively halved using an under sampling shader to get the average illumination value ).

The famous Perlin noise generator has also been implemented using shaders. It's like the model is dipped into a 'soup'. The noise is recursively added to itself to form 'fractal noise' which when used to modulate common functons like 'sin' or 'cos' gives realistic textures.

Example of the polyhedron dipped in fractal noise modulated by a sine function.
![Screen Shot 2015-10-18 at 11.51.55 AM.png]({{site.baseurl}}/Screen Shot 2015-10-18 at 11.51.55 AM.png)

An algorithm for smooth shadowing has also been implemented. which involves drawing the object along with it's shadows, blurring this image and then weighting final image with the shadow values to get a Smooth Shadow effect.

The blurring of shadows near object boundary is prevented by performing a per-object shadowing and removing the erroneous shadow pixels with a stencil buffer( containing the rendered object boundary ).

Weighting the blur intensity with the Z-buffer value gives an effect similar to PCSS( Percentage Closer Smooth Shadowing ) used by Dunia Engine 2( FarCry Series ).

