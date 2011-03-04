#include "Loaders.h"
#include <System/MediaManager.h>
#include <System/Loaders/ShadersLoader.h>
#include <System/Loaders/ShadersProgramLoader.h>

void Loaders::RegisterAllLoaders()
{
	//FIXME: Ajouter le chargeur d image
	//MediaManager.RegisterLoader(new CImagesLoader, "bmp, dds, jpg, pcx, png, pnm, raw, sgi, tga, tif");
	CMediaManager::Instance()->RegisterLoader(new CShaderProgramLoader, "shader");
	CMediaManager::Instance()->RegisterLoader(new ShadersLoader(FRAGMENT_SHADER), "frag");
	CMediaManager::Instance()->RegisterLoader(new ShadersLoader(VERTEX_SHADER), "vertex");
}
