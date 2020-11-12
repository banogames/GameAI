
#include"Astar.h";


bool Astar::isValid(int x, int y)
{ //If our Node is an obstacle it is not valid
	if (x < 0 || y < 0 || x >= (X_MAX / X_STEP) || y >= (Y_MAX / Y_STEP)) {
		return false;
	}
	return true;
}

bool Astar::isObstacle(int x, int y)
{
	int id = x + y * (X_MAX / X_STEP);
	return dicObstacles.count(id) != 0; //khac 0=> ton tai
}

bool Astar::isDestination(int x, int y, Node dest) {
		if (x == dest.vec.x && y == dest.vec.y) {
			return true;
		}
		return false;
	}

double Astar::calculateH(int x, int y, Node dest) {
		double H = (sqrt((x - dest.vec.x)*(x - dest.vec.x)
			+ (y - dest.vec.y)*(y - dest.vec.y)));
		return H;
}

vector<Astar::Node> Astar::makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest)
{
	try {
		cout << "Found a path" << endl;
		int x = dest.vec.x;
		int y = dest.vec.y;
		stack<Node> path;
		vector<Node> usablePath;

		while (!(map[x][y].vecParent.x == x && map[x][y].vecParent.y == y)
			&& map[x][y].vec.x != -1 && map[x][y].vec.y != -1)
		{
			path.push(map[x][y]);
			int tempX = map[x][y].vecParent.x;
			int tempY = map[x][y].vecParent.y;
			x = tempX;
			y = tempY;

		}
		path.push(map[x][y]);

		while (!path.empty()) {
			Node top = path.top();
			path.pop();
			//cout << top.x << " " << top.y << endl;
			usablePath.emplace_back(top);
		}
		return usablePath;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
}


vector<Astar::Node> Astar::aStar(Node player, Node dest) 
{
	vector<Node> empty;
	if (isValid(dest.vec.x, dest.vec.y) == false) {
		cout << "Destination is an obstacle" << endl;
		return empty;
		//Destination is invalid
	}
	if (isDestination(player.vec.x, player.vec.y, dest)) {
		cout << "You are the destination" << endl;
		return empty;
		//You clicked on yourself
	}
	bool closedList[(X_MAX / X_STEP)][(Y_MAX / Y_STEP)];

	//Initialize whole map
	array<array < Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> allMap;
	for (int x = 0; x < (X_MAX / X_STEP); x++) {
		for (int y = 0; y < (Y_MAX / Y_STEP); y++) {
			allMap[x][y].fCost = FLT_MAX;
			allMap[x][y].gCost = FLT_MAX;
			allMap[x][y].hCost = FLT_MAX;
			allMap[x][y].vecParent.reset(-1, -1);
			allMap[x][y].vec.reset(x, y);

			closedList[x][y] = false;
		}
	}

	//Initialize our starting list
	int x = player.vec.x;
	int y = player.vec.y;
	allMap[x][y].fCost = 0.0;
	allMap[x][y].gCost = 0.0;
	allMap[x][y].hCost = 0.0;
	allMap[x][y].vecParent.reset(x, y);

	vector<Node> openList;
	openList.emplace_back(allMap[x][y]);
	bool destinationFound = false;

	while (!openList.empty() && openList.size() < (X_MAX / X_STEP)*(Y_MAX / Y_STEP)) {
		Node node;
		do {
			//This do-while loop could be replaced with extracting the first
			//element from a set, but you'd have to make the openList a set.
			//To be completely honest, I don't remember the reason why I do
			//it with a vector, but for now it's still an option, although
			//not as good as a set performance wise.
			float temp = FLT_MAX;
			vector<Node>::iterator itNode;
			for (vector<Node>::iterator it = openList.begin();
				it != openList.end(); it = next(it)) {
				Node n = *it;
				if (n.fCost < temp) {
					temp = n.fCost;
					itNode = it;
				}
			}
			node = *itNode;
			openList.erase(itNode);
		} while (isValid(node.vec.x, node.vec.y) == false);

		x = node.vec.x;
		y = node.vec.y;
		closedList[x][y] = true;

		//For each neighbour starting from North-West to South-East
		for (int newX = -1; newX <= 1; newX++) {
			for (int newY = -1; newY <= 1; newY++) {
				double gNew, hNew, fNew;
				if (isValid(x + newX, y + newY) && !isObstacle(x + newX, y + newY)) {
					if (isDestination(x + newX, y + newY, dest))
					{
						//Destination found - make path
						allMap[x + newX][y + newY].vecParent.reset(x, y);
						destinationFound = true;
						return makePath(allMap, dest);
					}
					else if (closedList[x + newX][y + newY] == false)
					{
						gNew = node.gCost + 1.0;
						hNew = calculateH(x + newX, y + newY, dest);
						fNew = gNew + hNew;
						// Check if this path is better than the one already present
						if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
							allMap[x + newX][y + newY].fCost > fNew)
						{
							// Update the details of this neighbour node
							allMap[x + newX][y + newY].fCost = fNew;
							allMap[x + newX][y + newY].gCost = gNew;
							allMap[x + newX][y + newY].hCost = hNew;
							allMap[x + newX][y + newY].vecParent.reset(x, y);
							openList.emplace_back(allMap[x + newX][y + newY]);
						}
					}
				}
			}
		}
	}
	if (destinationFound == false) {
		cout << "Destination not found" << endl;
		return empty;
	}
}