// All rights reserved, Muhammad A.Moniem 2016 http://www.mamoniem.com/

#include "Bellz.h"
#include "Gladiator.h"
#include "GameDataTables.h"
#include "PaperSpriteComponent.h"
#include "GameDataTables.h"
#include "PickupBase.h"
#include "CoinPickup.h"

AGladiator::AGladiator()
{
	//Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	TotalHealth = 100.f;
	AttackRange = 25.f;

	jumppingVelocity = 600.f;

	//set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = jumppingVelocity;
	GetCharacterMovement()->AirControl = 0.2f;

	//Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EffectSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ClawEffect"));
	EffectSprite->AttachTo(CameraBoom);

	//Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	//are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	IsStillAlive = true;
	IsAttacking = false;
	WeaponIndex = 1;
	CollectedCoins = 0;
	CollectedCoinsValue = 0;

	//by default the inputs should be enabled, in case there is something ned to be tested
	OnSetPlayerController(true);
}

void AGladiator::BeginPlay()
{
	//Ask the datamanager to get all the tables datat at once and store them
	//AGameDataTables dataHolder;
	for (TActorIterator<AGameDataTables> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr)
		{
			//print theinstance name to screen
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, ActorItr->GetName());

			//Call the fetch to the tables, now we get all the datat stored. Why? simpley because keep readin everytime from the table itself is going to cost over your memory
			//but the mosts afe method, is just to read all the datat at once, and then keep getting whatever needed values from the storage we've .
			TablesInstance = *ActorItr;
			TablesInstance->OnFetchAllTables();
			
		}
	}

	
	//Creating an instance of the bellz load game
	UBellzSaveGame* BellzLoadGameInstance = Cast<UBellzSaveGame>(UGameplayStatics::CreateSaveGameObject(UBellzSaveGame::StaticClass()));
	//Access the correct slot and use the correct player index
	BellzLoadGameInstance = Cast<UBellzSaveGame>(UGameplayStatics::LoadGameFromSlot(BellzLoadGameInstance->SlotName, BellzLoadGameInstance->PlayerIndex));
	//get the needed value and store it in a loacl variable
	float loadedPlayerHealth = BellzLoadGameInstance->PlayerHealth;
	
	FString message;
	if (loadedPlayerHealth != 0.f)
	{
		TotalHealth = loadedPlayerHealth;

		message = TEXT("The health been loaded ---> ") + FString::SanitizeFloat(TotalHealth);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, message);
	}
	else
	{
		message = TEXT("---- No HEALTH value been found on save files ");
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGladiator::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	//Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Attack", IE_Released, this, &AGladiator::OnAttack);
	InputComponent->BindAction("ChangeWeapon", IE_Released, this, &AGladiator::OnChangeWeapon);

	InputComponent->BindAxis("MoveForward", this, &AGladiator::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGladiator::MoveRight);

	//We have 2 versions of the rotation bindings to handle different kinds of devices differently
	//"turn" handles devices that provide an absolute delta, such as a mouse.
	//"turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AGladiator::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AGladiator::LookUpAtRate);
}

void AGladiator::Jump()
{
	if (IsControlable && !IsAttacking)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void AGladiator::StopJumping()
{
	if (IsControlable)
	{
		bPressedJump = false;
		JumpKeyHoldTime = 0.0f;
	}
}

void AGladiator::OnAttack()
{
	if (IsControlable)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Doing Attack"));
		IsAttacking = true;
	}
}

void AGladiator::OnPostAttack()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Doing Post Attack"));
	IsAttacking = false;
}

void AGladiator::OnChangeWeapon()
{
	if (IsControlable)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("SWITCHING WEAPON"));
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::FromInt(WeaponIndex));
		if (WeaponIndex < TablesInstance->AllWeaponsData.Num())
		{
			WeaponIndex++;
		}
		else
		{
			WeaponIndex = 1;
		}
	}
}

void AGladiator::TurnAtRate(float Rate)
{
	if (IsControlable)
	{
		//calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGladiator::LookUpAtRate(float Rate)
{
	if (IsControlable)
	{
		//calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGladiator::OnCollectPickup()
{
	//Lets make a temp array to hold the overlapped coins
	TArray<AActor*> CollectedPickups;

	//let's check if something overlaps the player capsule, and then save the result at the temp array
	this->CapsuleComponent->GetOverlappingActors(CollectedPickups);

	// loop within the overlapped items
	for (int32 c = 0; c < CollectedPickups.Num(); c++)
	{
		// Cast the actor the base pickup
		APickupBase* const _tempPickup = Cast<APickupBase>(CollectedPickups[c]);
		// If cast is done, then let's make sure that it is active
		if (_tempPickup && !_tempPickup->IsPendingKill() && _tempPickup->IsActive())
		{
			//call the internal OnGetCollected
			_tempPickup->OnGetCollected();
			//Then cast to a coin, to make sure it is a coing, you can add different scenarios for the different types of collectables
			ACoinPickup* const _tempCoin = Cast<ACoinPickup>(_tempPickup);
			if (_tempCoin)
			{
				//if it is really a coin, then lets add 1 to the amount of collected coins, and then add the value of this coin to the score
				CollectedCoinsValue += _tempCoin->GetCoinValue();
				CollectedCoins += 1;

				FString message = TEXT("Collected another coinCoin ---> Total of ") +  FString::FromInt(CollectedCoins) + TEXT(" And all value now is") + FString::FromInt(CollectedCoinsValue);
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, message);
			}
			// Disable the pickup item
			_tempPickup->SetActive(false);
		}
	}
}

void AGladiator::OnSetPlayerController(bool status)
{
	IsControlable = status;
}

void AGladiator::OnChangeHealthByAmount(float usedAmount)
{
	TotalHealth -= usedAmount;
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ApplyGetDamageEffect"), ar, NULL, true);

	//A message to be printed to screen
	FString message;
	//Creating an instance of the bellz save class
	UBellzSaveGame* BellzSaveGameInstance = Cast<UBellzSaveGame>(UGameplayStatics::CreateSaveGameObject(UBellzSaveGame::StaticClass()));
	//Change the health value of the save class to match the current health value of the gladiator
	BellzSaveGameInstance->PlayerHealth = TotalHealth;
	//Store the health value to the save file
	UGameplayStatics::SaveGameToSlot(BellzSaveGameInstance, BellzSaveGameInstance->SlotName, BellzSaveGameInstance->PlayerIndex);
	//update the message that will be printed
	message = TEXT("The health been Saved ---> ") + FString::SanitizeFloat(TotalHealth);
	//print a message to the screen
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, message);
}

void AGladiator::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && IsControlable && !IsAttacking)
	{
		//find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		OnCollectPickup();
	}
}

void AGladiator::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && IsControlable && !IsAttacking)
	{
		//find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//add movement in that direction
		AddMovementInput(Direction, Value);
		OnCollectPickup();
	}
}
