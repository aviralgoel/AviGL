# Avi(ral's) Graphics Library

Development of my own 3D rendering library to learn the most fundamental algorithms (along with their implementations) of Computer Graphics. 
It is written in purely in C.

Only two external libaries have been used <math.h> and <SDL.H>

## Pipeline

### SETUP()

1. Initialize frambuffer and display window,
2. Read Obj Files and load data into vertex buffer along with index buffer.
3. Initialize Perspective Projection Matrix with FOV, aspect ratio, znear and zfar.

### UPDATE()

5. Maintain fixed time step

6. Iterate over each face
   
   1. Translate/Scale/Rotate the each vertex of the face (using Matrices)
   
   2. Do backface culling for that face
   
   3. if(!culled)
      
      1. project the face using projection matrice multiplication
      
      2. Calculate and store the face's average z value
   
   4. for all not culled faces, sort from from to back (quicksort for painter algorithm)

### RENDER()

9. Draw every non culled triangle/face

10. free the array of faces and clear buffer
    
    ## Result

![backface_culling](https://github.com/aviralgoel/AviGL/blob/master/MyProject/progress/demo.gif?raw=true)

## Motivation

Journey into becoming a graphics programmer / rendering engineer / game engine developer.


