# SimpleGameEngine
Simple Game Engine written in C++ using legacy OpenGL.
This was made for a class practice in which the main goal was to create a basic 3D scene using legacy OpenGL but I took a bit further and I decided to create a game engine to create the scene with it.
The end goal then of this simple game engine is to be able to create a scene with objects and their components. But also, to learn how to implement different kind of things from scratch such as text rendering, raycasting... 
Features:
- Loading and rendering 3D models (.obj) with textures (any format file) using stb library to load textures.
- Objects with components that define its behaviour.
- Abstracted Input System.
- Collisions: box colliders
- Raycasting
- Simple Audio System using Winmm library.
- Simple UI using the Billboard approach.
- Text Rendering using a font map.

Other features specific to the scene:
- Player Controller to move horizontally (WASD) and vertically (jump and gravity)
- First person camera movement with mouse.
- Interaction system with raycasting to interact with some objects if you look at them such as the candles and notes.
- Nested transformations: solar system, robot arm and Frame, table & note.

# Demo Video:
<br />
![image](https://imgur.com/wSW2n4W)

<br />
[![Demo Video](https://github.com/iago-cpalmer/SimpleGameEngine/assets/68129919/5e642f86-0ba9-4cb7-97c0-b189f842df33)](https://www.youtube.com/watch?v=VHlE1FgZDWs&ab_channel=IagoCaldenteyPalmer "Demo Simple Game Engine for Computer Graphics Class Practice")
