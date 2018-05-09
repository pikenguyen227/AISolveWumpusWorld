// Agent.cc

#include "Agent.h"
#include <algorithm>

using namespace std;

stateUtility::stateUtility(Location curr)
{
	currentLocation = curr;
	up = -1;
	down = -1;
	left = -1;
	right = -1;
}

stateUtility::~stateUtility()
{

}

Agent::Agent ()
{
	
}

Agent::~Agent ()
{

}

void Agent::Initialize ()
{
	this->currentLocation.X = 1;
	this->currentLocation.Y = 1;
	
	maxMoveCount = 0;
	this->currentOrientation = RIGHT;
	this->haveGold = false;
	this->arrow = 1;
//	this->locationLog = list<Location>();
	this->locationLog.push_back(Location(1,1));
	this->visited = list<Location>();
	this->posiblePitLocation = list<Location>();
	this->visited.push_back(Location(1,1));
	this->burstMode = false;
	if(this->SafeLocation.size() == 0)
	{
		this->AddSafeLocation(1,1);
	}
	if(!limitX)
	{
		this->wSizeX = 1;
	}
	if(!limitY)
	{
		this->wSizeY = 1;
	}
	this->currentAction = 0;
	this->nextActions = list<Action>();
	counter = 0;

	if(!tries)
	{
		// Assue world size is 4 x 4
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				this->UtilitiesWorld.push_back(stateUtility(Location(i,j)));
			}
		}
		printUtilitiesWorlds();
	}
}

void Agent::UpdateState(Location cur, Location next, Orientation ori)
{
	for(int i = 0; i < UtilitiesWorld.size(); i++)
	{
		list<stateUtility>::iterator it = UtilitiesWorld.begin();
		advance(it,i);
		if(it->currentLocation.X == cur.X && it->currentLocation.Y == cur.Y)
		{
			cout << "here" << endl;
		}
		break;
	}
}

void Agent::printUtilitiesWorlds()
{
	cout << "[up,right,down,left]" << endl;
	int tc = 0;
	for(int i = this->UtilitiesWorld.size()-1; i >= 0; i--)
	{
		if(tc == 4)
		{
			cout << endl;
			tc = 0;
		}
		list<stateUtility>::iterator it = UtilitiesWorld.begin();
		advance(it,i);
		cout << "[" << it->up << ", " << it->right << ", " << it->down 
												<< ", " << it->left << "] ";
		tc++;
	}
	cout << endl;
}

list<Action> Agent::FindPath(Location startLoc, Orientation startOri,
		Location goalLoc, Orientation goalOri)
{

	list<Action> aList;
	int curX = startLoc.X;
	int curY = startLoc.Y;
	Orientation  curOr = startOri;
	Action actionnow;

	list<Location> possiblePath;

	int maxX = 0;
	int maxY = 0;


	list<Location>::iterator findMaxit = SafeLocation.begin();
	while(findMaxit != SafeLocation.end())
	{
		if(maxX < findMaxit->X)
		{
			maxX = findMaxit->X;
		}
		if(maxY < findMaxit->Y)
		{
			maxY = findMaxit->Y;
		}
		*findMaxit++;
	}



	if(startLoc.X >= goalLoc.X && startLoc.Y >= goalLoc.Y)
	{
		while(curX != goalLoc.X || curY != goalLoc.Y)
		{
			if(curX+1 == goalLoc.X && curY == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curX++;
				break;
			}
			else if(curX == goalLoc.X && curY + 1 == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curY++;
				break;
			}
			else if(curX - 1 == goalLoc.X && curY == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curX--;
				break;
			}
			else if(curX == goalLoc.X && curY - 1 == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curY--;
				break;
			}
			if(startOri == DOWN)
			{
				if((curY < maxY)
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY + 1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY + 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY + 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY++));
				}
				else if((curX- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX - 1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX - 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX - 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX--,curY));
				}
				else if((curY- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY -1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY - 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY - 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY--));
				}
				else if((curX < maxX) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX +1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX + 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX + 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX++,curY));
				}			
			}
			else if(startOri == RIGHT)
			{
				if((curX- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX - 1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX - 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX - 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX--,curY));
				}
				else if((curY- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY -1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY - 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY - 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY--));
				}
				else if((curX < maxX) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX +1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX + 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX + 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX++,curY));
				}
				else if((curY < maxY)
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY + 1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY + 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY + 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY++));
				}
			}
			else if(startOri == LEFT)
			{
				if((curX < maxX) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX +1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX + 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX + 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX++,curY));
				}	
				else if((curY- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY -1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY - 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY - 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY--));
				}
				else if((curX- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX - 1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX - 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX - 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX--,curY));
				}
			
				else if((curY < maxY)
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY + 1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY + 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY + 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY++));
				}
			}
			else if(startOri == UP)
			{
				if((curY- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY -1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY - 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY - 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY--));
				}
				else if((curX- 1 > 0) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX - 1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX - 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX - 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX--,curY));
				}
				else if((curY < maxY)
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY + 1)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY + 1)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY + 1)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX,curY++));
				}
				else if((curX < maxX) 
				&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX +1, curY)) != this->SafeLocation.end()
				&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX + 1, curY)) == possiblePath.end()
				&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX + 1, curY)) == deadEnd.end())
				{
					possiblePath.push_back(Location(curX++,curY));
				}				
			}

			cout << "Possible path 1" << endl;
			cout << maxX << "    " << maxY << endl;
			cout << curX << "    " << curY << endl;
			printLocation(possiblePath);
			printLocation(deadEnd);
			//printLocation(SafeLocation);*/
		}
		possiblePath.push_back(Location(curX,curY));
		

	}

	else
	{
		while(curX != goalLoc.X || curY != goalLoc.Y)
		{
			if(curX+1 == goalLoc.X && curY == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curX++;
				break;
			}
			else if(curX == goalLoc.X && curY + 1 == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curY++;
				break;
			}
			else if(curX - 1 == goalLoc.X && curY == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curX--;
				break;
			}
			else if(curX == goalLoc.X && curY - 1 == goalLoc.Y)
			{
				possiblePath.push_back(Location(curX,curY));
				curY--;
				break;
			}
			if((curY < maxY) 
			&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY + 1)) != this->SafeLocation.end()
			&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY + 1)) == possiblePath.end()
			&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY + 1)) == deadEnd.end())
			{
				possiblePath.push_back(Location(curX,curY++));
			}
			else if((curX < maxX) 
			&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX + 1, curY)) != this->SafeLocation.end()
			&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX + 1, curY)) == possiblePath.end()
			&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX + 1, curY)) == deadEnd.end())
			{
				possiblePath.push_back(Location(curX++,curY));
			}
			else if((curY- 1 > 0) 
			&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX, curY - 1)) != this->SafeLocation.end()
			&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX, curY - 1)) == possiblePath.end()
			&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX, curY - 1)) == deadEnd.end())
			{
				possiblePath.push_back(Location(curX,curY--));
			}
			else if((curX - 1 > 0) 
			&& std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(curX - 1, curY)) != this->SafeLocation.end()
			&& std::find(possiblePath.begin(), possiblePath.end(), Location(curX - 1, curY)) == possiblePath.end()
			&& std::find(deadEnd.begin(), deadEnd.end(), Location(curX - 1, curY)) == deadEnd.end())
			{
				possiblePath.push_back(Location(curX--,curY));
			}		
			cout << "Possible path 2" << endl;
			cout << maxX << "    " << maxY << endl;
			cout << curX << "    " << curY << endl;
			printLocation(possiblePath);
			
		}
		possiblePath.push_back(Location(curX,curY));

	}

	list<Location>::iterator it = possiblePath.begin();
	int i;

	for(i = 0; i < possiblePath.size() - 1;i++)
	{
		list<Location>::iterator temp = it;
		*it++;
		if(temp-> X < it-> X)
		{
			while(curOr != RIGHT)
			{
				if(curOr == UP)
				{
					actionnow = TURNRIGHT;
					curOr = RIGHT;
				}
				else if(curOr == DOWN)
				{
					actionnow = TURNLEFT;
					curOr = RIGHT;
				}
				else if(curOr == LEFT)
				{
					actionnow = TURNLEFT;
					curOr = DOWN;
				}
				aList.push_back(actionnow);
			}
		}
		else if(temp->X > it->X)
		{
			while(curOr != LEFT)
			{
				if(curOr == UP)
				{
					actionnow = TURNLEFT;
					curOr = LEFT;
				}
				else if(curOr == DOWN)
				{
					actionnow = TURNRIGHT;
					curOr = LEFT;
				}
				else if(curOr == RIGHT)
				{
					actionnow = TURNLEFT;
					curOr = UP;
				}
				aList.push_back(actionnow);
			}
		}
		else if(temp->Y < it->Y)
		{
			while(curOr != UP)
			{
				if(curOr == LEFT)
				{
					actionnow = TURNRIGHT;
					curOr = UP;
				}
				else if(curOr == RIGHT)
				{
					actionnow = TURNLEFT;
					curOr = UP;
				}
				else if(curOr == DOWN)
				{
					actionnow = TURNLEFT;
					curOr = RIGHT;
				}
				aList.push_back(actionnow);
			}
		}
		else if(temp-> Y > it->Y)
		{
			while(curOr != DOWN)
			{
				if(curOr == LEFT)
				{
					actionnow = TURNLEFT;
					curOr = DOWN;
				}
				else if(curOr == RIGHT)
				{
					actionnow = TURNRIGHT;
					curOr = DOWN;
				}
				else if(curOr == UP)
				{
					actionnow = TURNLEFT;
					curOr = LEFT;
				}
				aList.push_back(actionnow);
			}
		}
		actionnow = GOFORWARD;
		aList.push_back(actionnow);
	}
	/*
	while(curOr != goalOri)
	{
		actionnow = TURNLEFT;
		aList.push_back(actionnow);
		if(curOr == LEFT)
		{
			curOr = DOWN;
		}
		else if(curOr == DOWN)
		{
			curOr = RIGHT;
		}
		else if(curOr == RIGHT)
		{
			curOr = UP;
		}
		else if(curOr == UP)
		{
			curOr = LEFT;
		}
	}*/
	
		
	/*	
	cout << "Possible path" << endl;
	printLocation(possiblePath);
	
	cout << "Action List:" << endl;
	printAction(aList);*/
	return aList;
}

