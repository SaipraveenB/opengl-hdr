
from PIL import Image
import math
import sys
import random

def dot( vec1, vec2 ):
    return vec1[0]*vec2[0] + vec1[1]*vec2[1];

def ftoi( vec ):
    return ( int(x*255) for x in vec )


# returns an interpolation value based on the spatial localized locations of the sample point wrt the data point.
def interpolant( vec ):
    return (1 - dot(vec,vec))*(1 - dot(vec,vec));

def ceil( val ):
    if( (val % 1.0) == 0 ):
        return val+1
    else:
        return math.ceil(val)

# function to cast the parameter as an 'S' curve; a degree 3 polynomial acheives this condition.
def delinearize( t ):
    return -2 * t * t * t + 3 * t * t;

def makeRandomNoise(img, mat, srcx, srcy):
    data = img.getdata();
    width = data.size[0];
    height = data.size[1];

    for i in range(0, width):
        for j in range(0,height):
            #val = int(random.random()*255);
            #data.putpixel((i,j),(val,)*3 + (255,));

            srcw = (float((i*srcx))/width)
            srch = (float((j*srcy))/height)
            #print( ""+ format(srcw) +" " + format(srch))
            vec0 = (srcw-math.floor(srcw), srch-math.floor(srch));
            vec1 = (srcw-ceil(srcw), srch-math.floor(srch));
            vec2 = (srcw-math.floor(srcw), srch-ceil(srch));
            vec3 = (srcw-ceil(srcw), srch-ceil(srch));

            val0 = dot(mat[ int(math.floor(srcw)) ][ int(math.floor(srch)) ], vec0 );#*interpolant((srcw-math.floor(srcw), srch-math.floor(srch)));
            val1 = dot(mat[ int(math.ceil(srcw)) ][ int(math.floor(srch)) ], vec1 );#*interpolant((srcw-math.ceil(srcw), srch-math.floor(srch)));
            val2 = dot(mat[ int(math.floor(srcw)) ][ int(math.ceil(srch)) ], vec2 );#*interpolant((srcw-math.floor(srcw), srch-math.ceil(srch)));
            val3 = dot(mat[ int(math.ceil(srcw)) ][ int(math.ceil(srch)) ], vec3 );#*interpolant((srcw-math.ceil(srcw), srch-math.ceil(srch)));


            # bi-not-so-linear interpolation
            valtemp0 = val0*delinearize(1 - vec0[0]) + val1*delinearize(1 + vec1[0]);
            valtemp1 = val2*delinearize(1 - vec2[0]) + val3*delinearize(1 + vec3[0]);

            val = valtemp0*delinearize(1 - vec0[1]) + valtemp1*delinearize(1 + vec2[1]);

            val = int((val*0.5 + 0.5)*255);
            data.putpixel((i,j), (val,)*3 + (255,) )

            #iif (vec0[0] == 0) and (vec0[1] == 0 ):
            #    data.putpixel( (i,j), (255,0,0,255) )

    img.putdata( data )

def randvec():
    x = random.random()*2 -1;
    y = random.random()*2 -1;
    mag = (math.sqrt(x*x + y*y))
    return (x/mag,y/mag);

def makeVectorTable( width, height ):
    return [[ randvec() for x in range(width+1) ] for y in range(height+1)]

def makeRandomGraph( data ):
    for i in range(0, data.size[0]):
        for j in range(0, data.size[1]):
            val = int(random.random()*255);
            data.putpixel((i,j),(val,)*3 + (255,));


vect = makeVectorTable( 10, 10 );

i2 = Image.new('RGBA',(400,400));

makeRandomNoise( i2, vect, 10, 10 );

i2.save('test'+sys.argv[1]+'.png');
