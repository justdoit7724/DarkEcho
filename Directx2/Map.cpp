#include "Map.h"
#include <unordered_set>
#include <map>

SpaceRect::SpaceRect(float posX, float posY, float widthX, float widthY, float s, UINT k)
{
	center = Vector2(posX, posY);
	rad = Vector2(widthX/2.0f, widthY/2.0f);

	kind = k;

	Matrix rot = Matrix::RotationZ(-s);
	rightDir = Vector2(1, 0)*rot;
	upDir = Vector2(0, 1)*rot;
	e1 = center - (rightDir * rad.x) + (upDir * rad.y);
	e2 = center + (rightDir * rad.x) + (upDir * rad.y);
	e3 = center - (rightDir * rad.x) - (upDir * rad.y);
	e4 = center + (rightDir * rad.x) - (upDir * rad.y);
	float minX = min(e1.x, min(e2.x, min(e3.x, e4.x)));
	float maxX = max(e1.x, max(e2.x, max(e3.x, e4.x)));
	float minY = min(e1.y, min(e2.y, min(e3.y, e4.y)));
	float maxY = max(e1.y, max(e2.y, max(e3.y, e4.y)));
	maskStartPt = IVector2(minX, minY);
	maskSize = IVector2(maxX - minX, maxY - minY);
}
bool SpaceRect::Point_OBB(Vector2 pt, Vector2 oPos, Vector2 oRad, Vector2& closestPt)
{
	if (pt.x < 125 && oRad.x == 275)
		int a = 0;

	float lx = Vector2::Dot(rightDir, pt - oPos);
	float ly = Vector2::Dot(upDir, pt - oPos);
	float absX = abs(lx);
	float absY = abs(ly);
	if (absX > oRad.x || absY > oRad.y)
	{
		float signX = absX / lx;
		float signY = absY / ly;
		float closestXDist = max((absX - oRad.x), 0)*signX;
		float closestYDist = max((absY - oRad.y), 0)*signY;

		closestPt = pt -
			rightDir * closestXDist -
			upDir * closestYDist;

		return false;
	}

	closestPt = pt;

	return true;
}
bool SpaceRect::AABB_OBB(Vector2 aPos, Vector2 aRad, Vector2 oPos, Vector2 oRad)
{
	Vector2 a1 = Vector2(-aRad.x, +aRad.y) + aPos;
	Vector2 a2 = Vector2(+aRad.x, +aRad.y) + aPos;
	Vector2 a3 = Vector2(-aRad.x, -aRad.y) + aPos;
	Vector2 a4 = Vector2(+aRad.x, -aRad.y) + aPos;

	Vector2 proj;
	float d1, d2, d3, d4;

	//aabb
	d1 = e1.x - aPos.x;
	d2 = e2.x - aPos.x;
	d3 = e3.x - aPos.x;
	d4 = e4.x - aPos.x;
	float minD = fmin(d1, fmin(d2, fmin(d3, d4)));
	float maxD = max(d1, max(d2, max(d3, d4)));
	float aMinD = fmin(abs(d1), fmin(abs(d2), fmin(abs(d3), abs(d4))));
	if (aMinD >= aRad.x && minD*maxD > 0)
		return false;

	d1 = e1.y - aPos.y;
	d2 = e2.y - aPos.y;
	d3 = e3.y - aPos.y;
	d4 = e4.y - aPos.y;
	minD = fmin(d1, fmin(d2, fmin(d3, d4)));
	maxD = max(d1, max(d2, max(d3, d4)));
	aMinD = fmin(abs(d1), fmin(abs(d2), fmin(abs(d3), abs(d4))));
	if (aMinD >= aRad.x && minD*maxD > 0)
		return false;

	proj = rightDir;
	d1 = Vector2::Dot(proj, oPos - a1);
	d2 = Vector2::Dot(proj, oPos - a2);
	d3 = Vector2::Dot(proj, oPos - a3);
	d4 = Vector2::Dot(proj, oPos - a4);
	minD = fmin(d1, fmin(d2, fmin(d3, d4)));
	maxD = max(d1, max(d2, max(d3, d4)));
	aMinD = fmin(abs(d1), fmin(abs(d2), fmin(abs(d3), abs(d4))));
	if (aMinD >= oRad.x && minD*maxD > 0)
		return false;

	proj = upDir;
	d1 = Vector2::Dot(proj, oPos - a1);
	d2 = Vector2::Dot(proj, oPos - a2);
	d3 = Vector2::Dot(proj, oPos - a3);
	d4 = Vector2::Dot(proj, oPos - a4);
	minD = fmin(d1, fmin(d2, fmin(d3, d4)));
	maxD = max(d1, max(d2, max(d3, d4)));
	aMinD = fmin(abs(d1), fmin(abs(d2), fmin(abs(d3), abs(d4))));
	if (aMinD >= oRad.y && minD*maxD > 0)
		return false;

	return true;
}
Vector2 Line_Line(Vector2 p1, Vector2 p1_2, Vector2 p2, Vector2 p2_2)
{
	Vector2 size1 = p1_2 - p1;
	Vector2 size2 = p2_2 - p2;

	float denominator = -size2.x * size1.y + size1.x * size2.y;
	if (denominator == 0.0f)
		return Vector2(FLT_MAX, FLT_MAX);
	float t =
		(size2.x * (p1.y - p2.y) - size2.y * (p1.x - p2.x))
		/
		denominator;

	return p1 + (size1 * t);
}

