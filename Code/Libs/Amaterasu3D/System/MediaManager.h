#ifndef MEDIAMANAGER_H_
#define MEDIAMANAGER_H_

#include <Singleton.h>
#include <Debug/Exceptions.h>
#include <string>
#include <vector>

class IResource
{
public :
    // Constructeur par d�faut
    IResource();
    // Destructeur
    virtual ~IResource() = 0;
    // Renvoie le nom associ� � la ressource
    const std::string& GetName() const;
    // Ajoute une r�f�rence sur la ressource
    void AddRef();
    // Retire une r�f�rence sur la ressource
    int Release();

private :
    // Donn�es membres
    std::string m_Name;     // Nom associ� � la ressource
    int         m_RefCount; // Compteur de r�f�rences
};

// Exception .......
struct MediaManagerException: public CException
{
	MediaManagerException(const std::string& File, bool directory);
};

//Todo: think about the complexity. Can store filename only and don't research at each request.
class MediaManager : public CSingleton<MediaManager>
{
private:
	// Attributs
	typedef std::vector<std::string> PathList;
	PathList m_path;

	// For Singleton design pattern
	friend class CSingleton<MediaManager>;
	MediaManager();
public:
	virtual ~MediaManager();

	void AddPath(const std::string& path);
	void AddPathAndChilds(const std::string& path);
	const std::string GetPath(const std::string& filename);
	void CheckPathSystem();
	PathList GetPaths();
};

#endif /* MEDIAMANAGER_H_ */
