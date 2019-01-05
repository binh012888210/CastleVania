#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "DefineNumber.h"
#include "Sprites.h"
#include "Textures.h"
#include "GameObject.h"

#define NUMWIDTH 14

using namespace std;

class CTileMaps
{
public:

	// ---- update K_1.6

	// truyền vào lúc khởi tạo
	int IDTopLeftTile;


	// ------
	CTextures * textures;
	CSprites * sprites;
	
	RECT *rect;
	CSprite *currentSprite;
	vector<vector<int>> map1Matrix;
	vector<vector<int>> map1GridView;
	CTileMaps();
	~CTileMaps();
	void ReadMatrixFromFile(char* filename, const string& splitchar);
	
	void LoadGridView(vector<LPGAMEOBJECT> *objects);
	void AddObjectToGridView(LPGAMEOBJECT objects);
	void GetCoObjectsFromGridView(int cameraX,vector<int> *coObjectID);
	void LoadTiledMap(LPCWSTR filePath);
	void GetIDTopLeftTile(float CameraX);
	void RenderTiledMap(int stage, bool UnderGround);
};