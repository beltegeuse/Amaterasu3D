#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include <Singleton.h>
#include <Math/Vector2.h>

/////////////////////////////////////////////
// SettingsManager : Class which handle usefull
// information for all the renderer
/////////////////////////////////////////////
class SettingsManager : CSingleton<SettingsManager>
{
	MAKE_SINGLETON(SettingsManager)
private:
	// Attributs
	Math::TVector2I m_SizeRenderingWindow;
public:
	SettingsManager();
	virtual ~SettingsManager();

	// Load the config file
	// caution: Need the relative path
	void LoadFile(const std::string& path);

	// To manage the Size of the rendering window
	const Math::TVector2I& GetSizeRenderingWindow() const;
	void SetSizeRenderingWindow(const Math::TVector2I& newSize);

};

#endif /* SETTINGSMANAGER_H_ */
