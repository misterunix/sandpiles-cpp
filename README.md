# sandpiles-cpp

I've been toying around with Abelian sandpiles for about a year now. 

Im still not satisfied with the speed, so I keep making versions and testing them.

Someday I intend to document my results. 

Dynamically resizing the array to allow it to expand, keeping a 10-pixel border around the sandpile.

static libraries need
- libgd 
- libjpeg




# Formatting style
class names are prefixed with `c` using camel case.  
class member variables are prefixed with `_m`.  
function parameters are prefixed with `_t`.   

common sense says to inline all functions that can be for performance reasons.  