void Agent::AddSafeLocation(int x,int y)
{
	if((std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(x,y))) == this->SafeLocation.end())
	{
		this->SafeLocation.push_back(Location(x,y));
	}
}

void Agent::RemoveSafeLocation(int x, int y)
{
	if((std::find(this->SafeLocation.begin(), this->SafeLocation.end(), Location(x,y))) != this->SafeLocation.end())
	{
		this->SafeLocation.remove(Location(x,y));
	}
}

void Agent::displayCurrentLocation()
{
	cout << "Current Location" << endl;
	cout << "x: " << currentLocation.X << endl;
	cout << "y: " << currentLocation.Y << endl;
}

void Agent::displayCurrentOrientation()
{
	cout << "Current orientation" << endl;
	cout << "Orientation: ";
	PrintOrientation(currentOrientation);
	cout << endl;
}

void Agent::displayWorldSize()
{
	cout << "Current World Size: " << this->wSizeX << " " << this->wSizeY << endl;;
}

void Agent::displayInfo()
{
	cout << "Have Gold: ";
	cout << this->haveGold << endl;
}

void Agent::updateWorldSize(Percept &percept)
{
	if(!percept.Bump)
	{
		if(currentOrientation == UP && currentLocation.Y > this->wSizeY)
		{
			this->wSizeY++;
		}
		else if(currentOrientation == RIGHT && currentLocation.X > this->wSizeX)
		{
			this->wSizeX++;	
		}
	}
	else
	{
		if(this->currentOrientation == UP)
		{
			limitY = true;
		}
		else if(this->currentOrientation == RIGHT)
		{
			limitX = true;
		}
	}

}

void Agent::printAction(list<Action> alist)
{
	
	int i;
	for(i = 0; i< alist.size(); i++)
	{
		list<Action>::iterator it = alist.begin();
		advance(it,i);
		if(*it == GOFORWARD)
		{
			cout << "action: GOFORWARD"<< endl;
		}
		else if(*it == TURNRIGHT)
		{
			cout << "action: TURNRIGHT"<< endl;
		}
		else if(*it == TURNLEFT)
		{
			cout << "action: TURNLEFT"<< endl;
		}
	} 
}


void Agent::printLocation(list<Location> alist)
{
	
	int i;
	for(i = 0; i< alist.size(); i++)
	{
		list<Location>::iterator it = alist.begin();
		advance(it,i);
		cout << "location: " << it->X  << " " << it->Y << endl;
	}
}

void Agent::AnalyzeTheSurrouding(Percept &percept)
{
	/*
	//this->possibleWumpusLocation = this->RemoveElapse(this->possibleWumpusLocation,this->SafeLocation);
	if(!percept.Stench && !percept.Breeze)
	{
		this->SafeLocation = this->AddingLocation(this->SafeLocation,list<Location>());
	}
	if(percept.Stench)
	{
		this->possibleWumpusLocation = this->AddingLocation(this->possibleWumpusLocation,this->SafeLocation);

	}
	if(percept.Breeze)
	{
		this->pitLocation = this->AddingLocation(this->pitLocation,this->SafeLocation);
	}
	*/
}

list<Location> Agent::RemoveElapse(list<Location> alist,
		list<Location> alist1)
{
	int i;
	for(i = 0; i< alist.size(); i++)
	{
		list<Location>::iterator it = alist.begin();
		advance(it,i);
		if((std::find(alist1.begin(), alist1.end(), Location(it->X,it->Y))) != alist1.end())
		{
			alist.remove(Location(it->X,it->Y));
		}
	}
	return alist;
}

list<Location> Agent::AddingLocation(list<Location> alist,list<Location> alist1)
{
	if(!limitX || currentLocation.X + 1 < this->wSizeX)
	{
		if((std::find(alist.begin(), alist.end(), Location(currentLocation.X+1,currentLocation.Y))) == alist.end())
		{
			if(alist1.size() == 0 || (std::find(alist1.begin(), alist1.end(), Location(currentLocation.X+1,currentLocation.Y))) == alist1.end())
				alist.push_back(Location(currentLocation.X+1,currentLocation.Y));
		}
	}
	if(!limitY || currentLocation.Y + 1 < this->wSizeY)
	{
		if((std::find(alist.begin(), alist.end(), Location(currentLocation.X,currentLocation.Y+1))) == alist.end())
		{
			if(alist1.size() == 0 || (std::find(alist1.begin(), alist1.end(), Location(currentLocation.X,currentLocation.Y+1))) == alist1.end())
				alist.push_back(Location(currentLocation.X,currentLocation.Y+1));
		}
	}
	if(currentLocation.Y - 1 > 0)
	{
		if((std::find(alist.begin(), alist.end(), Location(currentLocation.X,currentLocation.Y-1))) == alist.end())
		{
			if(alist1.size() == 0 || (std::find(alist1.begin(), alist1.end(), Location(currentLocation.X,currentLocation.Y-1))) == alist1.end())
				alist.push_back(Location(currentLocation.X,currentLocation.Y-1));
		}
	}
	if(currentLocation.X - 1 > 0)
	{
		if((std::find(alist.begin(), alist.end(), Location(currentLocation.X-1,currentLocation.Y))) == alist.end())
		{
			if(alist1.size() == 0 || (std::find(alist1.begin(), alist1.end(), Location(currentLocation.X-1,currentLocation.Y))) == alist1.end())
				alist.push_back(Location(currentLocation.X-1,currentLocation.Y));
		}
	}
	return alist;
}

void Agent::addDeadPlace(Location loc)
{
	this->deadPlace.push_back(loc);	
}

bool Agent::deadDetection()
{

	if(locationLog.size() > 2)
	{
		list<Location>::iterator it = this->locationLog.begin();
		advance(it,locationLog.size() - 2);
		printLocation(locationLog);
		if((locationLog.back().X == 1 && locationLog.back().Y == 1 
			&& locationLog.size() > 1))
		{
			if((it->X > 1 || it->Y > 1) && currentLocation.X == 1 && currentLocation.Y == 1 && currentOrientation == RIGHT)
			{
				return true;
			}
		} 
		
	}
	return false;
}

bool Agent::deadEndDetection()
{
	if(locationLog.size() > 3)
	{
		list<Location>::iterator it = this->locationLog.begin();
		advance(it,locationLog.size() - 3);	
		if(locationLog.back().X == it->X && locationLog.back().Y == it->Y)
		{
			it = this->locationLog.begin();
			advance(it,locationLog.size() - 2);	
			if((std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(it->X,it->Y))) == this->deadEnd.end())
			{
				return true;
			}
		}
	}
	return false;
}

