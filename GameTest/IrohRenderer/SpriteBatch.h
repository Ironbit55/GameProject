/*
* Class: SpriteBatch
* Author: Ed Curran (b5021040)
* Description: The idea of this is if we are bad at ordering our daw calls. This will hold multiple batches
* and put the submited sprite in the correct batch by texture
* 
* Batches are drawn/empties when full, we need a new batch or end of render frame
* 
* this is unnecesarry if our draw calls are perfectly ordererd
* performance gain probably not worth the effort
* so I'm actually not gunno implement this I've just decided
* 
*
*/

#pragma once
class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();
};

