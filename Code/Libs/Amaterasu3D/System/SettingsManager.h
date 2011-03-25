//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================
#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include <Singleton.h>
#include <Math/Vector2.h>

/////////////////////////////////////////////
// SettingsManager : Class which handle usefull
// information for all the renderer
/////////////////////////////////////////////
class CSettingsManager : public CSingleton<CSettingsManager>
{
	MAKE_SINGLETON(CSettingsManager)
private:
	// Attributs
	Math::TVector2I m_SizeRenderingWindow;
public:
	CSettingsManager();
	virtual ~CSettingsManager();

	// Load the config file
	// caution: Need the relative path
	void LoadFile(const std::string& path);

	// To manage the Size of the rendering window
	const Math::TVector2I& GetSizeRenderingWindow() const;
	void SetSizeRenderingWindow(const Math::TVector2I& newSize);

	/*
	 * Public attributs
	 */
	bool VerticalSync;
};

#endif /* SETTINGSMANAGER_H_ */
