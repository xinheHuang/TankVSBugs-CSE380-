#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\RandomBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"


/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
RandomBot::RandomBot()
{
	// INIT THE BASIC STUFF
//	initBot(initMin, initMax, initMaxVelocity);
	pp.setVelocity(0.0f, 0.0f);
	thinkCounter = 0;
	currentDirection=-1;
	pvx=0.0f;
	pvy=0.0f;
	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	
//	pickRandomCyclesInRange();


}

Bot* RandomBot::clone()
{
	RandomBot *botClone = new RandomBot();
	return botClone;
}


/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
/**void RandomBot::initBot(	unsigned int initMin,
									unsigned int initMax,
									unsigned int initMaxVelocity)
{
	// IF THE MAX IS SMALLER THAN THE MIN, SWITCH THEM
	if (initMax < initMin)
	{
		unsigned int temp = initMax;
		initMax = initMin;
		initMin = temp;
	}
	// IF THEY ARE THE SAME, ADD 100 CYCLES TO THE MAX
	else if (initMax == initMin)
		initMax += 100;

	// INIT OUR RANGE VARIABLES
	minCyclesBeforeThinking = initMin;
	maxCyclesBeforeThinking = initMax;

	// AND OUR VELOCITY CAPPER
	maxVelocity = initMaxVelocity;

	// AND WE'LL USE THIS TO ENSURE OUR BOTS ALL LOOK A LITTLE DIFFERENT
	animationRandomizer = (rand() % 45) + 5;
}
**/
/*
	pickRandomCyclesInRange - a randomized method for determining when this bot
	will think again. This method sets that value.
*/

/*
	pickRandomVelocity - calculates a random velocity vector for this
	bot and initializes the appropriate instance variables.
*/
void RandomBot::pickRandomDestination(Game *game)
{

	game->getGSM()->getWorld()->getWorldWidth();
	int worldCoordinateX = ((float)rand()/RAND_MAX)*game->getGSM()->getWorld()->getWorldWidth();
	int worldCoordinateY = ((float)rand()/RAND_MAX)*game->getGSM()->getWorld()->getWorldHeight();
	// NOW WE CAN SCALE OUR X AND Y VELOCITIES
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	GridPathfinder *pathfinder = spriteManager->getPathfinder();
	pathfinder->mapPath(this, (float)worldCoordinateX, (float)worldCoordinateY);

}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void RandomBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	//thinkCounter++;
//	if(thinkCounter % 120 == 0)
	{
		float pX = game->getGSM()->getSpriteManager()->getPlayer()->getB2Body()->GetPosition().x * 5.0f;
		float pY = game->getGSM()->getSpriteManager()->getPlayer()->getB2Body()->GetPosition().y * -5.0f;
		float bX = this->getB2Body()->GetPosition().x * 5.0f;
		float bY = this->getB2Body()->GetPosition().y * -5.0f;
		float vX,vY;
		if(fabsl(bX-pX) <= 200.0f && fabsl(bY-pY) <= 200.0f)
		{
			if(collide)
				return;
			//game->getGSM()->getSpriteManager()->getPathfinder()->mapPath(this, pX, pY);
			if (bX-pX>0) vX=-MAX_BUG_SPEED;
			else if (bX-pX<0) vX=MAX_BUG_SPEED;
			else vX=0;

			if (bY-pY>0) vY=MAX_BUG_SPEED;
			else if (bX-pX<0) vY=-MAX_BUG_SPEED;
			else vY=0;

			if (pvx==vX && pvy==vY) 
				return;
			pvx=vX;
			pvy=vY;
			if (!(vX==0 || vY==0)){
               int a= (float)rand()/RAND_MAX*2;
			   if (a==0)
				   vX=0;
			   else
				   vY=0;
			}
			if (vX<0) this->setCurrentState(L"MOVE LEFT");
			if (vY<0) this->setCurrentState(L"MOVE DOWN");
			if (vX>0) this->setCurrentState(L"MOVE RIGHT");
			if (vY>0)  this->setCurrentState(L"MOVE UP");
			this->getB2Body()->SetLinearVelocity(b2Vec2(vX, vY));
		}
		else{
		if (this->hasReachedDestination() || thinkCounter==120){
			//int worldCoordinateX = ((float)rand()/RAND_MAX)*game->getGSM()->getWorld()->getWorldWidth();
	      //  int worldCoordinateY = ((float)rand()/RAND_MAX)*game->getGSM()->getWorld()->getWorldHeight();
		//	game->getGSM()->getSpriteManager()->getPathfinder()->mapPath(this,(float)worldCoordinateX, (float)worldCoordinateY);
			int a= (float)rand()/RAND_MAX*4;
            if (currentDirection==a){
				a= (a+1)%4;
			}
			currentDirection=a;
			if (a==0){
			    vY = - MAX_BUG_SPEED;
				this->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, vY));
				this->setCurrentState(L"MOVE DOWN");
			}
			if (a==1){
			   vX =  MAX_BUG_SPEED;
				this->getB2Body()->SetLinearVelocity(b2Vec2(vX, 0.0f));
				this->setCurrentState(L"MOVE RIGHT");
			}
			if (a==2){
			     vY =  MAX_BUG_SPEED;
				this->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, vY));
				this->setCurrentState(L"MOVE UP");
			}
			if (a==3){
			    vX = - MAX_BUG_SPEED;
				this->getB2Body()->SetLinearVelocity(b2Vec2(vX, 0.0f));
				this->setCurrentState(L"MOVE LEFT");
			}

			thinkCounter = 0;
		}
	    thinkCounter++;
		}
	}
}