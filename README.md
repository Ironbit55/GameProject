# GameProjectTest
Coursework for Games Engineering module. This is 90% a game engine, with a incomplete game built on top. The idea is the engine can take a game implemented as a "world" object and run it. However you'd need to define some game entities (has to be in code, ideally would be done as data) that you can use when defining levels (can be defined with toml files). The [world](https://github.com/Ironbit55/GameProject/blob/master/GameTest/GameTest/World.cpp) file is container for initialsing and loading levels essentially.

The [engine](https://github.com/Ironbit55/GameProject/blob/master/GameTest/GameTest/FeedbackEngine.h) is made up of a few systems (inside GameTest):

- Input System - input system on top of sdlgl input, allows for keys to be rebound with config (this one is my favourite)
- Render System (Iroh Renderer) - renderer on top of sdlgl
- Entity System (Entity Attempt) - this is a kinda crappy, entity / game object thing. Ideally this would glue the systems together via entities.
- Physics System - basically interface between box2d physics and the entity system
- Messaging System - allows other systems to talk to each other and what not.
- Audio Manager - loads, caches and triggers audio effects
- Content Manager - loads and caches textures
- sdlgl - not really a module, a wrapper around sdl with opengl and sdl input