Action Agent::Process (Percept& percept)
{
	
		char c;
		tries = 1;
		Action action;
		bool validAction = false;
		cout << "ASSUME:\n";
		cout << "The probability that there is a pit in a location is: 	  0.2 \n";
		cout << "The probability that there is not a pit in a location is: 0.8 \n";
		string choice = "";
		srand(time(0));



		if(locationLog.size() > 1 && percept.Bump)
		{
			locationLog.pop_back();
			this->currentLocation = locationLog.back();
		}
		
		if(this->deadEndDetection())
		{
			list<Location>::iterator it = this->locationLog.begin();
			advance(it,locationLog.size() - 2);
			this->deadEnd.push_back(Location(it->X,it->Y));
		}

		else
		{
			AddSafeLocation(locationLog.back().X,locationLog.back().Y);
		}
		
		updateWorldSize(percept);
		
		//printLocation(this->visited);
		//this->AnalyzeTheSurrouding(percept);
		//cout << "locationLog size: ";
		//cout << locationLog.size() << endl;
		//displayCurrentLocation();
		//displayInfo();
		//displayWorldSize();
		//cout << "List of safe Location: " << endl;
		//printLocation(this->SafeLocation);
		//cout << "Possible Wumpus Location: " << endl;
		//printLocation(this->possibleWumpusLocation);
		//cout << "Possible Pit Location: " << endl;
		//printLocation(this->pitLocation);
		//cout << "Dead place" << endl;
		//printLocation(this->deadPlace);
		//cout << "Action based on path" << endl;
		///printAction(nextActions);
		//cout << "Dead End" << endl;
		//printLocation(this->deadEnd);
		/*
		if(limitX)
		{
			cout << "Limit X reached" << endl;
		}
		if(limitY)
		{
			cout << "Limit Y reached" << endl;
		}*/

		if(percept.Stench)
		{
			this->addStenchLocation(this->currentLocation);
			if(foundW != true)
			{
				findWumpus(this->currentLocation);
				if(!isInList(deadPlace,Wumpus))
				{
					this->deadPlace.push_back(Wumpus);
				}
				
			}
		}
		if(percept.Breeze)
		{
			this->addBreezeLocation(this->currentLocation);
			//findPit(this->currentLocation);
		}
		cout << "****Guess Wumpus Location****: ";
		cout << Wumpus.X << "  " << Wumpus.Y << endl;
		cout << "****location Stench**** \n";
		printLocation(this->Stenched);
		cout << "****Guess Pit Location**** \n";
		printLocation(this->pitLocation);
		cout << "****location Breeze**** \n";
		printLocation(this->Breezed);
		cout << "****Possible Pit location**** \n";
		printLocation(posiblePitLocation);
		cout << "Loop detector (If counter > 4, agent is stuck in a loop)" << endl;
		cout << "Counter: " << counter << endl;
		if(isStuck)
		{
			cout << "Agent is now stucked or it is stucked on the last try." <<  endl;
		}

		if(!burstMode)
		{
			cout << "1. Manual" << endl;
			cout << "2. Auto (Single step)" << endl;
			cout << "3. Auto (Burst)" << endl;
			cout << "Enter your choice: ";

			while(choice.compare("1") != 0 
				&& choice.compare("2") != 0
				&& choice.compare("3") != 0)
			{
				getline(cin,choice);
			}
		}
		
		if(choice.compare("3") == 0)
		{
			this->burstMode = true;
		}

		while (!validAction)
		{
			if(choice.compare("1") == 0)
			{
				validAction = true;
				cout << "Action? ";
				cin >> c;
				if (c == 'f') {
					action = GOFORWARD;
				} else if (c == 'l') {
					action = TURNLEFT;
				} else if (c == 'r') {
					action = TURNRIGHT;
				} else if (c == 'g') {
					action = GRAB;
				} else if (c == 's') {
					action = SHOOT;
				} else if (c == 'c') {
					action = CLIMB;
				} else {
					cout << "Huh?" << endl;
					validAction = false;
				}
			}
			else if(choice.compare("2") == 0 
				|| burstMode)
			{
				validAction = true;
				if(this->currentLocation.X == 1 && this->currentLocation.Y == 1
					&& (this->haveGold == true || isStuck))
				{
					action = CLIMB;
				}
				else if(percept.Glitter && !this->haveGold)
				{
					action = GRAB;
					this->haveGold = true;
					this->foundG = true;
					this->goldLocation = this->currentLocation;
				}
				else if(percept.Stench && this->arrow > 0)
				{
					
							action = SHOOT;
							this->arrow--;
				}
				
				else if(percept.Bump && !foundG)
				{
					action = TURNLEFT;
				}
				else
				{
					if(!foundG)
					{
						action = this->findAction(this->currentLocation.X, this->currentLocation.Y,this->currentOrientation, percept);
						if(action == TURNLEFT || action == TURNRIGHT)
						{
							counter++;
							if(counter > 4)
							{
								isStuck = true;
							}
						}
						else
						{
							counter = 0;
						}
						maxMoveCount++;
					}
					else
					{
						if(this->nextActions.size() == 0)
						{
							if(!haveGold && currentLocation.X == 1 && currentLocation.Y == 1)
							{
								nextActions = FindPath(Location(1,1), currentOrientation,goldLocation,LEFT);
							}
							else
							{
								nextActions = FindPath(goldLocation, currentOrientation,Location(1,1),DOWN);
							}
						}
						
						this->actionIter = nextActions.begin();
						advance(actionIter,currentAction);
						currentAction++;
						action = *actionIter;
						if(currentAction == nextActions.size())
						{
							nextActions = list<Action>();
							currentAction = 0;
						}
						
					}
				}
			}
		}
		updateLocation(action);
		updateOrientation(action);
		actionLog.push_back(action);
		return action;
	
}

void Agent::addStenchLocation(Location cur)
{
	if(!isInList(Stenched,cur))
	{
		this->Stenched.push_back(cur);
	}
}

void Agent::addBreezeLocation(Location cur)
{
	if(!isInList(Breezed,cur))
	{
		this->Breezed.push_back(cur);
	}
}

void Agent::findPit(Location cur)
{
	if(isInList(Breezed,Location(cur.X,cur.Y)) 
		&& !isInList(Breezed, Location(cur.X - 1,cur.Y))
		&& !isInList(Breezed, Location(cur.X,cur.Y + 1))
		&& !isInList(Breezed, Location(cur.X + 1,cur.Y)))
	{
		if(cur.Y - 1 > 0 && !isInList(pitLocation,Location(cur.X,cur.Y-1)) && !isInList(SafeLocation,Location(cur.X,cur.Y-1)))
		{
			pitLocation.push_back(Location(cur.X,cur.Y-1));
		}
	}
	else if(isInList(Breezed,Location(cur.X,cur.Y)) 
		&& !isInList(Breezed, Location(cur.X - 1,cur.Y))
		&& !isInList(Breezed, Location(cur.X,cur.Y + 1))
		&& !isInList(Breezed, Location(cur.X,cur.Y - 1)))
	{
		if(cur.X + 1 < wSizeX && !isInList(pitLocation,Location(cur.X+1,cur.Y)) && !isInList(SafeLocation,Location(cur.X + 1,cur.Y)))
		{
			pitLocation.push_back(Location(cur.X + 1,cur.Y));
		}
	}
	else if(isInList(Breezed,Location(cur.X,cur.Y)) 
		&& !isInList(Breezed, Location(cur.X - 1,cur.Y))
		&& !isInList(Breezed, Location(cur.X,cur.Y - 1))
		&& !isInList(Breezed, Location(cur.X + 1,cur.Y)))
	{
		if(cur.Y + 1 < wSizeY && !isInList(pitLocation,Location(cur.X,cur.Y+1)) && !isInList(SafeLocation,Location(cur.X,cur.Y+1)))
		{
			pitLocation.push_back(Location(cur.X,cur.Y+1));
		}
	}
	else if(isInList(Breezed,Location(cur.X,cur.Y)) 
		&& !isInList(Breezed, Location(cur.X,cur.Y-1))
		&& !isInList(Breezed, Location(cur.X+1,cur.Y))
		&& !isInList(Breezed, Location(cur.X,cur.Y+1)))
	{
		if(cur.X - 1 > 0 && !isInList(pitLocation,Location(cur.X - 1,cur.Y)) && !isInList(SafeLocation,Location(cur.X - 1,cur.Y)))
		{
			pitLocation.push_back(Location(cur.X-1,cur.Y));
		}
	}
}

void Agent::findWumpus(Location cur)
{
	if(isInList(Stenched,Location(cur.X,cur.Y)) 
		&& isInList(Stenched, Location(cur.X + 1,cur.Y + 1))
		&& !isInList(Stenched, Location(cur.X,cur.Y+1)))
	{
		this->Wumpus.X = cur.X+1;
		this->Wumpus.Y = cur.Y;
		foundW = true;
	}
	if(isInList(Stenched,Location(cur.X,cur.Y)) 
		&& isInList(Stenched, Location(cur.X + 1,cur.Y + 1))
		&& !isInList(Stenched, Location(cur.X + 1,cur.Y)))
	{
		this->Wumpus.X = cur.X;
		this->Wumpus.Y = cur.Y + 1;
		foundW = true;
	}
	if(isInList(Stenched,Location(cur.X,cur.Y)) 
		&& isInList(Stenched, Location(cur.X + 1,cur.Y - 1))
		&& !isInList(Stenched, Location(cur.X,cur.Y - 1)))
	{
		this->Wumpus.X = cur.X + 1;
		this->Wumpus.Y = cur.Y;
		foundW = true;
	}
	if(isInList(Stenched,Location(cur.X,cur.Y)) 
		&& isInList(Stenched, Location(cur.X - 1,cur.Y + 1))
		&& !isInList(Stenched, Location(cur.X,cur.Y + 1)))
	{
		this->Wumpus.X = cur.X - 1;
		this->Wumpus.Y = cur.Y;
		foundW = true;
	}
	
}

