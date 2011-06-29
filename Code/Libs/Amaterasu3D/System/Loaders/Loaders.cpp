#include "Loaders.h"
#include <System/MediaManager.h>
#include <System/Loaders/ShadersLoader.h>
#include <System/Loaders/AssimpLoader.h>
#include <System/Loaders/ImagesLoader.h>

void Loaders::RegisterAllLoaders()
{
	//FIXME: Ajouter le chargeur d image
	CMediaManager::Instance().RegisterLoader<Texture>(new ImagesLoader, "bmp, dds, exr, gif, hdr, jpg, pcx, png, pnm, raw, sgi, tga, tif");
	CMediaManager::Instance().RegisterLoader<Shader>(new ShadersLoader, "shader");
	CMediaManager::Instance().RegisterLoader<IMeshSceneNode>(new AssimpLoader, "x, obj, dae, blend, 3ds, ase, ply, dxf, lwo, lxo, stl, ac, ms3d, cob, scn, mdl, mdl2, mdl3, pk3, md5");
}
