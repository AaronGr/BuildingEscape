// Copyright Aaron Gravelle

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.generated.h"


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
	void FindPhysicsHandleComponent();

	// Return hit for first physics body in reach 
	FHitResult GetFirstPhysicsBodyInReach() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// How far ahead the player can reach in cm
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	APlayerController* Player = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* Input = nullptr;

	/// Ray-cast and grab what is in reach
	void Grab();
	void Release();
};