bool Agent::isInList(list<Location> listC, Location cur)
{
	if(std::find(listC.begin(), listC.end(), Location(cur.X, cur.Y)) != listC.end())
	{
		return true;
	}
	return false;
}

double Agent::TwoBreeze()
{
	double defaultProPit = 0.2;
	double defaultProNotPit = 0.8;
	double v1 = defaultProPit * (0.2*0.2 + 0.8*0.2 + 0.2*0.8);
	double v2 = defaultProNotPit * (0.2*0.2 + 0.8*0.2);
	double alpha = 1 / (v1 + v2);
	return alpha * v1;
}

double Agent::TwoBreezeMidle()
{
	double defaultProPit = 0.2;
	double defaultProNotPit = 0.8;
	double v1 = defaultProPit * (0.2*0.2 + 0.8*0.2 + 0.2*0.8 + 0.8 * 0.8);
	double v2 = defaultProNotPit * (0.2*0.2 + 0.8*0.2);
	double alpha = 1 / (v1 + v2);
	return alpha * v1;
}

double Agent::OneBreeze()
{
	double defaultProPit = 0.2;
	double defaultProNotPit = 0.8;
	double v1 = defaultProPit * (0.2 + 0.8);
	double v2 = defaultProNotPit * (0.2);
	double alpha = 1 / (v1 + v2);
	return alpha * v1;
}
Action Agent::findAction(int curX, int curY, Orientation curOr, Percept& percept)
{
	UpdateState(Location(curX,curY),Location(curX+1,curY),curOr);
}
	/*
	double result = 0;
	if(curOr == RIGHT)
	{
		if((curY- 1 > 0) 
		&& (((std::find(this->visited.begin(), this->visited.end(), Location(curX, curY - 1)) == visited.end()))
		&& (std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end())))
		{
			if(percept.Breeze)
			{
				if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& curX - 1 > 0
				&& isInList(Breezed,Location(curX - 1,curY + 1)))
				{
					result =TwoBreeze();
				}
				else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
					&& curY - 1 > 0
					&& isInList(Breezed,Location(curX + 1,curY - 1)))
				{
					result = TwoBreezeMidle();
				}
				else
				{
					result = OneBreeze();
				}
			}
			cout << "Probability Turning :" << result << endl;
			if(result < 0.5 || isInList(SafeLocation,Location(curX, curY - 1)))
			{
				if(result < 0.5 && isInList(posiblePitLocation,Location(curX, curY - 1)))
				{
					posiblePitLocation.remove(Location(curX, curY - 1));
				}
				return TURNRIGHT;
			}
			else if(!isInList(posiblePitLocation,Location(curX,curY-1)))
			{
				return TURNRIGHT;

			}
		}
		if((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
		{
			if((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()) 
			&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end()))
			{
				if(percept.Breeze)
				{
					if(curX-1 > 0 && ((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY))
					&& isInList(Breezed,Location(curX-1,curY+1)))
					{
						result = TwoBreeze();
					}
					else if(curY-1 > 0 && ((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
					&& isInList(Breezed,Location(curX+1,curY-1)))
					{
						result = TwoBreeze();
					}
					else
					{
						result = OneBreeze();
					}
				}
				cout << "Probability :" << result << endl;
				if(result < 0.5 || isInList(SafeLocation,Location(curX + 1, curY)) || visited.size() == 1 || counter > 4)
				{
					if(result < 0.5 && isInList(posiblePitLocation,Location(curX + 1, curY)))
					{
						posiblePitLocation.remove(Location(curX + 1, curY));
					}
					return GOFORWARD;
				}
				else
				{
					if(!isInList(posiblePitLocation,Location(curX + 1, curY)) && !isInList(SafeLocation,Location(curX + 1, curY)))
						posiblePitLocation.push_back(Location(curX + 1, curY));
				}
				if(counter > 4 && !isInList(visited,Location(curX + 1, curY)))
				{
					return GOFORWARD;
				}
			}
		}
		return TURNLEFT;
	}
	else if(curOr == UP)
	{
		if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
		&& ((std::find(this->visited.begin(), this->visited.end(), Location(curX + 1, curY)) == visited.end())
		&& (std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())))
		{
			if(percept.Breeze)
			{
				if(curX-1 > 0 && ((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY))
				&& isInList(Breezed,Location(curX-1,curY+1)))
				{
					result = TwoBreeze();
				}
				else if(curY-1 > 0 && ((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
				&& isInList(Breezed,Location(curX+1,curY-1)))
				{
					result = TwoBreezeMidle();
				}
				else
				{
					result = OneBreeze();
				}
			}
			cout << "Probability Turning :" << result << endl;
			if(result < 0.5 || isInList(SafeLocation,Location(curX + 1, curY)) || visited.size() == 1)
			{
				if(result < 0.5 && isInList(posiblePitLocation,Location(curX + 1, curY)))
				{
					posiblePitLocation.remove(Location(curX + 1, curY));
				}
				return TURNRIGHT;
			}
			else if(!isInList(posiblePitLocation,Location(curX+1,curY)))
			{
				return TURNRIGHT;
			}
		}
		if((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
		{
			if((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
			&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end()))
			{		
				if(percept.Breeze)
				{
					if(curY-1 > 0 && curX - 1 > 0
					&& isInList(Breezed,Location(curX-1,curY - 1)))
					{
						result =TwoBreeze();
					}
					else if(curY-1 > 0 && ((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
						&& isInList(Breezed,Location(curX + 1,curY - 1)))
					{
						result = TwoBreeze();
					}
					else
					{
						result = OneBreeze();
					}
				}
				cout << "Probability :" << result << endl;
				if(result < 0.5 || isInList(SafeLocation,Location(curX, curY + 1)) ||  visited.size() == 1 || counter > 4)
				{
					if(result < 0.5 && isInList(posiblePitLocation,Location(curX, curY + 1)))
					{
						posiblePitLocation.remove(Location(curX, curY + 1));
					}
					return GOFORWARD;
				}
				else
				{
					if(!isInList(posiblePitLocation,Location(curX, curY + 1)) && !isInList(SafeLocation,Location(curX, curY +1)))
						posiblePitLocation.push_back(Location(curX, curY + 1));
				}
				if(counter > 4 && !isInList(visited,Location(curX, curY + 1)))
				{
					return GOFORWARD;
				}
			}
		}	
		return TURNLEFT; 
	}

	else if(curOr == LEFT)
	{
		if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY))
		&& ((std::find(this->visited.begin(), this->visited.end(), Location(curX, curY + 1)) == visited.end())
		&& (std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())))
		{
			if(percept.Breeze)
			{
				if(curY-1 > 0 && curX - 1 > 0
				&& isInList(Breezed,Location(curX-1,curY - 1)))
				{
					result =TwoBreeze();
				}
				else if(curY-1 > 0 && ((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
					&& isInList(Breezed,Location(curX + 1,curY - 1)))
				{
					result = TwoBreeze();
				}
				else
				{
					result = OneBreeze();
				}
			}
			cout << "Probability Turning:" << result << endl;
			if(result < 0.5 || isInList(SafeLocation,Location(curX, curY + 1)) ||  visited.size() == 1)
			{
				if(result < 0.5 && isInList(posiblePitLocation,Location(curX, curY + 1)))
				{
					posiblePitLocation.remove(Location(curX, curY + 1));
				}
				return TURNRIGHT;
			}
			else if(counter > 4 && !isInList(posiblePitLocation,Location(curX,curY+1)))
			{
				return TURNRIGHT;
			}
		}
		if(curX - 1 > 0) 
		{
			if((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
			&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end()))
			{
				if(percept.Breeze)
				{
					if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
					&& ((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
					&& isInList(Breezed,Location(curX + 1,curY + 1)))
					{
						result =TwoBreeze();
					}
					else if(curY-1 > 0 && curX-1 > 0
						&& isInList(Breezed,Location(curX - 1,curY - 1)))
					{
						result = TwoBreezeMidle();
					}
					else
					{
						result = OneBreeze();
					}
				}
				cout << "Probability :" << result << endl;
				if(result < 0.5 || isInList(SafeLocation,Location(curX - 1, curY)) || counter > 4)
				{
					if(result < 0.5 && isInList(posiblePitLocation,Location(curX - 1, curY)))
					{
						posiblePitLocation.remove(Location(curX - 1, curY));
					}
					return GOFORWARD;
				}
				else
				{
					if(!isInList(posiblePitLocation,Location(curX - 1, curY)) && !isInList(SafeLocation,Location(curX - 1, curY )))
						posiblePitLocation.push_back(Location(curX - 1, curY));
				}
				if(counter > 4 && !isInList(visited,Location(curX - 1, curY)))
				{
					return GOFORWARD;
				}
			}
		}
		return TURNLEFT;
	}
	else if(curOr == DOWN)
	{
		if((curX - 1 > 0) 
		&& ((std::find(this->visited.begin(), this->visited.end(), Location(curX - 1, curY)) == visited.end())
		&& (std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end())))
		{
			if(percept.Breeze)
			{
				if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& ((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
				&& isInList(Breezed,Location(curX + 1,curY + 1)))
				{
					result =TwoBreeze();
				}
				else if(curY-1 > 0 && curX-1 > 0
					&& isInList(Breezed,Location(curX - 1,curY - 1)))
				{
					result = TwoBreezeMidle();
				}
				else
				{
					result = OneBreeze();
				}
			}
			cout << "Probability Turning :" << result << endl;
			if(result < 0.5 || isInList(SafeLocation,Location(curX - 1, curY)))
			{
				if(result < 0.5 && isInList(posiblePitLocation,Location(curX - 1, curY)))
				{
					posiblePitLocation.remove(Location(curX - 1, curY));
				}
				return TURNRIGHT;
			}
			else if(!isInList(posiblePitLocation,Location(curX-1,curY)))
			{
				return TURNRIGHT;
			}
		}
		if(curY- 1 > 0)  
		{
			if((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end())
			&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end()))
			{
			
				if(percept.Breeze)
				{
					if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
					&& curX - 1 > 0
					&& isInList(Breezed,Location(curX - 1,curY + 1)))
					{
						result =TwoBreeze();
					}
					else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX))
						&& curY - 1 > 0
						&& isInList(Breezed,Location(curX + 1,curY - 1)))
					{
						result = TwoBreeze();
					}
					else
					{
						result = OneBreeze();
					}
				}
				cout << "Probability :" << result << endl;
				if(result < 0.5 || isInList(SafeLocation,Location(curX, curY - 1)) || counter > 4)
				{
					if(result < 0.5 && isInList(posiblePitLocation,Location(curX, curY - 1)))
					{
						posiblePitLocation.remove(Location(curX, curY - 1));
					}
					return GOFORWARD;
				}
				else
				{
					if(!isInList(posiblePitLocation,Location(curX, curY - 1)) && !isInList(SafeLocation,Location(curX, curY - 1)))
						posiblePitLocation.push_back(Location(curX, curY - 1));
				}
				if(counter > 4 && !isInList(visited,Location(curX, curY - 1)))
				{
					return GOFORWARD;
				}
			}
		}	
		return TURNLEFT; 
	}
}
*/
void Agent::updateOrientation(Action action)
{
	if(action == TURNLEFT)
	{
		if(currentOrientation == UP)
		{
			currentOrientation = LEFT;
		}
		else if(currentOrientation == LEFT)
		{
			currentOrientation = DOWN;
		}
		else if(currentOrientation == DOWN)
		{
			currentOrientation = RIGHT;
		}
		else if(currentOrientation == RIGHT)
		{
			currentOrientation = UP;
		}
	}
	else if(action == TURNRIGHT)
	{
		if(currentOrientation == UP)
		{
			currentOrientation = RIGHT;
		}
		else if(currentOrientation == RIGHT)
		{
			currentOrientation = DOWN;
		}
		else if(currentOrientation == DOWN)
		{
			currentOrientation = LEFT;
		}
		else if(currentOrientation == LEFT)
		{
			currentOrientation = UP;
		}
	}
}

