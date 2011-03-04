#include "Loaders.h"
#include <System/MediaManager.h>
#include <System/Loaders/ShadersLoader.h>

void Loaders::RegisterAllLoaders()
{
	//FIXME: Ajouter le chargeur d image
	//MediaManager.RegisterLoader(new CImagesLoader, "bmp, dds, jpg, pcx, png, pnm, raw, sgi, tga, tif");
	CMediaManager::Instance()->RegisterLoader(new CShadersLoader, "shader");
}
