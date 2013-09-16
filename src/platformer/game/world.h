#ifndef platformer_world_h
#define platformer_world_h

#include <string>
#include <vector>
#include <map>

#include "util/pointer.h"

namespace Graphics{
class Bitmap;
}
namespace Path{
class AbsolutePath;
}
class Token;

/*! Platformers world class */

namespace Platformer{
    
class Animation;
class Background;
class Camera;
class Scriptable;
    
class World{
    public:
	World(const Token *);
	virtual ~World();
	virtual void act();
	virtual void draw(const Graphics::Bitmap &);
	
	virtual void moveCamera(int id, int x, int y);
	
	//! FIXME HANDLE Cameras correctly later
	virtual Util::ReferenceCount<Camera> getCamera(int id);
    
    inline virtual int getResolutionX() const {
        return this->resolutionX;
    }
    
    inline virtual int getResolutionY() const {
        return this->resolutionY;
    }
    
    protected:
	virtual void load(const Path::AbsolutePath &);
	virtual void load(const Token *);
	//! Name
	std::string name;
	//! Resolution of map (upscaled/downscaled according to paintowns screensize)
	int resolutionX;
	int resolutionY;
	//! Dimensions of the entire map
	int dimensionsX;
	int dimensionsY;
	// Player info
	
	//! Cameras
	std::map< int, Util::ReferenceCount<Camera> > cameras;
	
	//! Animation map
	std::map< std::string, Util::ReferenceCount<Animation> > animations;
	
	//! Backgrounds
	std::vector< Util::ReferenceCount<Background> > backgrounds;
    
    //! Script engine
    Util::ReferenceCount<Scriptable> scriptEngine;
};
}
#endif