void Agent::updateLocation(Action action)
{
	if(action == GOFORWARD)
	{
		if(this->currentOrientation == RIGHT)
		{	
			this->currentLocation.X++;	
		}
		else if(this->currentOrientation == UP)
		{
			this->currentLocation.Y++;	
		}
		else if(this->currentOrientation == LEFT)
		{	
			this->currentLocation.X--;	
		}
		else if(this->currentOrientation == DOWN)
		{	
			this->currentLocation.Y--;	
		}
		locationLog.push_back(this->currentLocation);
		visited.push_back(this->currentLocation);
	}
}

void Agent::GameOver (int score)
{
	list<Location>::iterator it = this->locationLog.begin();
	advance(it,locationLog.size() - 1);
	if((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(it->X,it->Y))) == this->deadPlace.end())
	{
		this->addDeadPlace(Location(it->X,it->Y));
	}
	this->RemoveSafeLocation(it->X,it->Y);
	cout << "MaxMove: " << maxMove << endl;
	cout << "MaxMoveCount: " << maxMoveCount << endl;
	if(!maxMoveFound && maxMoveCount > maxMove)
	{
		maxMove = maxMoveCount;
	}
	else if(maxMoveCount == maxMove)
	{
		maxMoveFound = true;
		//isStuck = true;
	}


}


	/*
	if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end()))))
	{
		if(curOr == RIGHT)
		{
			return GOFORWARD;
		}
	}
	//return null;
}
	/*
	if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end()))
		|| (std::find(this->locationLog.begin(), locationLog.end(), Location(curX, curY + 1)) == locationLog.end())))
	{
		if(curOr == UP)
		{
			return action;
		}
	}
	if((curX- 1 > 0) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end())) 
		|| (std::find(this->locationLog.begin(), locationLog.end(), Location(curX - 1, curY)) == locationLog.end())))
	{
		if(curOr == LEFT)
		{
			return action;
		}
	}
	if((curY- 1 > 0) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY-1)) == this->deadEnd.end())) 
		|| (std::find(this->locationLog.begin(), locationLog.end(), Location(curX, curY-1)) == locationLog.end())))
	{
		if(curOr == DOWN)
		{
			return action;
		}
		
	}*/



	/*
	if((this->limitX && curX + 1 <= this->wSizeX) || (!limitX))
	{
		if(std::find(this->locationLog.begin(), locationLog.end(), Location(curX + 1, curY)) == locationLog.end()
		&& curOr == RIGHT)
		{
			cout << "GO FOWAD RIGHT 1" << endl;
			return GOFORWARD;		
		}
		if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()
			&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())
		{
			if(curOr == RIGHT)
			{
				cout << "GO FOWAD RIGHT 2" << endl;
				return GOFORWARD;
			}
		}
		if(curOr == RIGHT)
		{
			if((this->limitY && curY + 1 <= this->wSizeY) || (!limitY))
			{
				cout << "RIGHT 1" << endl;
				return TURNLEFT;
			}
			else
			{
				cout << "RIGHT 2" << endl;
				return TURNRIGHT;
			}
		}
	}	
	
 	if((this->limitY && curY + 1 <= this->wSizeY) || (!limitY))
	{
		if(std::find(this->locationLog.begin(), locationLog.end(), Location(curX, curY + 1)) == locationLog.end()
		&& curOr == UP)
		{
			cout << "GO FOWAD UP 1" << endl;
			return GOFORWARD;		
		}
		if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY+1)) == deadPlace.end()
			&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY+1)) == this->deadEnd.end())
		{
			if(curOr == UP)
			{
				cout << "GO FOWAD UP 2" << endl;
				return GOFORWARD;
			}
		}
		else if(curOr == UP)
		{
			if((this->limitX && curX + 1 <= this->wSizeX) || (!limitX))
			{
				cout << "UP 1" << endl;
				return TURNRIGHT;
			}
			else
			{
				cout << "UP 2" << endl;
				return TURNLEFT;
			}
		}
	}
	if(curX - 1 > 0)
	{
		if(std::find(this->locationLog.begin(), locationLog.end(), Location(curX - 1, curY)) == locationLog.end()
		&& curOr == LEFT)
		{
			cout << "GO FOWAD LEFT 1" << endl;
			return GOFORWARD;		
		}
		if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()
			&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end())
		{
			if(curOr == LEFT)
			{
				cout << "GO FOWAD LEFT 2" << endl;
				return GOFORWARD;
			}
		}
		else if(curOr == LEFT)
		{
			if((this->limitY && curY + 1 <= this->wSizeY) || (!limitY))
			{
				cout << "LEFT 1" << endl;
				return TURNRIGHT;
			}
			else
			{
				cout << "LEFT 2" << endl;
				return TURNLEFT;
			}
		}
	}
	if(curY - 1 > 0)
	{
		if(std::find(this->locationLog.begin(), locationLog.end(), Location(curX, curY - 1)) == locationLog.end()
		&& curOr == DOWN)
		{
			cout << "GO FOWAD DOWN 1" << endl;
			return GOFORWARD;		
		}
		if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()
			&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end())
		{
			if(curOr == DOWN)
			{
				cout << "GO FOWAD DOWN 2" << endl;
				return GOFORWARD;
			}
		}
		else if(curOr == DOWN)
		{
			if((this->limitX && curX + 1 <= this->wSizeX) || (!limitX))
			{
				cout << "DOWN 1" << endl;
				return TURNLEFT;
			}
			else
			{
				cout << "DOWN 2" << endl;
				return TURNRIGHT;
			}
		}
	}
	return action;
	/*
	if(((this->limitY && curY + 1 <= this->wSizeY) || (!limitY))
		&& std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY + 1)) == locationLog.end()
		&& curOr == UP)
	{
		
		return GOFORWARD;
		
	}	
	if((curX- 1 > 0) 
		&& std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX - 1, curY)) == locationLog.end()
		&& curOr == LEFT)
	{
		
		
		return GOFORWARD;
		
		
	}
	if((curY- 1 > 0) 
		&& std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY - 1)) == locationLog.end()
		&& curOr == DOWN)	
	{
		return GOFORWARD;
	}
	/*
	if(((this->limitX && curX + 1 <= this->wSizeX)) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end()))))
	{
		if(curOr != RIGHT)
		{
			if(curOr == UP)
			{
				return TURNRIGHT;
			}
			else if(curOr == LEFT)
			{
				return TURNRIGHT;
			}
			else if(curOr == DOWN)
			{
				return TURNLEFT;
			}
			
		}
	}
	if(((this->limitY && curY + 1 <= this->wSizeY)) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end()))))
	{
		if(curOr != UP)
		{
			if(curOr == RIGHT)
			{
				return TURNLEFT;
			}
			else if(curOr == LEFT)
			{
				return TURNRIGHT;
			}
			else if(curOr == DOWN)
			{
				return TURNLEFT;
			}
			
		}
	}
	if((curX- 1 > 0) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end()))))
	{
		if(curOr != LEFT)
		{
			if(curOr == UP)
			{
				return TURNLEFT;
			}
			else if(curOr == DOWN)
			{
				return TURNRIGHT;
			}
			else if(curOr == RIGHT)
			{
				return TURNLEFT;
			}
			
		}
		
	}
	if((curY- 1 > 0) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY-1)) == this->deadEnd.end()))))
	{
		if(curOr != DOWN)
		{
			if(curOr == LEFT)
			{
				return TURNLEFT;
			}
			else if(curOr == RIGHT)
			{
				return TURNRIGHT;
			}
			else if(curOr == UP)
			{
				return TURNRIGHT;
			}
			
		}
		
	}*/
			
	//return GOFORWARD;


	/*	
	if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
		&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, startLoc.Y)) == deadPlace.end()
		&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, startLoc.Y)) == this->deadEnd.end())
		{
			while(startOri != RIGHT)
				{
					if(startOri == UP)
					{
						aList.push_back(TURNRIGHT);
						startOri = RIGHT;
					}
					else if(startOri == LEFT)
					{
						aList.push_back(TURNRIGHT);
						startOri = UP;
					}
					else if(startOri == DOWN)
					{
						aList.push_back(TURNLEFT);
						startOri = RIGHT;
					}
				}
			}
		}
		
			if((startLoc.X- 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X - 1, startLoc.Y)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X - 1, startLoc.Y)) == this->deadEnd.end())
			{
				while(startOri != LEFT)
				{
					if(startOri == UP)
					{
						aList.push_back(TURNLEFT);
						startOri = LEFT;
					}
					else if(startOri == RIGHT)
					{
						aList.push_back(TURNLEFT);
						startOri = UP;
					}
					else if(startOri == DOWN)
					{
						aList.push_back(TURNRIGHT);
						startOri = LEFT;
					}
				}
			}
		
	
			if(((this->limitY && startLoc.Y + 1 <= this->wSizeY) || (!this->limitY)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X, startLoc.Y + 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X, startLoc.Y + 1)) == this->deadEnd.end())
			{
				while(startOri != UP)
				{
					if(startOri == RIGHT)
					{
						aList.push_back(TURNLEFT);
						startOri = UP;
					}
					else if(startOri == LEFT)
					{
						aList.push_back(TURNRIGHT);
						startOri = UP;
					}
					else if(startOri == DOWN)
					{
						aList.push_back(TURNRIGHT);
						startOri = LEFT;
					}
				}
			}
		
		
			if((startLoc.Y - 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X, startLoc.Y-1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X, startLoc.Y - 1)) == this->deadEnd.end())
			{
				while(startOri != DOWN)
				{
					if(startOri == RIGHT)
					{
						aList.push_back(TURNRIGHT);
						startOri = DOWN;
					}
					else if(startOri == LEFT)
					{
						aList.push_back(TURNLEFT);
						startOri = DOWN;
					}
					else if(startOri == UP)
					{
						aList.push_back(TURNRIGHT);
						startOri = RIGHT;
					}
				}
			}
		
		aList.push_back(GOFORWARD);
		
	
		
	

	

	/*
	
		if(curOr == RIGHT)
		{		
			if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())
			{
				return GOFORWARD;
			}
			else if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())
			{
				return TURNLEFT;
			}
			else if((curY - 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end())
			{
				return TURNRIGHT;
			}
		}
		else if(curOr == UP)
		{
			if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())
			{
				return GOFORWARD;
			}
			else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())
			{
				return TURNRIGHT;
			}
			else if((curX - 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end())
			{
				return TURNLEFT;
			}
		}
		else if(curOr == LEFT)
		{		
			if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end())
			{
				return GOFORWARD;
			}
			else if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())
			{
				return TURNRIGHT;
			}
			else if((curY - 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end())
			{
				return TURNLEFT;
			}
		}
		else if(curOr == DOWN)
		{
			if(std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end())
			{
				return GOFORWARD;
			}
			else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())
			{
				return TURNLEFT;
			}
			else if((curX - 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end())
			{
				return TURNRIGHT;
			}
		}
	*/