bool SpaceRect::IsInPartition(Vector2 pPos)
{
	return AABB_OBB(pPos, Vector2(MAP_DIVISION_LENGTH_WIDTH/2, MAP_DIVISION_LENGTH_HEIGHT/2), center, rad);
}
bool SpaceRect::IsWall(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2 & reflect, Vector2& closestPt)
{
	if (Point_OBB(toPt, center, rad, closestPt)) {

		return false;
	}

	Vector2 proj1 = (e1 - fromPt)*Matrix::RotationZ(-ANGLE90);
	Vector2 proj2 = (e2 - fromPt)*Matrix::RotationZ(-ANGLE90);
	float d1 = Vector2::Dot(proj1, dir);
	float d2 = Vector2::Dot(proj2, dir);
	if (d1 > 0 && d2 <= 0) {

		float rad = Vector2::DirToRad(dir,rightDir);
		reflect = rightDir * Matrix::RotationZ(-rad);
		return true;
	}
	Vector2 proj3 = (e3 - fromPt)*Matrix::RotationZ(-ANGLE90);
	Vector2 proj4 = (e4 - fromPt)*Matrix::RotationZ(-ANGLE90);
	float d3 = Vector2::Dot(proj3, dir);
	float d4 = Vector2::Dot(proj4, dir);
	if (d4 > 0 && d3 <= 0) {

		float rad = Vector2::DirToRad(dir, rightDir);
		reflect = rightDir * Matrix::RotationZ(-rad);
		return true;
	}
	else{

		float rad = Vector2::DirToRad(dir, upDir);
		reflect = upDir * Matrix::RotationZ(-rad);
		return true;
	}
}
bool SpaceRect::IsInside(Vector2 pt, Vector2& cPt)
{
	return Point_OBB(pt, center, rad, cPt);

}

SpaceCircle::SpaceCircle(float cX, float cY, float r, UINT k)
{
	center = Vector2(cX, cY);
	rad = r*0.5f;
	kind = k;

	maskStartPt = IVector2(cX - rad, cY - rad);
	maskSize = IVector2(r, r);
}
bool SpaceCircle::AABB_Circle(Vector2 aPos, Vector2 aRad, Vector2 cCenter, float cRad)
{
	//aabb
	if (abs(cCenter.x - aPos.x) >= (aRad.x + cRad))
		return false;
	if (abs(cCenter.y - aPos.y) >= (aRad.y + cRad))
		return false;

	//circle
	Vector2 a1 = Vector2(-aRad.x, +aRad.y) + aPos;
	Vector2 a2 = Vector2(+aRad.x, +aRad.y) + aPos;
	Vector2 a3 = Vector2(-aRad.x, -aRad.y) + aPos;
	Vector2 a4 = Vector2(+aRad.x, -aRad.y) + aPos;
	Vector2 proj = Vector2::Normalize(aPos - cCenter);
	float d1 = Vector2::Dot(proj, cCenter - a1);
	float d2 = Vector2::Dot(proj, cCenter - a2);
	float d3 = Vector2::Dot(proj, cCenter - a3);
	float d4 = Vector2::Dot(proj, cCenter - a4);
	float minD = fmin(d1, fmin(d2, fmin(d3, d4)));
	float maxD = max(d1, max(d2, max(d3, d4)));
	float aMinD = fmin(abs(d1), fmin(abs(d2), fmin(abs(d3), abs(d4))));
	if (aMinD >= cRad && minD*maxD > 0)
		return false;

	return true;
}
bool SpaceCircle::Point_Circle(Vector2 pt, Vector2 cCenter, float rad, Vector2& closestPt)
{
	closestPt = 0;

	Vector2 distV = cCenter - pt;
	float length = Vector2::Length(distV);
	Vector2 dir = Vector2::Normalize(distV);
	if (length >= rad)
	{
		closestPt = pt + dir*(length - rad);
		return false;
	}
	closestPt = pt;
	return true;
}
Vector2 Line_Circle(Vector2 p1, Vector2 p2, Vector2 cCenter, float radius)
{
	float A, B, C, det, t;

	Vector2 lineSize = p2 - p1;
	A = Vector2::Dot(lineSize, lineSize);
	B = 2 * (lineSize.x * (p1.x - cCenter.x) + lineSize.y * (p1.y - cCenter.y));
	C = (p1.x - cCenter.x) * (p1.x - cCenter.x) +
		(p1.y - cCenter.y) * (p1.y - cCenter.y) -
		radius * radius;

	det = B * B - 4 * A * C;

	// Two solutions.
	t = (-B + sqrt(abs(det))) / (2 * A);
	return Vector2(p1.x + t * lineSize.x, p1.y + t * lineSize.y);
}
bool SpaceCircle::IsInPartition(Vector2 pPos)
{
	return AABB_Circle(pPos, Vector2(MAP_DIVISION_LENGTH_WIDTH / 2, MAP_DIVISION_LENGTH_HEIGHT / 2), center, rad);
}
bool SpaceCircle::IsWall(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2 & reflect, Vector2& closestPt)
{
	if (Point_Circle(toPt, center, rad, closestPt)) {

		return false;
	}

	Vector2 hitDir = Vector2::Normalize(toPt - center);
	float rad = Vector2::DirToRad(hitDir, dir);
	reflect = hitDir * Matrix::RotationZ(-ANGLE180 + rad);

	return true;
}
bool SpaceCircle::IsInside(Vector2 pt, Vector2& closestPt)
{
	return Point_Circle(pt, center, rad, closestPt);
}

