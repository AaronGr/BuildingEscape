// Copyright Aaron Gravelle

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define OUT


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

	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("This component is missing its owner"))
	}
	
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing its pressure plate component"), *(Owner->GetName()))
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	float WorldTime = GetWorld()->GetTimeSeconds();


	// Poll the trigger every frame
	if (GetTotalMassOfActorsOnPlate() > MassToOpen) {
		// If the ActorThatOpens is in the volume
		OpenDoor();
		LastDoorOpenTime = WorldTime;
	}

	// Check if it's time to close the door
	if (WorldTime - LastDoorOpenTime >= DoorCloseDelay && bDoorIsOpen) {
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor() {
	if (!Owner) {return;}
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	bDoorIsOpen = true;
}

void UOpenDoor::CloseDoor() {
	if (!Owner) { return; }
	Owner->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	bDoorIsOpen = false;
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() 
{
	float TotalMass = 0.f;
	if (!PressurePlate) { return 0; }
	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	// Iterate through them adding their masses

	return TotalMass;
}