// Copyright Aaron Gravelle

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	bDoorIsOpen = false;
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor() {
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	bDoorIsOpen = true;
}

void UOpenDoor::CloseDoor() {
	Owner->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	bDoorIsOpen = false;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float WorldTime = GetWorld()->GetTimeSeconds();



	// Poll the trigger every frame
	if (PressurePlate->IsOverlappingActor(ActorThatOpens) && !bDoorIsOpen) {
		// If the ActorThatOpens is in the volume
		OpenDoor();
		LastDoorOpenTime = WorldTime;		
	}

	// Check if it's time to close the door
	if (WorldTime - LastDoorOpenTime >= DoorCloseDelay && bDoorIsOpen) {
		CloseDoor();
	}
	
}

