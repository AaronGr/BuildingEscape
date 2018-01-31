// Copyright Aaron Gravelle

#include "Grabber.h"


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
	
	FindPhysicsHandleComponent();

	SetupInputComponent();

}

void UGrabber::SetupInputComponent() {
	if (Input) {
		UE_LOG(LogTemp, Warning, TEXT("%s has an InputComponent"), *(GetOwner()->GetName()));
		/// Bind the input action
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing InputComponent"), *(GetOwner()->GetName()));
	}
}

void UGrabber::FindPhysicsHandleComponent() {
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PhysicsHandle) {
		/// Physics handle is found
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandleComponent"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player view point this tick
	FVector PlayerPOVPosition;
	FRotator PlayerPOVRotation;
	Player->GetPlayerViewPoint(OUT PlayerPOVPosition, OUT PlayerPOVRotation);

	FVector LineTraceEnd = PlayerPOVPosition + (Reach * PlayerPOVRotation.Vector());

	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {

		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))	

	/// LINE TRACE and see if any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit) {
		//  attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"))
		// TODO release physics handle
		PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	/// Get player view point this tick
	FVector PlayerPOVPosition;
	FRotator PlayerPOVRotation;
	Player->GetPlayerViewPoint(OUT PlayerPOVPosition, OUT PlayerPOVRotation);

	FVector LineTraceEnd = PlayerPOVPosition + (Reach * PlayerPOVRotation.Vector());

	/// Draw a red trace in the world to visualise
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

	/// Setup query parameter
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerPOVPosition,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Object hit: %s"), *(ActorHit->GetName()))
	}

	return Hit;
}