/*
		if(startLoc.X < goalLoc.X)
		{
			if(((this->limitX && startLoc.X + 1 <= this->wSizeX) || (!this->limitX)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X + 1, startLoc.Y)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X + 1, startLoc.Y)) == this->deadEnd.end())
			{
				while(startOri != RIGHT)
				{
					if(startOri == UP)
					{
						aList.push_back(TURNRIGHT);
						startOri = RIGHT;
					}
					else if(startOri == LEFT)
					{
						aList.push_back(TURNRIGHT);
						startOri = UP;
					}
					else if(startOri == DOWN)
					{
						aList.push_back(TURNLEFT);
						startOri = RIGHT;
					}
				}
			}
		}
		else if(startLoc.X > goalLoc.X)
		{
			if((startLoc.X- 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X - 1, startLoc.Y)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X - 1, startLoc.Y)) == this->deadEnd.end())
			{
				while(startOri != LEFT)
				{
					if(startOri == UP)
					{
						aList.push_back(TURNLEFT);
						startOri = LEFT;
					}
					else if(startOri == RIGHT)
					{
						aList.push_back(TURNLEFT);
						startOri = UP;
					}
					else if(startOri == DOWN)
					{
						aList.push_back(TURNRIGHT);
						startOri = LEFT;
					}
				}
			}
		}
		else if(startLoc.Y < goalLoc.Y)
		{
			if(((this->limitY && startLoc.Y + 1 <= this->wSizeY) || (!this->limitY)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X, startLoc.Y + 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X, startLoc.Y + 1)) == this->deadEnd.end())
			{
				while(startOri != UP)
				{
					if(startOri == RIGHT)
					{
						aList.push_back(TURNLEFT);
						startOri = UP;
					}
					else if(startOri == LEFT)
					{
						aList.push_back(TURNRIGHT);
						startOri = UP;
					}
					else if(startOri == DOWN)
					{
						aList.push_back(TURNRIGHT);
						startOri = LEFT;
					}
				}
			}
		}
		else if(startLoc.Y > goalLoc.Y)
		{
			if((startLoc.Y - 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X, startLoc.Y-1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X, startLoc.Y - 1)) == this->deadEnd.end())
			{
				while(startOri != DOWN)
				{
					if(startOri == RIGHT)
					{
						aList.push_back(TURNRIGHT);
						startOri = DOWN;
					}
					else if(startOri == LEFT)
					{
						aList.push_back(TURNLEFT);
						startOri = DOWN;
					}
					else if(startOri == UP)
					{
						aList.push_back(TURNRIGHT);
						startOri = RIGHT;
					}
				}
			}
		}
		
		aList.push_back(GOFORWARD);
		
		if(startOri == RIGHT)
		{
			startLoc.X++;
		}
		else if(startOri == UP)
		{
			startLoc.Y++;
		}
		else if(startOri == LEFT)
		{
			startLoc.X--;
		}
		else if(startOri == DOWN)
		{
			startLoc.Y--;
		}*/


