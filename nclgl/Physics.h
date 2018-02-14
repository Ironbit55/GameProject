#pragma once

#include <vector>
#include "../../nclgl/Vector3.h"

using std::vector;

/* An elementary Physics Data structure, containing the bare minimum of information necessary to
say where an object should appear, and which way it should point. As we increase the complexity
of our physics system, we will add elements to this structure, but the elements which can affect
the graphical representation of entities are present below (and integrated with the graphics
framework already, to make your life easier). Ultimately, you may decide to replace this data
structure entirely (for example with a class to allow inclusion of embedded functions) but be
aware that doing so might require some slight doctoring of Renderer::Renderer() and
SceneNode::Update() */

struct PhysicsData {
	Vector3 position;
	float rotation;
	Vector3 scale;
};