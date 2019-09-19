#ifndef __Player_h__
#define __Player_h__

#include "../Object.h"
#include "../Ball/Ball.h"
#include "../Barrell/Barrell.h"
#include "../Shield/Shield.h"
#include "../2D/Bar/Bar.h"
#include "../../Physics/PhysicsTimer.h"

#define PAUL_SUCKS_AT_TEKKEN 100

class CPlayer : public CBaseObject
{
public:



	//Pointer to the players barrell.
	CBarrell*	   m_Barrell;
	CBarrell*	   m_PreviousBarrell;
	CShield*	   m_Shield;
	CMeter*		   m_HeatMeter;
	CVector3f	   m_MovementBuffer;

	//Default Foward Positon
	CVector3f		m_DefaultFoward;


	//String for ballcount.
	char m_BallCountString[8];

	//Players Speed
	float m_fRegular, m_fSprint;
	float m_fFiringRate;
	float m_HeatFromSprint,
		  m_HeatFromBlocking,
		  m_HeatFromShooting;
	float m_CoolDownRate;
	float m_KE, m_Damage, m_MaxKE, m_PickupKE;
	float m_fTimeDelta;


	int m_IDNumber;
	bool m_ShieldActive;
	bool m_Sprinting;
	bool m_Moveing;
	bool m_LockOn;
	bool m_HoldOn;
	bool m_bOverHeated;
	bool m_bReadyToFire;
	bool m_bFullyCharged;

	int m_NotReadyIndex,m_ReadyIndex;

	CPhysicsTimer m_ShootTimer;
	CPhysicsTimer m_ChargeTimer;
	CPhysicsTimer m_PlayerUpdateTimer;
	CPhysicsTimer m_ShieldTimer;



public:

	CPlayer()
	{
		//Copy the name.		
		strcpy(m_ModelName,"DefaultPlayer.x");

		m_fRegular = 5.0f;
		m_fSprint  = 10.0f;

		m_MyBody.vOrientation.z = 1.0f;

		//Face Foward.
		m_DefaultFoward.z = 1.0f;

		//How much Damage is done by a hit (based on a bar filled with 100)
		m_Damage = 20.0f;

		//KE at the start, 100 is overheat.
		m_MaxKE = 100.0f;
		m_KE = 0.0f;

		//No cannon buddy.
		m_Barrell = NULL;
		m_PreviousBarrell = NULL;

		//No shield.
		m_Shield = NULL;

		//Set shield to not active
		m_ShieldActive = false;

		// the firing rate for the player
		m_fFiringRate = 0.25f;

		// Heat effective values
		m_HeatFromSprint = 0.30f;
		m_HeatFromBlocking = 1.0f;
		m_HeatFromShooting = .25f;

		m_PickupKE		= 75.0f;
		m_CoolDownRate = 0.04f;

		m_HeatMeter = new CMeter("Textures/DefaultHeatMap.bmp");
		m_HeatMeter->m_MaxCapacity = m_MaxKE;
		m_HeatMeter->Empty();

		m_Sprinting = false;
		m_Moveing = false;
		m_LockOn = true;
		m_HoldOn = false;
		m_bOverHeated = false;
		m_bReadyToFire = false;
		m_bFullyCharged = false;
	}

	//Constructor/Destructor.
	CPlayer(char* modelname, char* HeatMap)
	{
		//Copy the name.		
		strcpy(m_ModelName,modelname);
		
		m_fRegular = 5.0f;
		m_fSprint  = 10.0f;

		m_MyBody.vOrientation.z = 1.0f;
		
		//Face Foward.
		m_DefaultFoward.z = 1.0f;

		//How much Damage is done by a hit (based on a bar filled with 100)
		m_Damage = 20.0f;

		//KE at the start, 100 is overheat.
		m_MaxKE = 100.0f;
		m_KE = 0.0f;

		//No cannon buddy.
		m_Barrell = NULL;
		m_PreviousBarrell = NULL;

		//No shield.
		m_Shield = NULL;

		//Set shield to not active
		m_ShieldActive = false;
		m_PickupKE		= 75.0f;


		// Heat effective values
		m_HeatFromSprint = 0.30f;
		m_HeatFromBlocking = 1.0f;
		m_HeatFromShooting = .25f;

		m_CoolDownRate = 0.04f;

		m_HeatMeter = new CMeter(HeatMap);
		m_HeatMeter->m_MaxCapacity = m_MaxKE;
		m_HeatMeter->Empty();

		m_Sprinting = false;
		m_Moveing = false;
		m_LockOn = true;
		m_HoldOn = false;
		m_bOverHeated = false;
		m_bReadyToFire = false;
		m_bFullyCharged = false;
 	}
	~CPlayer(){delete m_HeatMeter;}

	void MoveByDirection();


	//Modifiers.
	void MoveX(float value);
	void MoveY(float value);
	void MoveZ(float value);

	void SetMaxKE(float value)
	{
		m_MaxKE = value;
		m_HeatMeter->m_MaxCapacity = value;
	}

	void Update(float timeDelta);

	//Actions

	//Block
	void ActivateShield();
	void DisableShield();

	//Tell the player he has been hit
	bool GetHit(float factor = 1.0f);

	//Pick up ball... returns false if the stock is full.
	bool PickUpBall(CBall* ball);

	//Attach a cannon to player if player does not have a cannon.
	// Return false if there was already a cannon.
	bool AttachCannon(CBarrell* barrell, bool dontcarebouttimer = false);

	//Attach shield to player.
	// Return false if there already was a shield.
	bool AttachShield(CShield* shield);

	//Rotate the cannon along the y axis.
	void RotateCannon(float angle);
	
	//Rotate the Cannon
	void RotateCannonLeft();
	void RotateCannonRight();

	//Rotate the cannon at a lock of 45
	void RotateCannonLeft45();
	void RotateCannonRight45();

	//These functions prolly are going to be private.

	bool ChargeShot();

	//Shoot from the charge shot.
	bool ReleaseChargetShot();

	//Player overheat
	void OverHeat();

	void AddHeat(float heat);
	void CoolOff();

	void RepositionBarrel();

	//Center the cannon.
	void CenterCannon() 
	{
		if(m_Barrell)
		{
			//Center the vector and the angle.
			m_Barrell->m_MyBody.vOrientation = m_DefaultFoward;
			m_Barrell->m_RenderingOrientation = m_RenderingOrientation;
		}
	}
	
	bool HasReachedMaxCharge()
	{
		return m_bFullyCharged;
	}

};


#endif