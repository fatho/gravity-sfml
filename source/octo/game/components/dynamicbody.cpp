#include "dynamicbody.hpp"


#include <fmtlog/fmtlog.hpp>
#include <more-math/vector.hpp>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>

#include <cmath>
#include <iostream>

namespace octo {
namespace game {
namespace components {
namespace internal {

void b2BodyDeleter::operator()(b2Body* body) {
  body->GetWorld()->DestroyBody(body);
}

}



}
}
}

