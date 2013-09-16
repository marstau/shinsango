#ifndef _paintown_asteroids_arguments_h
#define _paintown_asteroids_arguments_h

#include "util/argument.h"
#include "util/pointer.h"

namespace Asteroids{
    std::vector< ::Util::ReferenceCount<Argument> > arguments();
}

#endif
