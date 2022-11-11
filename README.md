# Avi(ral's) Graphics Library
Development of my own 3d renderer in C

## What?

A C program which renders (or draws) 3D models on screen.

## Pipeline

### SETUP()
1. Read Obj Files
2. Fill in vertex buffer (of each model) and index buffer (of each face of the triangle in a model)
### UPDATE()
3. Iterate over each face
4. Translate/Scale/Rotate the each vertex of the face
5. Do back-face culling over each face
6. if(face not culled)
7. then project face using perspective divide
### RENDER()
8. Draw every non culled triangle/face
## Result

![backface_culling](https://user-images.githubusercontent.com/5007364/200764634-16a2def7-3125-4f5c-bc75-3b6272c58c83.gif)



## Why?
Journey into becoming a graphics programmer :)

