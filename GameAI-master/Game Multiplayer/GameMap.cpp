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
			layer->GetName() == "Tile Layer 1") 
		{
			for (int j = 0; j < _map->GetNumTilesets(); j++)
			{
				const Tmx::Tileset *tileSet = _map->GetTileset(j);

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

							// khởi tạo brick
							Brick* brick;
							if (layer->GetName() == "Brick")
							{
								brick = new BrickNormal(pos);
								_brickNorList.push_back((BrickNormal*)brick);
							}
							else if (layer->GetName() == "Metal Brick")
							{
								brick = new MetalBrick(pos);
							}
							else if (layer->GetName() == "Water")
							{
								brick = new Water(pos);
							}
							else if (layer->GetName() == "Tile Layer 1")
							{
								brick = new Boundary(pos);
							}

							_brickList.push_back(brick);
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

	// vẽ các hình ảnh khác lên
	for (int i = 0; i < _map->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = _map->GetTileLayer(i);

		 //layer là các viên brick đã khởi tạo => bỏ qua
		if (layer->GetName() == "Brick" || 
			layer->GetName() == "Metal Brick" || 
			layer->GetName() == "Water" || 
			layer->GetName() == "Tile Layer 1")
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