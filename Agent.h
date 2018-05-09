// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <list>

#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"
#include "WumpusWorld.h"

class stateUtility
{
public:
	stateUtility(Location curr);
	~stateUtility();
	Location currentLocation;
	double up;
	double down;
	double left;
	double right;
};

class Agent
{
public:
	Agent ();
	~Agent ();
	void Initialize ();
	void updateLocation(Action action);
	void updateOrientation(Action action);
	Action Process (Percept& percept);
	void GameOver (int score);
	void displayCurrentLocation();
	void displayCurrentOrientation();
	void displayInfo();
	void AddSafeLocation(int x,int y);
	void RemoveSafeLocation(int x,int y);
	list<Action> FindPath(Location startLoc, Orientation startOri,
		Location goalLoc, Orientation goalOri);
	void updateWorldSize(Percept &percept);
	void displayWorldSize();
	void printLocation(list<Location> alist);
	void AnalyzeTheSurrouding(Percept &percept);
	list<Location> AddingLocation(list<Location> alist,
		list<Location> alist1);
	list<Location> RemoveElapse(list<Location> alist,
		list<Location> alist1);
	Action findAction(int curX,int curY, Orientation curOr,Percept& percept);
	Action wonderingAroundTheWorld();
	void addDeadPlace(Location loc);
	bool deadDetection();
	bool deadEndDetection();
	void printAction(list<Action> alist);

	double TwoBreeze();
	double TwoBreezeMidle();
	double OneBreeze();

	void addStenchLocation(Location cur);
	void findWumpus(Location cur);
	bool isInList(list<Location> list, Location cur);

	Location Wumpus;
	bool foundW;

	list<Location> Breezed;
	list<Location> pitLocation;
	void addBreezeLocation(Location cur);
	void findPit(Location cur);
	double findPro(Location cur);

	list<Location> deadEnd;
	list<Location> unvisited;

	Location currentLocation;
	Orientation currentOrientation;
	bool haveGold;
	int arrow;
	int actionSequence;
	list<Action> actionLog;
	list<Location> locationLog;
	list<Location> visited;
	list<Location> SafeLocation;

	list<Location> possibleWumpusLocation;
	list<Location> posiblePitLocation;
	list<Location> deadPlace;
	Location goldLocation;
	list<Action> nextActions;
	list<Action>::iterator actionIter;
	list<Location> Stenched;

	list<stateUtility> UtilitiesWorld;
	int tries;
	void printUtilitiesWorlds();
	void UpdateState(Location cur, Location next, Orientation ori);

	int currentAction;
	int maxMove;
	int maxMoveCount;
	bool maxMoveFound;

	bool burstMode;
	int wSizeX;
	int wSizeY;
	bool limitX;
	bool limitY;
	bool foundG;
	bool cantfindgold;
	bool isStuck;
	int counter;
};

#endif // AGENT_H
