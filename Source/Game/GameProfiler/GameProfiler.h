#ifndef __Game_Profiler_h__
#define __Game_Profiler_h__

#include "../../main.h"

#define MAX_NAME_SIZE 32
#define MAX_FILENAME_SIZE 256
#define PLAYER_COUNT 4

struct tButtonConfig
{
	int _block,_rotateleft,_rotateright ,_sprint,_shoot,_start, _previous, _foward;

	tButtonConfig()
	{
		_block			= 1;
		_rotateleft		= 4;
		_rotateright	= 5;
		_sprint			= 2;
		_shoot			= 0;
		_start			= 7;

		//Navigation
		_previous	= 1;
		_foward		= 0;

	}
};

struct tCannonData
{
	//Name and Filename string stuff.	
	char _cannonName[MAX_NAME_SIZE];
	char _cannonModelName[MAX_FILENAME_SIZE];
	char _ballModelName[MAX_FILENAME_SIZE];
	char _ballIconTexture[MAX_FILENAME_SIZE];

	//Heat Values
	float _shootHeat, _chargeHeat, _mass, _ballMass, _ballRadius;

	//Im sick of explaining this shit an what the fuck everyting does. <--- hehe i jus put ' . ' and visual assist opened up these fucking values._cannonModelName
 	float _shootForce, _shootRate, _shootDamage, _chargeRate, _maxForce;

	//Count the balls and go to sleep.
	int _ballCount;

	tCannonData()
	{
		strcpy(_cannonName,"_Blank_");
		strcpy(_cannonModelName, "DefaultCannon.x");
		strcpy(_ballModelName, "DefaultBall.x");
		strcpy(_ballIconTexture, "DefaultBallIcon.bmp");

		_shootHeat		= 0.25f;
		_chargeHeat		= 20.0f;
		_mass			= 15.0f;
		_ballMass		= 1.0f;
		_ballRadius		= 1.0f;
		_chargeRate		= 10000.f;
		_shootForce		= 50000.0f;
		_maxForce		= 70000.0f;
		_shootRate		= 0.5f;
		_shootDamage	= 30.0f; 
		_ballCount		= 10;
		
		

	}


};

struct tPlayerData
{
	//Name and Filename string stuff.
	char _characterName[MAX_NAME_SIZE];
	char _playerModelName[MAX_FILENAME_SIZE];
	char _shieldModelName[MAX_FILENAME_SIZE];
	char _targetModelName[MAX_FILENAME_SIZE];
	char _heatMeterTexture[MAX_FILENAME_SIZE];
	char _playerIconTexture[MAX_FILENAME_SIZE];
	char _targetLifeTexture[MAX_FILENAME_SIZE];
	char _playerChant[MAX_FILENAME_SIZE];
	

	//Heat Values
	float _blockHeat, _sprintHeat, _cooldownHeat, _HeatResistance;

	//Shield Stuff.
	float _shieldReflectForce, _shieldReflectDamage, _shieldMass;
	float _shieldShrinkRate, _shieldShrinkHit;

	//Speed
	float _SpeedForce, _SprintSpeedForce;
	
	//Physics stuff
	float _mass, _radius;

	//Radius for blocking
	float _blockingRadius;

	//Maybe we will use this.
	float _maxKE, _pickupKE;

	//Controlling Factors.
	float _drag;

	//hold us a cannon
	tCannonData _cannon;

	tPlayerData()
	{
		strcpy(_characterName, "_blank_");
		strcpy(_playerModelName, "DefaultPlayer.x");
		strcpy(_shieldModelName, "DefaultShield.x");
		strcpy(_targetModelName,	"DefaultTarget.x");
		strcpy(_heatMeterTexture,	"DefaultHeatMeter.bmp");
		strcpy(_playerIconTexture,	"DefaultPlayerIcon.bmp");
		strcpy(_targetLifeTexture,	"DefaultTargetIcon.bmp");
		strcpy(_playerChant,		"DefaultChant.wav");
		

		_blockHeat		= 1.0f;
		_sprintHeat		= 0.3f;
		_cooldownHeat	= 0.05f;
		_SpeedForce		= 30000.0f;
		_mass			= 30.0f;
		_drag			= 240.0f;
		_blockingRadius = 7.0f;
		_radius			= 3.0f;
		_shieldMass		= 60.0f;
		_maxKE			= 100.0f;
		_pickupKE		= 70.0f;

		_SprintSpeedForce		= 90000.0f;
		_HeatResistance			= 10.0f;
		_shieldReflectForce		= 26000.0f;
		_shieldReflectDamage	= 10.0f;
		_shieldShrinkRate		= 0.1f;
		_shieldShrinkHit		= 0.9f;
	}


};



class CGameProfile
{
	//Members

private:
	static CGameProfile *pInstance;
	
	int PlayerDataIndices[PLAYER_COUNT];

	//Trilogy of evil bitches
	CGameProfile();
	CGameProfile(const CGameProfile&);
	CGameProfile &operator= (const CGameProfile&);

public:

	enum PLAYERS {SFINKS = 0, GREENBOT = 5, BLUEBOT = 6, TBOT = 1, DEATHBOT = 9, PFROST = 7, KENNY = 3};

	enum LEVELS {EGYPT = 0, HITECH = 1, HELL = 2, BEACH = 3};

	//Me
	static void DeleteInstance();
	static CGameProfile* GetInstance();

	//Data
	tPlayerData Players[PLAYER_COUNT];
	tButtonConfig ButtonLayout[PLAYER_COUNT];

	int			IndexOfLevel;
	int			Points4Win;



	//Methods
public:

	void SetPlayerIndices(int *playerID, int numPlayers);
	void SetLevelIndexToLoad(int levelIndx);
	void LoadPlayerData();

};




#endif