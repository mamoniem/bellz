// All rights reserved, Muhammad A.Moniem 2016 http:

#include "Bellz.h"
#include "PickupBase.h"


// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// All pickups start active
	bIsActive = true;

	// Add the static mesh component
	ThePickupStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = ThePickupStaticMesh;
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Getter to the active status
bool APickupBase::IsActive()
{
	return bIsActive;
}

// Setter of the active status
void APickupBase::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}

void APickupBase::OnGetCollected_Implementation()
{
	//Display to screen
	FString message = TEXT(" == Successfully Picked up == ") + GetName();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
}

