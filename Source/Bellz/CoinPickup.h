// All rights reserved, Muhammad A.Moniem 2016 http:

#pragma once

#include "PickupBase.h"
#include "CoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class BELLZ_API ACoinPickup : public APickupBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACoinPickup();

	// The Override for the Virtual of the base class
	void OnGetCollected_Implementation() override;

	//Access the Coin value
	float GetCoinValue();

protected:
	//The value that the coin adds to the collected items, you can make a Special coins with higher values or so...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins", Meta = (BlueprintProtected = "true"))
	float CoinValue;
	
	
	
};