/*
if(((this->limitX && startLoc.X + 1 <= this->wSizeX) || (!this->limitX)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X + 1, startLoc.Y)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X + 1, startLoc.Y)) == this->deadEnd.end())
		{
			if(startOri != RIGHT)
			{
				if(startOri == UP)
				{
					aList.push_back(TURNRIGHT);
					startOri == RIGHT;
				}
				else if(startOri == LEFT)
				{
					aList.push_back(TURNRIGHT);
					startOri == UP;
				}
				else if(startOri == DOWN)
				{
					aList.push_back(TURNLEFT);
					startOri == RIGHT;
				}
				
			}
			else
			{
				aList.push_back(GOFORWARD);
				startLoc.X++;
			}
		}
		if((startLoc.X- 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X - 1, startLoc.Y)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X - 1, startLoc.Y)) == this->deadEnd.end())
		{
			if(startOri != LEFT)
			{
				if(startOri == UP)
				{
					aList.push_back(TURNLEFT);
					startOri == LEFT;
				}
				else if(startOri == RIGHT)
				{
					aList.push_back(TURNLEFT);
					startOri == UP;
				}
				else if(startOri == DOWN)
				{
					aList.push_back(TURNRIGHT);
					startOri == LEFT;
				}
				
			}
			else
			{
				aList.push_back(GOFORWARD);
				startLoc.X--;
			}
		}
		if(((this->limitY && startLoc.Y + 1 <= this->wSizeY) || (!this->limitY)) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X, startLoc.Y + 1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X, startLoc.Y + 1)) == this->deadEnd.end())
		{
			if(startOri != UP)
			{
				if(startOri == RIGHT)
				{
					aList.push_back(TURNLEFT);
					startOri == UP;
				}
				else if(startOri == LEFT)
				{
					aList.push_back(TURNRIGHT);
					startOri == UP;
				}
				else if(startOri == DOWN)
				{
					aList.push_back(TURNRIGHT);
					startOri == LEFT;
				}
				
			}
			else
			{
				aList.push_back(GOFORWARD);
				startLoc.Y++;
			}
		}
		if((startLoc.Y - 1 > 0) 
				&& std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(startLoc.X, startLoc.Y-1)) == deadPlace.end()
				&& std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(startLoc.X, startLoc.Y - 1)) == this->deadEnd.end())
		{
			if(startOri != DOWN)
			{
				if(startOri == RIGHT)
				{
					aList.push_back(TURNRIGHT);
					startOri == DOWN;
				}
				else if(startOri == LEFT)
				{
					aList.push_back(TURNLEFT);
					startOri == DOWN;
				}
				else if(startOri == UP)
				{
					aList.push_back(TURNRIGHT);
					startOri == RIGHT;
				}
				
			}
			else
			{
				aList.push_back(GOFORWARD);
				startLoc.Y--;
			}
		}
*/

/*

if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
		&& std::find(this->locationLog.begin(), locationLog.end(), Location(curX + 1, curY)) == locationLog.end())
	{
		if(curOr != RIGHT)
		{
			if(curOr == UP)
			{
				return TURNRIGHT;
			}
			else if(curOr == LEFT)
			{
				return TURNRIGHT;
			}
			else if(curOr == DOWN)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
	}	
	if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
		&& std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY + 1)) == locationLog.end())
	{
		if(curOr != UP)
		{
			if(curOr == RIGHT)
			{
				return TURNLEFT;
			}
			else if(curOr == LEFT)
			{
				return TURNRIGHT;
			}
			else if(curOr == DOWN)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
	}	
	if((curX- 1 > 0) 
		&& std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX - 1, curY)) == locationLog.end())
	{
		if(curOr != LEFT)
		{
			if(curOr == UP)
			{
				return TURNLEFT;
			}
			else if(curOr == DOWN)
			{
				return TURNRIGHT;
			}
			else if(curOr == RIGHT)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
		
	}
	if((curY- 1 > 0) 
		&& std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY - 1)) == locationLog.end())
	{
		if(curOr != DOWN)
		{
			if(curOr == LEFT)
			{
				return TURNLEFT;
			}
			else if(curOr == RIGHT)
			{
				return TURNRIGHT;
			}
			else if(curOr == UP)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
		
	}
*/

/*

if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())) 
		|| (std::find(this->locationLog.begin(), locationLog.end(), Location(curX + 1, curY)) == locationLog.end())))
	{
		if(curOr != RIGHT)
		{
			if(curOr == UP)
			{
				return TURNRIGHT;
			}
			else if(curOr == LEFT)
			{
				return TURNRIGHT;
			}
			else if(curOr == DOWN)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
	}
	if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end()))
		|| (std::find(this->locationLog.begin(), locationLog.end(), Location(curX, curY + 1)) == locationLog.end())))
	{
		if(curOr != UP)
		{
			if(curOr == RIGHT)
			{
				return TURNLEFT;
			}
			else if(curOr == LEFT)
			{
				return TURNRIGHT;
			}
			else if(curOr == DOWN)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
	}
	if((curX- 1 > 0) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end())) 
		|| (std::find(this->locationLog.begin(), locationLog.end(), Location(curX - 1, curY)) == locationLog.end())))
	{
		if(curOr != LEFT)
		{
			if(curOr == UP)
			{
				return TURNLEFT;
			}
			else if(curOr == DOWN)
			{
				return TURNRIGHT;
			}
			else if(curOr == RIGHT)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
		
	}
	if((curY- 1 > 0) 
		&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
		&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY-1)) == this->deadEnd.end())) 
		|| (std::find(this->locationLog.begin(), locationLog.end(), Location(curX, curY-1)) == locationLog.end())))
	{
		if(curOr != DOWN)
		{
			if(curOr == LEFT)
			{
				return TURNLEFT;
			}
			else if(curOr == RIGHT)
			{
				return TURNRIGHT;
			}
			else if(curOr == UP)
			{
				return TURNLEFT;
			}
			
		}
		else
		{
			return GOFORWARD;
		}
		
	}
*/

/*
		if(curX < goalLoc.X)
		{
			if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end()) 
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())))
				& curOr == RIGHT)
			{
				actionnow = GOFORWARD;
				curX++;
				aList.push_back(actionnow);
			}
		
		}
		else 
		{
			if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY + 1)) == locationLog.end())))
				&& curOr == RIGHT)
				{
					actionnow = TURNLEFT;
					curOr = UP;
					aList.push_back(actionnow);
				}
			else if((curY- 1 > 0) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY -1)) == locationLog.end())))
				&& curOr == RIGHT)
				{
					actionnow = TURNRIGHT;	
					curOr = DOWN;
					aList.push_back(actionnow);
				}
			else if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())))
				&& curOr == RIGHT)
				{
					actionnow = TURNLEFT;
					curOr = UP;
					aList.push_back(actionnow);
				}
			else if((curY- 1 > 0) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end()))) 
				&& curOr == RIGHT)
				{
					actionnow = TURNRIGHT;	
					curOr = DOWN;
					aList.push_back(actionnow);
				}
	
		}
		if(curY < goalLoc.Y)
		{
			if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())))
				&& curOr == UP)
			{	
				cout << "in here" << endl;
				actionnow = GOFORWARD;
				curY++;	
				aList.push_back(actionnow);
			}
		}
		else
		{
			if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
			&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX + 1, curY)) == locationLog.end())))
			&& curOr == UP)
			{
				actionnow = TURNRIGHT;
				curOr = RIGHT;
				aList.push_back(actionnow);
			}
			else if((curX- 1 > 0) 
			&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX - 1, curY)) == locationLog.end())))
			&& curOr == UP)
			{
				actionnow = TURNLEFT;
				curOr = LEFT;
				aList.push_back(actionnow);	
			}
			else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
			&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end())
			&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())))
			&& curOr == UP)
			{
				actionnow = TURNRIGHT;
				curOr = RIGHT;
				aList.push_back(actionnow);
			}
			else if((curX- 1 > 0) 
			&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
			&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end()))) 
			&& curOr == UP)
			{
				actionnow = TURNLEFT;
				curOr = LEFT;
				aList.push_back(actionnow);	
			}
		}
		
			/*
			else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX + 1, curY)) == locationLog.end())))
				&& curOr == UP)
			{		
				actionnow = TURNRIGHT;
				curOr = RIGHT;
			}
			else if((curX- 1 > 0) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX - 1, curY)) == locationLog.end())))
				&& curOr == UP)
			{
				actionnow = TURNLEFT;
				curOr = LEFT;	
			}			
			else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end())
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())))
				&& curOr == UP)
			{
				actionnow = TURNRIGHT;
				curOr = RIGHT;
			}
			else if((curX- 1 > 0) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end()))) 
				&& curOr == UP)
			{
				actionnow = TURNLEFT;	
				curOr = LEFT;
			}*/


/*
if(goalLoc.X > startLoc.X || goalLoc.Y > startLoc.Y)
		{
			if((((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
					&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) != deadPlace.end()) 
					|| (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) != this->deadEnd.end())))
					&& curOr == RIGHT) || curX == goalLoc.X)
				{
					if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
						&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY + 1)) == locationLog.end())))
						&& curOr == RIGHT)
						{
							actionnow = TURNLEFT;
							curOr = UP;
							aList.push_back(actionnow);
						}	
					else if((curY- 1 > 0) 
						&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY -1)) == locationLog.end())))
						&& curOr == RIGHT)
						{
							actionnow = TURNRIGHT;	
							curOr = DOWN;
							aList.push_back(actionnow);
						}
					else if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
						&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
						&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())))
						&& curOr == RIGHT)
						{
							actionnow = TURNLEFT;
							curOr = UP;
							aList.push_back(actionnow);
						}
					else if((curY- 1 > 0) 
						&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
						&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end()))) 
						&& curOr == RIGHT)
						{
							actionnow = TURNRIGHT;	
							curOr = DOWN;
							aList.push_back(actionnow);
						}

				}
			if((((this->limitY && curY <= this->wSizeY) || (!this->limitY)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) != deadPlace.end()) 
				|| (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) != this->deadEnd.end())))
				&& curOr == UP) || curY == goalLoc.Y)
				{
					if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
					&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX + 1, curY)) == locationLog.end())))
					&& curOr == UP)
					{
						actionnow = TURNRIGHT;
						curOr = RIGHT;
						aList.push_back(actionnow);
					}
					else if((curX- 1 > 0) 
					&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX - 1, curY)) == locationLog.end())))
					&& curOr == UP)
					{
						actionnow = TURNLEFT;
						curOr = LEFT;
						aList.push_back(actionnow);	
					}
					else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
					&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end())
					&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())))
					&& curOr == UP)
					{
						actionnow = TURNRIGHT;
						curOr = RIGHT;
						aList.push_back(actionnow);	
					}
					else if((curX- 1 > 0) 
					&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
					&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end()))) 
					&& curOr == UP)
					{
						actionnow = TURNLEFT;	
						curOr = LEFT;
						aList.push_back(actionnow);	
					}

				}
		}
*/

