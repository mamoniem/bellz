// All rights reserved, Muhammad A.Moniem 2016 http://www.mamoniem.com/

#include "Bellz.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "Gladiator.h"
#include "Perception/PawnSensingComponent.h"

//Set all the defualt values using the constructor
AEnemy::AEnemy()
{
 	//Tick needed or no, it's a defualt from UE
	PrimaryActorTick.bCanEverTick = true;

	//Set the AI Controller class.
	AIControllerClass = AEnemyAIController::StaticClass();

	//Set the enemy behavior values
	TotalHealth = 100;
	AttackRange = 100;
	AttackDamage = 10;
	IsDead = false;
	IsAttacking = false;

	//Because the enmy have to rotate to face the running direction, The Yaw rotation needed!
	bUseControllerRotationYaw = true;

	/*Build the sensing component, and set the required values for it. You can publicate some
	variables to control those values, but for my AI within this game, it is fine to just set some base values here*/
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->SensingInterval = .25f; // 4 times per second
	PawnSensor->bOnlySensePlayers = true;
	PawnSensor->SetPeripheralVisionAngle(85.f);

	bodySphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BodyTriggerSphere"));
	bodySphereTrigger->SetSphereRadius(150.f);
	bodySphereTrigger->AttachTo(Mesh);

	FColor handsTriggersColor = FColor (0, 0, 255, 255);

	leftHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandTriggerSphere"));
	leftHandTrigger->SetSphereRadius(70.f);
	leftHandTrigger->AttachTo(Mesh, "hand_lf", EAttachLocation::SnapToTarget);
	leftHandTrigger->ShapeColor = handsTriggersColor;
	leftHandTrigger->bGenerateOverlapEvents = 0;

	rightHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandTriggerSphere"));
	rightHandTrigger->SetSphereRadius(70.f);
	rightHandTrigger->AttachTo(Mesh, "hand_rt", EAttachLocation::SnapToTarget);
	rightHandTrigger->ShapeColor = handsTriggersColor;
	rightHandTrigger->bGenerateOverlapEvents = 0;

	//Just make sure to not use the Enemy::Character capsul as the navigation collision, and use the agent and set it's radius to something fits the enemy size
	//the main goal is to avoid as much as possible the cases when the enemy meshes intersecting with eachother, or with the environment
	GetCharacterMovement()->NavAgentProps.AgentRadius = 400.f;
	GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(false);

}

//Called when the game starts or when spawned -> Default by UE
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

//Called every frame -> Default by UE
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//Called to bind functionality to input -> Default by UE
void AEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

/*The post initialize required here in order to registering the delegate functions for seeing and hearing
, Those functions came from "Perception/PawnSensingComponent.h" */
void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensor->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &AEnemy::OnHearNoise);

	// Register to the delegate of OnComponentBeginOverlap
	leftHandTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHandTriggerOverlap);
	rightHandTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHandTriggerOverlap);
}

void AEnemy::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);
	Super::FaceRotation(CurrentRotation, DeltaTime);
}

//TODO: Update this logic and comment it
void AEnemy::OnPerformAttack()
{
	//UE_LOG(LogClass, Log, TEXT("===========>>>> Starting Melee Attack"));
	const FVector TraceStart = GetActorLocation();
	const FVector LookingDirection = GetActorRotation().Vector();
	const FVector TraceEnd = TraceStart + LookingDirection * AttackRange;



	//perform a sphere sweep
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
	FCollisionQueryParams TracingParameters(WeaponFireTag, true);
	TracingParameters.AddIgnoredActor(this);
	TracingParameters.bTraceAsyncScene = true;
	TracingParameters.bReturnPhysicalMaterial = true;


	FHitResult Hit(ForceInit);
	GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(25), TracingParameters);

	//if it really hit something
	if (Hit.Actor != NULL)
	{
		ACharacter *character = Cast<ACharacter>(Hit.GetActor());
		if (character != NULL)
		{
			FString debuggingString = character->GetName();
			//UE_LOG(LogClass, Log, TEXT("The enmey has found ^_^_^_^_^_^_^_^__^_^_^_^__^_^ %s"), *debuggingString);

			FPointDamageEvent PointDmg;
			PointDmg.DamageTypeClass = UDamageType::StaticClass();
			PointDmg.HitInfo = Hit;
			PointDmg.ShotDirection = LookingDirection;
			PointDmg.Damage = AttackDamage;
			character->TakeDamage(AttackDamage, PointDmg, Controller, this);

			IsAttacking = true;

			//this two been moved to their own function, so it is easier to call them from within the animation notification at the right timing
			//leftHandTrigger->SetActive(true);
			//rightHandTrigger->SetActive(true);
		}
	}
}

//just make sure to stop the attack animations, and so as the triggers effect
void AEnemy::OnPreAttack()
{
	leftHandTrigger->bGenerateOverlapEvents = 1;
	rightHandTrigger->bGenerateOverlapEvents = 1;

	FString message = TEXT(">>>>>>>>>>> Attack just goind to start <<<<<<<<<<<< ");
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
}

//just make sure to stop the attack animations, and so as the triggers effect
void AEnemy::OnPostAttack()
{
	IsAttacking = false;

	leftHandTrigger->bGenerateOverlapEvents = 0;
	rightHandTrigger->bGenerateOverlapEvents = 0;

	FString message = TEXT(">>>>>>>>>>> Attack just finished <<<<<<<<<<<< ");
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
}

//The hear noise implementation
void AEnemy::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT(">>>>>>>>>>> Heard Actor <<<<<<<<<<<< ") + OtherActor->GetName() + VolumeDesc;
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
	//UE_LOG(LogClass, Log, TEXT(">>>>>>>>>>> Heard Actor <<<<<<<<<<<<"));   
}

//The see pawn implementation
void AEnemy::OnSeePawn(APawn *OtherPawn)
{
	FString message = TEXT(">>>>>>>>>>> Saw Actor <<<<<<<<<<<< ") + OtherPawn->GetName();
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
	//UE_LOG(LogClass, Log, TEXT(">>>>>>>>>>> Saw Actor <<<<<<<<<<<<"));
}

//On overlap component callback
void AEnemy::OnHandTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AGladiator* const _tempGladiator =  Cast<AGladiator>(OtherActor);
	if (_tempGladiator)
	{
		FString message = TEXT("=== HIT PLAYER WITH HAND ==== ");
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);

		//in case it hit the player, it is good idea to disable the triggers, this way we'll make sure that the triggers will not over calculate with each single hit
		leftHandTrigger->bGenerateOverlapEvents = 0;
		rightHandTrigger->bGenerateOverlapEvents = 0;

		_tempGladiator->OnChangeHealthByAmount(5.f);
	}
}