Map::Map(ID3D11Device* device)
{
	//1-water
	spaces.push_back(new SpaceRect(192, 288, 192, 384));
	walkables.push_back(new SpaceRect(192, 288, 172, 364));
	spaces.push_back(new SpaceRect(432, 192, 672, 192));
	walkables.push_back(new SpaceRect(432, 192, 652, 172));

	spaces.push_back(new SpaceRect(864, 764, 576, 192));
	walkables.push_back(new SpaceRect(864, 768, 556, 172));
	spaces.push_back(new SpaceRect(672, 480, 192, 768,0,SPACE_WATER));
	walkables.push_back(new SpaceRect(672, 480, 172, 748,0,SPACE_WATER));

	//1-circle
	//bottom room
	spaces.push_back(new SpaceRect(1824, 384, 678, 96, (ANGLE90/2.0f)));
	walkables.push_back(new SpaceRect(1824, 384, 658, 76, (ANGLE90/2.0f)));
	spaces.push_back(new SpaceRect(2190, 160, 370, 68));
	walkables.push_back(new SpaceRect(2190, 160, 350, 48));
	//top room
	spaces.push_back(new SpaceRect(2016, 1344, 1200, 180, -(ANGLE90 / 2.0f)));
	walkables.push_back(new SpaceRect(2016, 1344, 1180, 160, -(ANGLE90 / 2.0f)));
	spaces.push_back(new SpaceRect(2520, 1732, 300, 192));
	walkables.push_back(new SpaceRect(2520, 1732, 290, 172));
	//circle
	spaces.push_back(new SpaceCircle(1440, 768, 768));
	walkables.push_back(new SpaceCircle(1440, 768, 748));
	spaces.push_back(new SpaceCircle(1440, 768, 638,SPACE_TRAP));
	walkables.push_back(new SpaceCircle(1440, 768, 638, SPACE_TRAP));


	//2-media
	spaces.push_back(new SpaceRect(1440, 1536, 1544, 60));
	walkables.push_back(new SpaceRect(1440, 1536, 1544, 40));
	spaces.push_back(new SpaceRect(1536, 1665, 576, 60, ANGLE90*0.3f));
	walkables.push_back(new SpaceRect(1536, 1665, 556, 40, ANGLE90*0.3f));
	spaces.push_back(new SpaceRect(1055, 1665, 576, 60, -ANGLE90*0.3f));
	walkables.push_back(new SpaceRect(1055, 1665, 556, 40, -ANGLE90*0.3f));
	spaces.push_back(new SpaceRect(1260, 1536, 330, 60, 0, SPACE_WATER));
	walkables.push_back(new SpaceRect(1260, 1536, 330, 40, 0, SPACE_WATER));

	//2-main
	spaces.push_back(new SpaceRect(385, 1870, 750, 1152));
	walkables.push_back(new SpaceRect(385, 1870, 730, 1132));


	spaces.push_back(new SpaceRect(86, 1870, 160, 1152,0,SPACE_WATER));
	walkables.push_back(new SpaceRect(86, 1870, 140, 1132, 0, SPACE_WATER));
	spaces.push_back(new SpaceRect(680, 2020, 190, 882, 0, SPACE_WATER));
	walkables.push_back(new SpaceRect(680, 2020, 170, 862, 0, SPACE_WATER));

	spaces.push_back(new SpaceRect(470, 2220, 300, 150, ANGLE90*0.6f, SPACE_TRAP));
	walkables.push_back(new SpaceRect(470, 2220, 300, 150, ANGLE90*0.6f, SPACE_TRAP));
	spaces.push_back(new SpaceRect(300, 1930, 300, 220, ANGLE90*0.6f, SPACE_TRAP));
	walkables.push_back(new SpaceRect(300, 1930, 300, 220, ANGLE90*0.6f, SPACE_TRAP));
	spaces.push_back(new SpaceRect(560, 1610, 290, 60, 0, SPACE_TRAP));
	walkables.push_back(new SpaceRect(560, 1610, 290, 60, 0, SPACE_TRAP));

	//3-media
	spaces.push_back(new SpaceRect(420, 2520, 192, 250));
	walkables.push_back(new SpaceRect(420, 2520, 172, 230));
	spaces.push_back(new SpaceRect(668, 2688, 1152, 160));
	walkables.push_back(new SpaceRect(668, 2688, 1132, 140));

	spaces.push_back(new SpaceRect(1480, 2600, 600, 170,ANGLE90*0.2f));
	walkables.push_back(new SpaceRect(1480, 2600, 580, 150, ANGLE90*0.2f));
	spaces.push_back(new SpaceCircle(1730, 2580, 320));
	walkables.push_back(new SpaceCircle(1730, 2580, 300));
	
	//3-main
	spaces.push_back(new SpaceRect(2030, 2580, 370, 192, 0,SPACE_WATER));
	walkables.push_back(new SpaceRect(2030, 2580, 350, 172, 0, SPACE_WATER));
	spaces.push_back(new SpaceRect(2170, 2580, 170, 540, 0,SPACE_WATER));
	walkables.push_back(new SpaceRect(2170, 2580, 150, 520,  0,SPACE_WATER));

	destination = new SpaceRect(2170, 2235, 170, 192, 0, SPACE_DEST);
	spaces.push_back(destination);
	walkables.push_back(destination);

	Vector2 center = Vector2(MAP_DIVISION_LENGTH_WIDTH, MAP_DIVISION_LENGTH_HEIGHT) / 2;
	for (int y = 0; y < MAP_DIVISION_NUM_HEIGHT; ++y) {
		for (int x = 0; x < MAP_DIVISION_NUM_WIDTH; ++x) {

			Vector2 curPos = center + Vector2(x*MAP_DIVISION_LENGTH_WIDTH, y*MAP_DIVISION_LENGTH_HEIGHT);

			for (auto s : spaces) {
				if (s->IsInPartition(curPos))
					spacePartition[y][x].push_back(s);
			}
		}
	}
	for (int y = 0; y < MAP_DIVISION_NUM_HEIGHT; ++y) {
		for (int x = 0; x < MAP_DIVISION_NUM_WIDTH; ++x) {

			Vector2 curPos = center + Vector2(x*MAP_DIVISION_LENGTH_WIDTH, y*MAP_DIVISION_LENGTH_HEIGHT);

			for (auto w : walkables) {
				if (w->IsInPartition(curPos))
					walkablePartition[y][x].push_back(w);
			}
		}
	}

	#pragma region SRV setting

	UINT* maskValues = new UINT[MAP_RESOLUTION];
	for (int i = 0; i < MAP_RESOLUTION; ++i) {
		maskValues[i] = 0x00ffffff;
	}
	for (auto s : spaces) {

		IVector2 maskSize = s->MaskSize();
		IVector2 maskStartPt = s->MaskStartPt();
		for (int mY = 0; mY < maskSize.y; mY++) {
			for (int mX = 0; mX < maskSize.x; mX++) {

				int curX = maskStartPt.x + mX;
				int curY = maskStartPt.y + mY;

				Vector2 empty;
				if (s->IsInside(Vector2(curX, curY), empty))
					maskValues[curX + curY * MAP_WIDTH] = UINT(s->Kind());
			}
		}
	}
	ComPtr<ID3D11Texture2D> mask;
	D3D11_TEXTURE2D_DESC maskDesc;
	maskDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	maskDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	maskDesc.Usage = D3D11_USAGE_IMMUTABLE;
	maskDesc.ArraySize = 1;
	maskDesc.CPUAccessFlags = 0;
	maskDesc.Width = MAP_WIDTH;
	maskDesc.Height = MAP_HEIGHT;
	maskDesc.MipLevels = 1;
	maskDesc.MiscFlags = 0;
	maskDesc.SampleDesc.Count = 1;
	maskDesc.SampleDesc.Quality = 0;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = maskValues;
	data.SysMemPitch = sizeof(UINT)*MAP_WIDTH;
	r_assert(
		device->CreateTexture2D(
			&maskDesc,
			&data,
			mask.GetAddressOf())
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC maskSRVDesc;
	maskSRVDesc.Format = maskDesc.Format;
	maskSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	maskSRVDesc.Texture2D.MipLevels = 1;
	maskSRVDesc.Texture2D.MostDetailedMip = 0;
	r_assert(
		device->CreateShaderResourceView(
			mask.Get(),
			&maskSRVDesc,
			spaceColorSRV.GetAddressOf())
	);
#pragma endregion
}

Map::~Map()
{
	for (auto s : spaces)
		delete s;
	for (auto w : walkables)
		delete w;
}


ID3D11ShaderResourceView*const* Map::SpaceColorSRV()
{
	return spaceColorSRV.GetAddressOf();
}

const Vector2 Map::Move(Vector2 fromPt, Vector2 toPt, UINT& toSpace)
{

	int pIdxX = toPt.x / MAP_DIVISION_LENGTH_WIDTH;
	int pIdxY = toPt.y / MAP_DIVISION_LENGTH_HEIGHT;

	std::unordered_set<MapSpace*> curSpaces;
	
	//3x3
	int prevX = max(0, pIdxX-1);
	int nextX = min(MAP_DIVISION_NUM_WIDTH - 1, pIdxX+1);
	int prevY = max(0, pIdxY-1);
	int nextY = min(MAP_DIVISION_NUM_HEIGHT - 1, pIdxY+1);
	for (int y = prevY; y <= nextY; ++y) {
		for (int x = prevX; x <= nextX; ++x) {
			for (auto w : walkablePartition[y][x]) {

				curSpaces.insert(w);
			}
		}
	}

	std::map<float, Vector2> closestPts;
	closestPts.insert(std::pair<float,Vector2>(FLT_MAX, fromPt));
	
	for (auto s : curSpaces)
	{
		Vector2 cPt;
		if (s->IsInside(toPt, cPt) && s->Kind() != SPACE_NORMAL) {
			toSpace = s->Kind();
			return toPt;
		}

		closestPts.insert(std::pair<float, Vector2>(Vector2::SLength(toPt - cPt), cPt));
	}

	toSpace = SPACE_NORMAL;
	return closestPts.begin()->second;
}

IVector4 Map::Dest()
{
	IVector2 startPos = destination->MaskStartPt();
	IVector2 size = destination->MaskSize();
	return IVector4(startPos.x, startPos.y, size.x, size.y);
}

bool Map::HitReflect(Vector2 fromPt, Vector2 toPt, Vector2 dir, Vector2 & reflection)
{
	int pIdxX = toPt.x / MAP_DIVISION_LENGTH_WIDTH;
	int pIdxY = toPt.y / MAP_DIVISION_LENGTH_HEIGHT;

	std::unordered_set<MapSpace*> curSpaces;

	//3x3
	int prevX = max(0, pIdxX - 1);
	int nextX = min(MAP_DIVISION_NUM_WIDTH - 1, pIdxX + 1);
	int prevY = max(0, pIdxY - 1);
	int nextY = min(MAP_DIVISION_NUM_HEIGHT - 1, pIdxY + 1);
	for (int y = prevY; y <= nextY; ++y) {
		for (int x = prevX; x <= nextX; ++x) {
			for (auto s : spacePartition[y][x]) {

				curSpaces.insert(s);
			}
		}
	}

	std::map<float, Vector2> closestPts;
	closestPts.insert(std::pair<float, Vector2>(FLT_MAX, fromPt));
	for (auto s : curSpaces)
	{
		Vector2 curReflect=0;
		Vector2 closestPt = 0;
		if (s->IsWall(fromPt, toPt, dir, curReflect, closestPt)) {

			closestPts.insert(std::pair<float, Vector2>(Vector2::SLength(toPt - closestPt), curReflect));
		}
		else {
			return false;
		}
	}

	reflection = closestPts.begin()->second;

	return true;
	/*
	*/
}
