
#include "SquareCC.h"

SquareCC::SquareCC(PixelDim dim) :  dim(dim), CollisionComponent(Type::Square){

}

PixelDim SquareCC::getDim() const{
	return dim;
}