/*
	if((((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
					&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) != deadPlace.end()) 
					|| (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) != this->deadEnd.end())))
					&& curOr == RIGHT) || curX <= goalLoc.X)
				{
					while(curOr != RIGHT)
					{
						actionnow = TURNLEFT;
						aList.push_back(actionnow);
						if(curOr  == RIGHT)
						{
							curOr = UP;
						}
						else if(curOr == UP)
						{
							curOr = LEFT;
						}
						else if(curOr == LEFT)
						{
							curOr = DOWN;
						}
						else if(curOr == DOWN)
						{
							curOr = RIGHT;
						}			
					}
					if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
						&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY + 1)) == locationLog.end())))
						&& curOr == RIGHT)
						{
							actionnow = TURNLEFT;
							curOr = UP;
							aList.push_back(actionnow);
						}
					else if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
						&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
						&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())))
						&& curOr == RIGHT)
						{
							actionnow = TURNLEFT;
							curOr = UP;
							aList.push_back(actionnow);
						}	
					else if((curY- 1 > 0) 
						&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY -1)) == locationLog.end())))
						&& curOr == RIGHT)
						{
							actionnow = TURNRIGHT;	
							curOr = DOWN;
							aList.push_back(actionnow);
						}
					else if((curY- 1 > 0) 
						&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
						&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end()))) 
						&& curOr == RIGHT)
						{
							actionnow = TURNRIGHT;	
							curOr = DOWN;
							aList.push_back(actionnow);
						}

				}
			if((((this->limitY && curY <= this->wSizeY) || (!this->limitY)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) != deadPlace.end()) 
				|| (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) != this->deadEnd.end())))
				&& curOr == UP) || curY == goalLoc.Y)
				{
					while(curOr != UP)
					{
						actionnow = TURNLEFT;
						aList.push_back(actionnow);
						if(curOr  == RIGHT)
						{
							curOr = UP;
						}
						else if(curOr == UP)
						{
							curOr = LEFT;
						}
						else if(curOr == LEFT)
						{
							curOr = DOWN;
						}
						else if(curOr == DOWN)
						{
							curOr = RIGHT;
						}			
					}
					if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
					&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX + 1, curY)) == locationLog.end())))
					&& curOr == UP)
					{
						actionnow = TURNRIGHT;
						curOr = RIGHT;
						aList.push_back(actionnow);
					}
					else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
					&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end())
					&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())))
					&& curOr == UP)
					{
						actionnow = TURNRIGHT;
						curOr = RIGHT;
						aList.push_back(actionnow);	
					}
					else if((curX- 1 > 0) 
					&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX - 1, curY)) == locationLog.end())))
					&& curOr == UP)
					{
						actionnow = TURNLEFT;
						curOr = LEFT;
						aList.push_back(actionnow);	
					}
					else if((curX- 1 > 0) 
					&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
					&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end()))) 
					&& curOr == UP)
					{
						actionnow = TURNLEFT;	
						curOr = LEFT;
						aList.push_back(actionnow);	
					}

				}
		
				if(((curX - 1 > 0) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) != deadPlace.end()) 
				|| (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) != this->deadEnd.end())))
				&& curOr == LEFT) || curX >= goalLoc.X)
			{
				while(curOr != LEFT)
					{
						actionnow = TURNLEFT;
						aList.push_back(actionnow);
						if(curOr  == RIGHT)
						{
							curOr = UP;
						}
						else if(curOr == UP)
						{
							curOr = LEFT;
						}
						else if(curOr == LEFT)
						{
							curOr = DOWN;
						}
						else if(curOr == DOWN)
						{
							curOr = RIGHT;
						}			
					}
				if((curY- 1 > 0) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY - 1)) == locationLog.end())))
				&& curOr == LEFT)
				{
					actionnow = TURNLEFT;
					curOr = DOWN;
					aList.push_back(actionnow);			
				}
				else if((curY- 1 > 0) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) == deadPlace.end()) 
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) == this->deadEnd.end()))) 
				&& curOr == LEFT)
				{
					actionnow = TURNLEFT;
					curOr = DOWN;
					aList.push_back(actionnow);	
					
				}				
				else if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX, curY + 1)) == locationLog.end())))
				&& curOr == LEFT)
				{
					actionnow = TURNRIGHT;
					curOr = UP;
					aList.push_back(actionnow);	
				}	
				else if(((this->limitY && curY + 1 <= this->wSizeY) || (!this->limitY)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY + 1)) == deadPlace.end())
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY + 1)) == this->deadEnd.end())))
				&& curOr == LEFT)
				{
					actionnow = TURNRIGHT;
					curOr = UP;
					aList.push_back(actionnow);	
				}

			}
			
			if(((curY- 1 > 0) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX, curY - 1)) != deadPlace.end()) 
				|| (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX, curY - 1)) != this->deadEnd.end()))) 
				&& curOr == DOWN) || curY == goalLoc.Y)
			{
	
				while(curOr != DOWN)
					{
						actionnow = TURNLEFT;
						aList.push_back(actionnow);
						if(curOr  == RIGHT)
						{
							curOr = UP;
						}
						else if(curOr == UP)
						{
							curOr = LEFT;
						}
						else if(curOr == LEFT)
						{
							curOr = DOWN;
						}
						else if(curOr == DOWN)
						{
							curOr = RIGHT;
						}			
					}
				if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX + 1, curY)) == locationLog.end())))
				&& curOr == DOWN)
				{
					actionnow = TURNLEFT;
					curOr = RIGHT;
					aList.push_back(actionnow);	
				}
				else if(((this->limitX && curX + 1 <= this->wSizeX) || (!this->limitX)) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX + 1, curY)) == deadPlace.end())
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX + 1, curY)) == this->deadEnd.end())))
				&& curOr == DOWN)
				{
					actionnow = TURNLEFT;
					curOr = RIGHT;
					aList.push_back(actionnow);	
				}
				else if((curX- 1 > 0) 
				&& (((std::find(this->locationLog.begin(), this->locationLog.end(), Location(curX - 1, curY)) == locationLog.end())))
				&& curOr == DOWN)
				{
					actionnow = TURNRIGHT;
					curOr = LEFT;
					aList.push_back(actionnow);		
				}
				else if((curX- 1 > 0) 
				&& (((std::find(this->deadPlace.begin(), this->deadPlace.end(), Location(curX - 1, curY)) == deadPlace.end()) 
				&& (std::find(this->deadEnd.begin(), this->deadEnd.end(), Location(curX - 1, curY)) == this->deadEnd.end()))) 
				&& curOr == DOWN)
				{
					actionnow = TURNRIGHT;
					curOr = LEFT;
					aList.push_back(actionnow);		
				}
			}
*/

/*
	list<Location>::iterator it = this->Stenched.begin();
	int i;
	for(i = 0; i < this->Stenched.size() - 1;i++)
	{
		
			if(isInList(Stenched,Location(it->X,it->Y)) 
				&& isInList(Stenched, Location(it->X + 1,it->Y + 1))
				&& !isInList(Stenched, Location(it->X,it->Y+1)))
			{
				this->Wumpus.X = it->X+1;
				this->Wumpus.Y = it->Y;
				foundW = true;
			}
			else if(isInList(Stenched,Location(it->X,it->Y)) 
				&& isInList(Stenched, Location(it->X + 1,it->Y + 1))
				&& !isInList(Stenched, Location(it->X + 1,it->Y)))
			{
				this->Wumpus.X = it->X;
				this->Wumpus.Y = it->Y + 1;
				foundW = true;
			}
			else if(isInList(Stenched,Location(it->X,it->Y)) 
				&& isInList(Stenched, Location(it->X + 1,it->Y - 1))
				&& !isInList(Stenched, Location(it->X,it->Y - 1)))
			{
				this->Wumpus.X = it->X + 1;
				this->Wumpus.Y = it->Y;
				foundW = true;
			}
			else if(isInList(Stenched,Location(it->X,it->Y)) 
				&& isInList(Stenched, Location(it->X - 1,it->Y + 1))
				&& !isInList(Stenched, Location(it->X,it->Y + 1)))
			{
				this->Wumpus.X = it->X - 1;
				this->Wumpus.Y = it->Y;
				foundW = true;
			}
		
		*it++;

	}*/