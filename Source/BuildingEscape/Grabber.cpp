// Copyright Aaron Gravelle

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector PlayerPOVPosition;
	FRotator PlayerPOVRotation;
	Player->GetPlayerViewPoint(OUT PlayerPOVPosition, OUT PlayerPOVRotation);
	/*UE_LOG(LogTemp, Warning, TEXT("View point position: %s.  View point rotation: %s"), 
		*PlayerPOVPosition.ToString(),
		*PlayerPOVRotation.ToString())*/

	FVector LineTraceEnd =  PlayerPOVPosition + (Reach * PlayerPOVRotation.Vector());

	// Draw a red trace in the world to visualise
	DrawDebugLine(
		GetWorld(),
		PlayerPOVPosition,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	// Ray-cast to reach distance

	// See what we hit
}

