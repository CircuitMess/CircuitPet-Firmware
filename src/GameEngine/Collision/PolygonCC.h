#ifndef CIRCUITPET_FIRMWARE_POLYGONCC_H
#define CIRCUITPET_FIRMWARE_POLYGONCC_H


#include <glm/vec2.hpp>
#include <vector>
#include "CollisionComponent.h"

class PolygonCC : public CollisionComponent {
public:
	PolygonCC(std::initializer_list<glm::vec2> points);
	~PolygonCC() override = default;

	const std::vector<glm::vec2>& getPoints() const;
	bool isConvex() const;

private:
	const std::vector<glm::vec2> points;
	static bool checkConvexity(const std::vector<glm::vec2>& polygon);

	const bool convex;
};


#endif //CIRCUITPET_FIRMWARE_POLYGONCC_H