// All rights reserved, Muhammad A.Moniem 2016 http:

#pragma once

#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UCLASS()
class BELLZ_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Return the static mesh of the pickup instance
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ThePickupStaticMesh; }

	//The pickup active or not!
	UFUNCTION(BlueprintPure, Category = "Pickups")
		bool IsActive();

	//Change the active status from the outside
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void SetActive(bool NewPickupState);

	//When the pickup get collecetd, this function should be called
	UFUNCTION(BlueprintNativeEvent)
	void OnGetCollected();

	virtual void OnGetCollected_Implementation();


protected:
	//True or False of the status
	bool bIsActive;

private:
	//The static mesh of the pickup, to give it visual looking, you can replace it with Sprites if it is a 2d game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ThePickupStaticMesh;

	
	
};
