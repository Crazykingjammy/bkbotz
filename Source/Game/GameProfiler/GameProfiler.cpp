#include "gameprofiler.h"



CGameProfile* CGameProfile::pInstance = 0;

//Default Constructor
CGameProfile::CGameProfile()
{
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		PlayerDataIndices[i] = -1;
	}



	Points4Win = 2;


	//Controls.
	fstream fFile;
	char  buf;
	//////////////////////////////////////////////////////////////////////////
	// player 1 controls
	fFile.open("Player1Controls.BUTTONMAP");
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[0]._shoot = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[0]._block = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[0]._sprint = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[0]._start = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[0]._rotateleft = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[0]._rotateright = atoi(&buf);
	fFile.close();

	//////////////////////////////////////////////////////////////////////////
	// player 2 controls
	fFile.open("Player2Controls.BUTTONMAP");
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[1]._shoot = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[1]._block = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[1]._sprint = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[1]._start = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[1]._rotateleft = atoi(&buf);
	fFile.read(&buf, sizeof(buf));
	this->ButtonLayout[1]._rotateright = atoi(&buf);
	fFile.close();




}


void CGameProfile::LoadPlayerData()
{

	for(int i = 0; i < PLAYER_COUNT; i++)
	{

		switch (PlayerDataIndices[i])
		{
			//Spinks
		case SFINKS:
			{

				//Player Strings
				strcpy(this->Players[i]._characterName,"Sfinks");
				strcpy(this->Players[i]._playerModelName,"Models/Botz/sfinks.x");
				strcpy(this->Players[i]._shieldModelName,"Models/Shield.x");
				strcpy(this->Players[i]._targetModelName,"Models/Target1.x");
				strcpy(this->Players[i]._heatMeterTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerIconTexture,"Textures/hud/Robot_Icons/SphinxIcon.bmp");
				strcpy(this->Players[i]._targetLifeTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerChant,		"Audio/SFX/CharacterChant/SfinxChant.wav");

				//Cannon Strings.
				strcpy(this->Players[i]._cannon._cannonModelName,"Models/Botz/sfinkshead.x");
				strcpy(this->Players[i]._cannon._ballModelName,"Models/balls/jewl.x");
				strcpy(this->Players[i]._cannon._ballIconTexture,"Textures/rubyicon.bmp");
				strcpy(this->Players[i]._cannon._cannonName,"DeadHead");

				//Cannon Data.
				Players[i]._cannon._shootHeat			= 0.55f;
				Players[i]._cannon._chargeHeat			= 20.0f;
				Players[i]._cannon._mass				= 15.0f;
				Players[i]._cannon._ballMass			= 3.0f;
				Players[i]._cannon._ballRadius			= 2.5f;
				Players[i]._cannon._chargeRate			= 1000.0f;
				Players[i]._cannon._shootForce			= 20000.0f;
				Players[i]._cannon._maxForce			= 29000.0f;
				Players[i]._cannon._shootRate			= 0.009f;
				Players[i]._cannon._shootDamage			= 3.995f;
				Players[i]._cannon._ballCount			= 15;

				//Player Data.
				Players[i]._blockHeat					= 1.0f;
				Players[i]._sprintHeat					= 0.07f;
				Players[i]._cooldownHeat				= 0.15f;
				Players[i]._SpeedForce					= 25000.0f;
				Players[i]._mass						= 30.0f;
				Players[i]._drag						= 240.0f;
				Players[i]._blockingRadius				= 7.0f;
				Players[i]._radius						= 3.0f;
				Players[i]._shieldMass					= 60.0f;
				Players[i]._maxKE						= 100.0f;
				Players[i]._pickupKE					= 75.0f;
				Players[i]._SprintSpeedForce			= 65000.0f;
				Players[i]._HeatResistance				= 10.0f;
				Players[i]._shieldReflectForce			= 6000.0f;
				Players[i]._shieldReflectDamage			= 10.0f;
				Players[i]._shieldShrinkHit				= 0.9f;
				Players[i]._shieldShrinkRate			= 0.1f;

				break;
			}

			//green.
		case GREENBOT:
			{

				//Player Strings
				strcpy(this->Players[i]._characterName,"Goob");
				strcpy(this->Players[i]._playerModelName,"Models/Botz/GreenBot.x");
				strcpy(this->Players[i]._shieldModelName,"Models/Shield.x");
				strcpy(this->Players[i]._targetModelName,"Models/Target2.x");
				strcpy(this->Players[i]._heatMeterTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerIconTexture,"Textures/hud/Robot_Icons/GoobIcon.bmp");
				strcpy(this->Players[i]._targetLifeTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerChant,		"Audio/SFX/CharacterChant/GreenChant.wav");

				//Cannon Strings.
				strcpy(this->Players[i]._cannon._cannonModelName,"Models/Botz/GreenbotHead.x");
				strcpy(this->Players[i]._cannon._ballModelName,"Models/balls/ruby.x");
				strcpy(this->Players[i]._cannon._ballIconTexture,"Textures/rubyicon.bmp");
				strcpy(this->Players[i]._cannon._cannonName,"SharpShiner");


				//Cannon Data.
				Players[i]._cannon._shootHeat			= 0.25f;
				Players[i]._cannon._chargeHeat			= 20.0f;
				Players[i]._cannon._mass				= 100.0f;
				Players[i]._cannon._ballMass			= 60.0f;
				Players[i]._cannon._ballRadius			= 2.5f;
				Players[i]._cannon._chargeRate			= 20000.0f;
				Players[i]._cannon._shootForce			= 420000.0f;
				Players[i]._cannon._maxForce			= 730000.0f;
				Players[i]._cannon._shootRate			= 0.55f;
				Players[i]._cannon._shootDamage			= 7.5f;
				Players[i]._cannon._ballCount			= 7;

				//Player Data.
				Players[i]._blockHeat					= 0.8f;
				Players[i]._sprintHeat					= 0.9f;
				Players[i]._cooldownHeat				= 0.05f;
				Players[i]._SpeedForce					= 17000.0f;
				Players[i]._mass						= 40.0f;
				Players[i]._drag						= 150.0f;
				Players[i]._blockingRadius				= 17.0f;
				Players[i]._radius						= 4.0f;
				Players[i]._shieldMass					= 160.0f;
				Players[i]._maxKE						= 100.0f;
				Players[i]._pickupKE					= 90.0f;
				Players[i]._SprintSpeedForce			= 120000.0f;
				Players[i]._HeatResistance				= 20.0f;
				Players[i]._shieldReflectForce			= 9000.0f;
				Players[i]._shieldReflectDamage			= 10.0f;
				Players[i]._shieldShrinkHit				= 1.7f;
				Players[i]._shieldShrinkRate			= 0.6f;


				break;
			}


		case BLUEBOT:
			{

				//Player Strings
				strcpy(this->Players[i]._characterName,"Goob");
				strcpy(this->Players[i]._playerModelName,"Models/Botz/BlueBot.x");
				strcpy(this->Players[i]._shieldModelName,"Models/Shield.x");
				strcpy(this->Players[i]._targetModelName,"Models/Target2.x");
				strcpy(this->Players[i]._heatMeterTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerIconTexture,"Textures/hud/Robot_Icons/blueIcon.bmp");
				strcpy(this->Players[i]._targetLifeTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerChant,		"Audio/SFX/CharacterChant/BlueChant.wav");

				//Cannon Strings.
				strcpy(this->Players[i]._cannon._cannonModelName,"Models/Botz/BluebotHead.x");
				strcpy(this->Players[i]._cannon._ballModelName,"Models/balls/rubyblue.x");
				strcpy(this->Players[i]._cannon._ballIconTexture,"Textures/rubyicon.bmp");
				strcpy(this->Players[i]._cannon._cannonName,"SharpShiner");


				//Cannon Data.
				Players[i]._cannon._shootHeat			= 2.795f;
				Players[i]._cannon._chargeHeat			= 20.0f;
				Players[i]._cannon._mass				= 200.0f;
				Players[i]._cannon._ballMass			= 100.0f;
				Players[i]._cannon._ballRadius			= 0.8f;
				Players[i]._cannon._chargeRate			= 100000.0f;
				Players[i]._cannon._shootForce			= 500000.0f;
				Players[i]._cannon._maxForce			= 800000.0f;;
				Players[i]._cannon._shootRate			= 0.05f;
				Players[i]._cannon._shootDamage			= 9.0f;
				Players[i]._cannon._ballCount			= 5;

				//Player Data.
				Players[i]._blockHeat					= 0.4f;
				Players[i]._sprintHeat					= 0.09f;
				Players[i]._cooldownHeat				= 0.05f;
				Players[i]._SpeedForce					= 45000.0f;
				Players[i]._mass						= 30.0f;
				Players[i]._drag						= 340.0f;
				Players[i]._blockingRadius				= 12.0f;
				Players[i]._radius						= 2.7f;
				Players[i]._shieldMass					= 60.0f;
				Players[i]._maxKE						= 100.0f;
				Players[i]._pickupKE					= 93.0f;
				Players[i]._SprintSpeedForce			= 55000.0f;
				Players[i]._HeatResistance				= 10.0f;
				Players[i]._shieldReflectForce			= 12000.0f;
				Players[i]._shieldReflectDamage			= 50.0f;
				Players[i]._shieldShrinkHit				= 1.2f;
				Players[i]._shieldShrinkRate			= 0.5f;


				break;
			}


			//Tbot
		case TBOT:
			{
				//Player Strings
				strcpy(this->Players[i]._characterName,"Tbot");
				strcpy(this->Players[i]._playerModelName,"Models/Botz/tbot.x");
				strcpy(this->Players[i]._shieldModelName,"Models/Shield.x");
				strcpy(this->Players[i]._targetModelName,"Models/Target2.x");
				strcpy(this->Players[i]._heatMeterTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerIconTexture,"Textures/hud/Robot_Icons/TBotIcon.bmp");
				strcpy(this->Players[i]._targetLifeTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerChant,		"Audio/SFX/CharacterChant/TBotChant.wav");

				//Cannon Strings.
				strcpy(this->Players[i]._cannon._cannonModelName,"Models/Botz/tbothead.x");
				strcpy(this->Players[i]._cannon._ballModelName,"Models/Tball.x");
				strcpy(this->Players[i]._cannon._ballIconTexture,"Textures/rubyicon.bmp");
				strcpy(this->Players[i]._cannon._cannonName,"PittyOnFools");


				//Cannon Data.
				Players[i]._cannon._shootHeat			= 0.55f;
				Players[i]._cannon._chargeHeat			= 20.0f;
				Players[i]._cannon._mass				= 50.0f;
				Players[i]._cannon._ballMass			= 10.0f;
				Players[i]._cannon._ballRadius			= 3.0f;
				Players[i]._cannon._chargeRate			= 10000.0f;
				Players[i]._cannon._shootForce			= 29000.0f;
				Players[i]._cannon._maxForce			= 98000.0f;
				Players[i]._cannon._shootRate			= 0.22f;
				Players[i]._cannon._shootDamage			= 5.35f;
				Players[i]._cannon._ballCount			= 12;

				//Player Data.
				Players[i]._blockHeat					= 0.5f;
				Players[i]._sprintHeat					= 0.56f;
				Players[i]._cooldownHeat				= 0.18f;
				Players[i]._SpeedForce					= 28000.0f;
				Players[i]._mass						= 50.0f;
				Players[i]._drag						= 200.0f;
				Players[i]._blockingRadius				= 7.5f;
				Players[i]._radius						= 2.0f;
				Players[i]._shieldMass					= 560.0f;
				Players[i]._maxKE						= 100.0f;
				Players[i]._pickupKE					= 75.0f;
				Players[i]._SprintSpeedForce			= 49000.0f;
				Players[i]._HeatResistance				= 10.0f;
				Players[i]._shieldReflectForce			= 6000.0f;
				Players[i]._shieldReflectDamage			= 40.0f;
				Players[i]._shieldShrinkHit				= 0.8f;
				Players[i]._shieldShrinkRate			= 0.15f;

				break;
			}

			//DeathBot
		case DEATHBOT:
			{
				//Player Strings
				strcpy(this->Players[i]._characterName,"DeathBot");
				strcpy(this->Players[i]._playerModelName,"Models/Botz/deathbot.x");
				strcpy(this->Players[i]._shieldModelName,"Models/Shield.x");
				strcpy(this->Players[i]._targetModelName,"Models/Target2.x");
				strcpy(this->Players[i]._heatMeterTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerIconTexture,"Textures/hud/Robot_Icons/Deathbot.bmp");
				strcpy(this->Players[i]._targetLifeTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerChant,		"Audio/SFX/CharacterChant/DeathChant.wav");

				//Cannon Strings.
				strcpy(this->Players[i]._cannon._cannonModelName,"Models/Botz/deathbothead.x");
				strcpy(this->Players[i]._cannon._ballModelName,"Models/balls/skull.x");
				strcpy(this->Players[i]._cannon._ballIconTexture,"Textures/rubyicon.bmp");
				strcpy(this->Players[i]._cannon._cannonName,"FireOfHell");


				//Cannon Data.
				Players[i]._cannon._shootHeat			= 1.45f;
				Players[i]._cannon._chargeHeat			= 20.0f;
				Players[i]._cannon._mass				= 50.0f;
				Players[i]._cannon._ballMass			= 17.0f;
				Players[i]._cannon._ballRadius			= 3.33f;
				Players[i]._cannon._chargeRate			= 10000.0f;
				Players[i]._cannon._shootForce			= 50199.0f;
				Players[i]._cannon._maxForce			= 90000.0f;
				Players[i]._cannon._shootRate			= 0.333f;
				Players[i]._cannon._shootDamage			= 5.7f;
				Players[i]._cannon._ballCount			= 3;

				//Player Data.
				Players[i]._blockHeat					= 1.2f;
				Players[i]._sprintHeat					= 0.2f;
				Players[i]._cooldownHeat				= 0.19f;
				Players[i]._SpeedForce					= 40000.0f;
				Players[i]._mass						= 20.0f;
				Players[i]._drag						= 200.0f;
				Players[i]._blockingRadius				= 6.0f;
				Players[i]._radius						= 6.5f;
				Players[i]._shieldMass					= 660.0f;
				Players[i]._maxKE						= 100.0f;
				Players[i]._pickupKE					= 75.0f;
				Players[i]._SprintSpeedForce			= 80000.0f;
				Players[i]._HeatResistance				= 30.0f;
				Players[i]._shieldReflectForce			= 13000.0f;
				Players[i]._shieldReflectDamage			= 40.0f;
				Players[i]._shieldShrinkHit				= 0.9f;
				Players[i]._shieldShrinkRate			= 0.1f;

				break;
			}

		case PFROST:
			{
				//Player Strings
				strcpy(this->Players[i]._characterName,"Pissed Mr Frosty");
				strcpy(this->Players[i]._playerModelName,"Models/Botz/PissedMrFrosty.x");
				strcpy(this->Players[i]._shieldModelName,"Models/Shield.x");
				strcpy(this->Players[i]._targetModelName,"Models/Target2.x");
				strcpy(this->Players[i]._heatMeterTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerIconTexture,"Textures/hud/Robot_Icons/PissedMrFrosty.bmp");
				strcpy(this->Players[i]._targetLifeTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerChant,		"Audio/SFX/CharacterChant/PFrostChant.wav");

				//Cannon Strings.
				strcpy(this->Players[i]._cannon._cannonModelName,"Models/Botz/PissedMrFrostyhead.x");
				strcpy(this->Players[i]._cannon._ballModelName,"Models/balls/snowball.x");
				strcpy(this->Players[i]._cannon._ballIconTexture,"Textures/rubyicon.bmp");
				strcpy(this->Players[i]._cannon._cannonName,"Top Hat Cap");


				//Cannon Data.
				Players[i]._cannon._shootHeat			= 0.55f;
				Players[i]._cannon._chargeHeat			= 20.0f;
				Players[i]._cannon._mass				= 670.0f;
				Players[i]._cannon._ballMass			= 750.0f;
				Players[i]._cannon._ballRadius			= 1.0f;
				Players[i]._cannon._chargeRate			= 600000.0f;
				Players[i]._cannon._shootForce			= 2000000.0f;
				Players[i]._cannon._maxForce			= 5000000.0f;
				Players[i]._cannon._shootRate			= 0.0841205f;
				Players[i]._cannon._shootDamage			= 6.54f;
				Players[i]._cannon._ballCount			= 13;

				//Player Data.
				Players[i]._blockHeat					= 0.5f;
				Players[i]._sprintHeat					= 0.4f;
				Players[i]._cooldownHeat				= 0.3f;
				Players[i]._SpeedForce					= 30000.0f;
				Players[i]._mass						= 80.0f;
				Players[i]._drag						= 70.0f;
				Players[i]._blockingRadius				= 10.0f;
				Players[i]._radius						= 4.0f;
				Players[i]._shieldMass					= 60.0f;
				Players[i]._maxKE						= 100.0f;
				Players[i]._pickupKE					= 40.0f;
				Players[i]._SprintSpeedForce			= 60000.0f;
				Players[i]._HeatResistance				= 10.0f;
				Players[i]._shieldReflectForce			= 13700.0f;
				Players[i]._shieldReflectDamage			= 40.0f;
				Players[i]._shieldShrinkHit				= 1.9f;
				Players[i]._shieldShrinkRate			= 0.19f;

				break;
			}



		case KENNY:
			{
				//Player Strings
				strcpy(this->Players[i]._characterName,"Kenny");
				strcpy(this->Players[i]._playerModelName,"Models/Botz/Kenny.x");
				strcpy(this->Players[i]._shieldModelName,"Models/Shield.x");
				strcpy(this->Players[i]._targetModelName,"Models/Target1.x");
				strcpy(this->Players[i]._heatMeterTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerIconTexture,"Textures/hud/Robot_Icons/KennyIcon.bmp");
				strcpy(this->Players[i]._targetLifeTexture,"Textures/HeatMeter.bmp");
				strcpy(this->Players[i]._playerChant,		"Audio/SFX/CharacterChant/KennyChant.wav");

				//Cannon Strings.
				strcpy(this->Players[i]._cannon._cannonModelName,"Models/Botz/Kennyhead.x");
				strcpy(this->Players[i]._cannon._ballModelName,"Models/balls/snowball.x");
				strcpy(this->Players[i]._cannon._ballIconTexture,"Textures/rubyicon.bmp");
				strcpy(this->Players[i]._cannon._cannonName,"DeadHead");


				//Cannon Data.
				Players[i]._cannon._shootHeat			= 2.55f;
				Players[i]._cannon._chargeHeat			= 0.075f;
				Players[i]._cannon._mass				= 50.0f;
				Players[i]._cannon._ballMass			= 5.0f;
				Players[i]._cannon._ballRadius			= 2.343f;
				Players[i]._cannon._chargeRate			= 10000.0f;
				Players[i]._cannon._shootForce			= 80000.0f;
				Players[i]._cannon._maxForce			= 100000.0f;
				Players[i]._cannon._shootRate			= 1.0f;
				Players[i]._cannon._shootDamage			= 18.64f;
				Players[i]._cannon._ballCount			= 1;

				//Player Data.
				Players[i]._blockHeat					= 1.5f;
				Players[i]._sprintHeat					= 0.4f;
				Players[i]._cooldownHeat				= 0.11f;
				Players[i]._SpeedForce					= 35000.0f;
				Players[i]._mass						= 50.0f;
				Players[i]._drag						= 200.0f;
				Players[i]._blockingRadius				= 15.0f;
				Players[i]._radius						= 2.76f;
				Players[i]._shieldMass					= 960.0f;
				Players[i]._maxKE						= 100.0f;
				Players[i]._pickupKE					= 75.0f;
				Players[i]._SprintSpeedForce			= 90000.0f;
				Players[i]._HeatResistance				= 5.0f;
				Players[i]._shieldReflectForce			= 15000.0f;
				Players[i]._shieldReflectDamage			= 40.0f;
				Players[i]._shieldShrinkHit				= 2.0f;
				Players[i]._shieldShrinkRate			= 0.5f;

				break;


		}


	}



}

}

void CGameProfile::SetPlayerIndices(int *playerID, int numPlayers)
{
	for (int i = 0; i < PLAYER_COUNT; i++)
	{
		if (i < numPlayers)
		{
			PlayerDataIndices[i] = playerID[i];
		}
	}

	LoadPlayerData();
}

void CGameProfile::SetLevelIndexToLoad(int levelIndx)
{
	this->IndexOfLevel = levelIndx;
}

void CGameProfile::DeleteInstance()
{
	//if we have an instance
	if(pInstance)
	{
		delete pInstance;
		pInstance = 0;
   	}
}


//Get Instance.
CGameProfile* CGameProfile::GetInstance()
{
	if(pInstance == 0)
	{
		pInstance = new CGameProfile;
	}

	return pInstance;
}