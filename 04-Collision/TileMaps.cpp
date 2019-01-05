#include "TileMaps.h"

CTileMaps::CTileMaps()
{
	map1Matrix.clear();
	map1GridView.clear();
}

CTileMaps::~CTileMaps()
{
	map1Matrix.clear();
	map1GridView.clear();
}

void CTileMaps::LoadGridView(vector<LPGAMEOBJECT> *objects)
{
	map1GridView.clear();
	int left2;
	int i, k;
	for (i = 0; i < 40; i++) 
	{	
		map1GridView.push_back(vector<int>());
		for (k = 1; k < objects->size(); k++)
		{
			left2 = objects->at(k)->x / 144;
			if (left2 == i || left2 < 0)
			{
				map1GridView.back().push_back(objects->at(k)->ObjectID);
			}
		}
	}
}

void CTileMaps::AddObjectToGridView(LPGAMEOBJECT object)
{
	int left2 = object->x / 144;
	map1GridView[left2].push_back(object->ObjectID);
}

void CTileMaps::GetCoObjectsFromGridView(int cameraX, vector<int> *coObjectID)
{
	coObjectID->clear();
	int IDGridView = cameraX / 144; 
	for (int w = IDGridView; w < IDGridView + 5 ; w++)
	{
		if (w > 39)
			break;
		for (int h = 0; h < map1GridView[w].size(); h++)
		{
			coObjectID->push_back(map1GridView[w][h]);
		}
	}
}

void CTileMaps::LoadTiledMap(LPCWSTR filePath)
{
	textures = CTextures::GetInstance();
	textures->Add(ID_TEX_BACKGROUND_TILE_MAP, filePath, D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texBackground = textures->Get(ID_TEX_BACKGROUND_TILE_MAP);
	sprites = CSprites::GetInstance();
	for (int i = 0; i < 106; i++)
	{
		int x = i % NUMWIDTH;
		int y = i / NUMWIDTH;
		sprites->Add(i, x*TILE_SIZE, y*TILE_SIZE, x*TILE_SIZE + TILE_SIZE, y*TILE_SIZE + TILE_SIZE, texBackground);
	}
}
void CTileMaps::GetIDTopLeftTile(float CameraX)
{
	IDTopLeftTile = (CameraX / TILE_SIZE) ;
}
void CTileMaps::ReadMatrixFromFile(char* filename,const string& splitchar)
{
	string      line;
	string      strnum;
	ifstream	in(filename);



	while (getline(in, line)) //tách từng dòng ra từ file in
	{
		map1Matrix.push_back(vector<int>()); // thêm 1 vector int vào cuối

		for (string::const_iterator i = line.begin(); i != line.end(); ++i) //iterator dùng để so sánh
		{
			// nếu i không phải kí tự cách, thì bỏ nó vào strnum
			if (splitchar.find(*i) == -1)
			{
				strnum += *i;
				if (i + 1 != line.end()) //Khác end thì vẫn tiếp tục(1-0-3-tab-1-0-4)
					continue;
			}

			// nếu strnum vẫn rỗng thì tiếp tục thêm vào (trường hợp 2 dấu cách liên tiếp)
			//if (strnum.empty())
			//	continue;

			// nếu strnum không rỗng thì chuyển kí tự thành số
			int       number;
			istringstream(strnum) >> number;
			map1Matrix.back().push_back(number); //thêm số này vào cuối vector

			strnum.clear();
		}
	}
	
}
void CTileMaps::RenderTiledMap(int stage, bool UnderGround)
{
	int h, w; //hang va cot trong ma tran map1Matrix

	//duyet tu dau den cuoi danh sach
	for (int w = IDTopLeftTile; w < IDTopLeftTile+9; w++)
	{
		for (int h = 0; h < 7; h++)
		{
			int tempH;
			if (stage == 0 || UnderGround == true)
			{
				tempH = h + 7;
			}
			else 
			{
				tempH = h;
			}
			int id = map1Matrix[tempH][w];
			currentSprite = sprites->Get(id);
			if (UnderGround == true)
				currentSprite->Draw(w * TILE_SIZE, tempH * TILE_SIZE);
			else
				currentSprite->Draw(w * TILE_SIZE, h * TILE_SIZE);
		}//---------------
	}

}
