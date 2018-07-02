// All rights reserved, Muhammad A.Moniem 2016 http:

#include "Bellz.h"
#include "CoinPickup.h"


//Set default values
ACoinPickup::ACoinPickup()
{
	//the base value the coin
	CoinValue = 20.0f;
}


void ACoinPickup::OnGetCollected_Implementation()
{
	// Use the base pickup behavior
	Super::OnGetCollected_Implementation();
	// Destroy the coin
	Destroy();
}

// return the value of the coin
float ACoinPickup::GetCoinValue()
{
	return CoinValue;
}

