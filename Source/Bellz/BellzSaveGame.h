// All rights reserved, Muhammad A.Moniem 2016 http:

#pragma once

#include "GameFramework/SaveGame.h"
#include "BellzSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BELLZ_API UBellzSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = SavedValues)
		FString SlotName;

	UPROPERTY(VisibleAnywhere, Category = SavedValues)
		uint32 PlayerIndex;

	UPROPERTY(VisibleAnywhere, Category = SavedValues)
		uint32 CollectedCoins;

	UPROPERTY(VisibleAnywhere, Category = SavedValues)
		float PlayerHealth;

	UPROPERTY(VisibleAnywhere, Category = SavedValues)
		uint32 lastUsedWeaponIndex;

	UPROPERTY(VisibleAnywhere, Category = SavedValues)
		FString PlayerName;


	UBellzSaveGame();
	
	
};
