/*
 * helpers.h
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

void get_bounding_box_for_node (const struct aiScene* scene,
		const struct aiNode* nd,
		struct aiVector3D* min,
		struct aiVector3D* max,
		struct aiMatrix4x4* trafo
);

void get_bounding_box (const struct aiScene* scene, struct aiVector3D* min, struct aiVector3D* max);
void Color4f(const struct aiColor4D *color);
void recursive_render (const struct aiScene *sc, const struct aiNode* nd);

#endif /* HELPERS_H_ */
