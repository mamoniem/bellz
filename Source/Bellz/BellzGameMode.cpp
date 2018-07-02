// All rights reserved, Muhammad A.Moniem 2016 http://www.mamoniem.com/

#include "Bellz.h"
#include "BellzGameMode.h"
#include "Gladiator.h"
#include "Blueprint/UserWidget.h"

ABellzGameMode::ABellzGameMode()
{
	//set the gladiator based blueprint as the default pawn class of the gamemode. Also we can assign it directly within the editor
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/GladiatorCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABellzGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GameUIWidget != nullptr)
	{
		GameUIInstance = CreateWidget<UUserWidget>(GetWorld(), GameUIWidget);
		if (GameUIInstance != nullptr)
		{
			GameUIInstance->AddToViewport();
		}
	}
}
