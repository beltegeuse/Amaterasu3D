#ifndef LOGGERDEBUG_H_
#define LOGGERDEBUG_H_

/*
 * Heritage de Logger
 */

#include <Logger/Logger.h>
namespace ama3D
{
class LoggerDebug: public Logger
{
private:
	//! Definition de la fonction Write.
	virtual void Write(const std::string& Message);
	virtual void Flush();
public:
	LoggerDebug();
	virtual ~LoggerDebug();
};
}

#endif /* LOGGERDEBUG_H_ */
