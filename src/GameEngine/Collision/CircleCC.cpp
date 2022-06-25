
#include "CircleCC.h"

CircleCC::CircleCC(float radius): radius(radius), CollisionComponent(CollisionType::Circle){

}

float CircleCC::getRadius() const{
	return radius;
}
