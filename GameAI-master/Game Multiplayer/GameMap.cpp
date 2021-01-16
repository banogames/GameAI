#include "GameMap.h"
#include "GameLog.h"

GameMap::GameMap(char* filePath)
{
	LoadMap(filePath);
	//debugDraw = new GameDebugDraw();
}

void GameMap::LoadMap(char* filePath)
{
	_map = new Tmx::Map();
	_map->ParseFile(filePath);

	// đưa danh sách hình ảnh tileset vào tilesetList
	for (int i = 0; i < _map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = _map->GetTileset(i);
		//char* link = "Resource files/%s", tileset->GetImage()->GetSource().c_str();
		Sprite *sprite = new Sprite("Resource files/tileset.png", D3DXCOLOR(255, 255, 255, 255));
		_tilesetList.insert(std::pair<int, Sprite*>(i, sprite));
	}

	// duyệt các layer của map
	for (int i = 0; i < _map->GetNumTileLayers(); i++)
	{	
		const Tmx::TileLayer *layer = _map->GetTileLayer(i);
		if (layer->GetName() == "Brick" || 
			layer->GetName() == "Metal Brick" || 
			layer->GetName() == "Water" || 
			layer->GetName() == "Grass" || 
			layer->GetName() == "Boundary" || 
			layer->GetName() == "Eagle NPC" || 
			layer->GetName() == "Eagle Player")
		{
			int tileWidth = _map->GetTileWidth();
			int tileHeight = _map->GetTileHeight();

			for (int m = 0; m < layer->GetHeight(); m++)
			{
				for (int n = 0; n < layer->GetWidth(); n++)
				{
					if (layer->GetTileTilesetIndex(n, m) != -1)
					{
						// tọa độ object
						D3DXVECTOR2 pos((float)(n * tileWidth + tileWidth / 2),
							(float)(m * tileHeight + tileHeight / 2));

						if (layer->GetName() == "Brick")
						{
							Brick* brick = new BrickNormal(pos);
							_brickNorList.push_back((BrickNormal*)brick);
							_brickList.push_back(brick);
							_bricks[pos.x / X_STEP][pos.y / Y_STEP] = brick;
						}
						else if (layer->GetName() == "Metal Brick")
						{
							Brick* brick = new MetalBrick(pos);
							_brickList.push_back(brick);
							_bricks[pos.x / X_STEP][pos.y / Y_STEP] = brick;
						}
						else if (layer->GetName() == "Water")
						{
							Brick* brick = new Water(pos);
							_brickList.push_back(brick);
							_bricks[pos.x / X_STEP][pos.y / Y_STEP] = brick;
						}
						else if (layer->GetName() == "Grass")
						{
							Grass *grass = new Grass(pos);
							_grassList.push_back(grass);
							_grassies[pos.x / X_STEP][pos.y / Y_STEP] = grass;
						}
						else if (layer->GetName() == "Boundary")
						{
							Brick* brick = new Boundary(pos);
							_brickList.push_back(brick);
							_bricks[pos.x / X_STEP][pos.y / Y_STEP] = brick;
						}
						else if (layer->GetName() == "Eagle NPC")
						{
							Eagle *eagle = new Eagle(pos, ET_EagleNPC);
							_eagleList.push_back(eagle);
						}
						else if (layer->GetName() == "Eagle Player")
						{
							Eagle *eagle = new Eagle(pos, ET_EaglePlayer);
							_eagleList.push_back(eagle);
						}
					}
				}
			}
		}
	}
}

void GameMap::Draw()
{
	// vẽ các object viên brick
	for (int i = 0; i < (int)_brickList.size(); i++)
	{
		_brickList[i]->Draw();
	}

	for (int i = 0; i < (int)_eagleList.size(); i++)
	{
		_eagleList[i]->Draw();
	}

	// vẽ các hình ảnh khác lên
	for (int i = 0; i < _map->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = _map->GetTileLayer(i);

		 //layer là các viên brick đã khởi tạo => bỏ qua
		if (layer->GetName() == "Brick" || 
			layer->GetName() == "Metal Brick" || 
			layer->GetName() == "Water" || 
			layer->GetName() == "Boundary" ||
			layer->GetName() == "Eagle")
			continue;

		if (!layer->IsVisible())
		{
			continue;
		}

		RECT sourceRECT;

		int tileWidth = _map->GetTileWidth();
		int tileHeight = _map->GetTileHeight();

		for (int m = 0; m < layer->GetHeight(); m++)
		{
			for (int n = 0; n < layer->GetWidth(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset *tileSet = _map->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = _tilesetList[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);
					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					// xác định hcn trong hình ảnh gốc
					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					// vị trí để vẽ map lên
					D3DXVECTOR2 pos(n * tileWidth + tileWidth / 2.f, 
						m * tileHeight + tileHeight / 2.f);

					// vẽ lên
					sprite->setPosition(pos);
					sprite->setRect(sourceRECT);
					sprite->setCenter(D3DXVECTOR2(tileWidth / 2.f, tileHeight / 2.f));
					sprite->Draw();
				}
			}

		}
	}

}

void GameMap::DrawInCamera(int posXMin, int posXMax, int posYMin, int posYMax)
{
	// vẽ các brick
	for (int x = posXMin; x <= posXMax; x++)
	{
		for (int y = posYMin; y <= posYMax; y++)
		{
			if (x < COUNT_X && y < COUNT_Y
				&& x >=0 && y >=0)
			{
				if (_bricks[x][y])
					_bricks[x][y]->Draw();
			}
		}	
	}
}

void GameMap::DrawGrassInCamera(int posXMin, int posXMax, int posYMin, int posYMax)
{
	//vẽ grass
	for (int x = posXMin; x <= posXMax; x++)
	{
		for (int y = posYMin; y <= posYMax; y++)
		{
			if (x < COUNT_X && y < COUNT_Y
				&& x >= 0 && y >= 0)
			{
				if (_grassies[x][y])
					_grassies[x][y]->Draw();
			}
		}
	}
}

vector<Brick*> GameMap::getBrickListAroundEntity(int posX, int posY)
{
	vector<Brick*> resuit;

	//quét từ posX - size => posX + size
	// posY - size => posY + size

	int size = 2;

	int xMin = posX - size < 0 ? 0 : posX - size;
	int xMax = posX + size > COUNT_X ? COUNT_X : posX + size;

	int yMin = posY - size < 0 ? 0 : posY - size;
	int yMax = posY + size > COUNT_Y ? COUNT_Y : posY + size;

	for (int x = xMin; x < xMax; x++) 
	{
		for (int y = yMin; y < yMax; y++) 
		{
			if (_bricks[x][y]) 
			{
				if (!_bricks[x][y]->IsDeleted)
					resuit.push_back(_bricks[x][y]);
			}
		}
	}
	return resuit;
}

bool GameMap::checkIsGrass(int posX, int posY) 
{
	if (!_grassies.empty()) 
	{
		return (_grassies[posX][posY]);
	}
	return false;
}
