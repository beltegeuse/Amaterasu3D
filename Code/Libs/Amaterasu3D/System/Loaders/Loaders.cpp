#include "Loaders.h"
#include <System/MediaManager.h>
#include <System/Loaders/ShadersLoader.h>
#include <System/Loaders/AssimpLoader.h>
void Loaders::RegisterAllLoaders()
{
	//FIXME: Ajouter le chargeur d image
	//MediaManager.RegisterLoader(new CImagesLoader, "bmp, dds, jpg, pcx, png, pnm, raw, sgi, tga, tif");
	CMediaManager::Instance().RegisterLoader<glShader>(new ShadersLoader, "shader");
	CMediaManager::Instance().RegisterLoader<SceneGraph::AssimpNode>(new AssimpLoader, "x, obj, dae, blend, 3ds, ase, ply, dxf, lwo, lxo, stl, ac, ms3d, cob, scn, mdl, mdl2, mdl3, pk3, md5");
}
