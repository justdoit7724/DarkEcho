#pragma once
#include <vector>
#include "Network.h"
#include "Game_info.h"

class MapSpace {
protected:

	IVector2 maskStartPt;
	IVector2 maskSize;
	UINT kind;

public:

	const IVector2 MaskStartPt() {return maskStartPt;}
	const IVector2 MaskSize() {return maskSize;}
	const UINT Kind(){return kind;}

	virtual bool IsInPartition(Vector2 pPos)=0;
	virtual bool IsWall(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2& reflect, Vector2& closestPt) = 0;
	virtual bool IsInside(Vector2 pt, Vector2& closestPt) = 0;
};
class SpaceRect : public MapSpace {
public:
	SpaceRect(float posX, float posY, float widthX, float widthY, float slope = 0.0f, UINT kind = SPACE_NORMAL);

	bool IsInPartition(Vector2 pPos);
	bool IsWall(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2& reflect, Vector2& closestPt) override;
	bool IsInside(Vector2 pt, Vector2& closestPt) override;

	const Vector2& Center() {return center;}
	const Vector2& Rad() {return rad;}

private:

	bool Point_OBB(Vector2 pt, Vector2 oPos, Vector2 oRad, _Out_ Vector2& dist);
	bool AABB_OBB(Vector2 aPos, Vector2 aRad, Vector2 oPos, Vector2 oRad);
	Vector2 center;
	Vector2 rad;
	Vector2 rightDir;
	Vector2 upDir;
	Vector2 e1, e2, e3, e4;//tl-tr-bl-br
};
class SpaceCircle : public MapSpace {
public:
	SpaceCircle(float, float, float, UINT kind = SPACE_NORMAL);

	bool IsInPartition(Vector2 pPos);
	bool IsWall(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2& reflect, Vector2& closestPt) override;
	bool IsInside(Vector2 pt, Vector2& closestPt) override;

	const Vector2& Center() { return center;}
	const float& Rad() {return rad;}

private:
	bool AABB_Circle(Vector2 aPos, Vector2 aRad, Vector2 cCenter, float cRad);
	bool Point_Circle(Vector2 pt, Vector2 cCenter, float rad, Vector2& closestPt);

	Vector2 center;
	float rad;
};

class Map : public IMapSpace
{
public:
	Map(ID3D11Device*);
	~Map();

	ID3D11ShaderResourceView*const* SpaceColorSRV() override;
	const Vector2 Move(Vector2 fromPt, Vector2 toPt, UINT& toSpace) override;
	IVector4 Dest() override; 
	bool HitReflect(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2& reflection) override;

private:

	std::vector<MapSpace*> spaces;
	std::vector<MapSpace*> walkables;
	std::vector<MapSpace*> spacePartition[MAP_DIVISION_NUM_HEIGHT][MAP_DIVISION_NUM_WIDTH];
	std::vector<MapSpace*> walkablePartition[MAP_DIVISION_NUM_HEIGHT][MAP_DIVISION_NUM_WIDTH];
	SpaceRect* destination;

	ComPtr<ID3D11ShaderResourceView> spaceColorSRV = nullptr;
};

