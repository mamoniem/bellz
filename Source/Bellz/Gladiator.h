// All rights reserved, Muhammad A.Moniem 2016 http://www.mamoniem.com/

#pragma once
#include "GameDataTables.h"
#include "GameFramework/Character.h"
#include "Gladiator.generated.h"

UCLASS(config = Game)
class AGladiator : public ACharacter
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	//Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//The sprite used to draw effect, better and more contrallable than using the HUD or Textures
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* EffectSprite;

public:
	AGladiator();	

	//Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	//Base look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	//Base Jump velocity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	float jumppingVelocity;

	//Is the player dead or not
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	bool IsStillAlive;

	//is the palyer attacking right now ?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	bool IsAttacking;

	//is the palyer attacking right now ?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	int32 WeaponIndex;

	//is the palyer attacking right now ?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Collectebles")
	int32 CollectedCoins;

	//is the palyer attacking right now ?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Collectebles")
	int32 CollectedCoinsValue;

	//To be able to disable the player during cutscenes, menus, death....etc
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Collectebles")
	bool IsControlable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game DataTables")
	AGameDataTables* TablesInstance;

	//Return if the player dead or alive
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	bool GetIsStillAlive() const { return IsStillAlive; }

	//Enable or disable inputs
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	void OnSetPlayerController(bool status);

	//Perform attack
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	void OnChangeHealthByAmount(float usedAmount);

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float OnGetHealthAmount() const {return TotalHealth;}

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnPostAttack();

	//Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//Returns FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void Jump();

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void StopJumping();

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnAttack();

	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnChangeWeapon();

	//Called via input to turn at a given rate.
	void TurnAtRate(float Rate);

	//Called via input to turn look up/down at a given rate.
	void LookUpAtRate(float Rate);

	//Called when we collecteing a pickup
	UFUNCTION(BlueprintCallable, Category = "PickupSystem")
	void OnCollectPickup();

	//The health of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
	float TotalHealth;

	//The range for the enemy attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
	float AttackRange;

	

protected:
	//APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	//End of APawn interface
};

