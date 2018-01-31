// Copyright Aaron Gravelle

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.generated.h"

struct PlayerPOV {
	FVector Location;
	FRotator Rotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Find (assumed) attached input component
	void SetupInputComponent();

	// Setup (assumed) attached physics component
	void SetupPhysicsHandleComponent();

	// Return hit for first physics body in reach 
	FHitResult GetFirstPhysicsBodyInReach() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

private:

	// How far ahead the player can reach in cm
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	/// Ray-cast and grab what is in reach
	void Grab();
	void Release();
	PlayerPOV GetPlayerPOV() const;
	FVector GetEndOfReach() const;
};
