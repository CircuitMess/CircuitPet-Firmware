
#include "CircleCC.h"

CircleCC::CircleCC(uint16_t radius): radius(radius), CollisionComponent(Type::Circle){

}

uint16_t CircleCC::getRadius() const{
	return radius;
}
