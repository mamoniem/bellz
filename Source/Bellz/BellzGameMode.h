// All rights reserved, Muhammad A.Moniem 2016 http://www.mamoniem.com/

#pragma once
#include "GameFramework/GameMode.h"
#include "BellzGameMode.generated.h"



UCLASS(minimalapi)
class ABellzGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABellzGameMode();

	virtual void BeginPlay() override;

protected:
	//The game UI widget blueprint that been designed in UMG editor
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (BleprintProtected = "true"))
	TSubclassOf<class UUserWidget> GameUIWidget;

	//The in game instance of the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (BleprintProtected = "true"))
	class UUserWidget* GameUIInstance;
};
