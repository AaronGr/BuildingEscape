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

	SetupInputComponent();
	SetupPhysicsHandleComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PhysicsHandle) {return;}
	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetEndOfReach());
	}
}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		/// Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing InputComponent"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupPhysicsHandleComponent() {
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandleComponent"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab() {
	/// LINE TRACE and see if any actors with physics body collision channel set
	auto ComponentToGrab = GetFirstPhysicsBodyInReach().GetComponent();

	/// If we hit something then attach a physics handle
	if (ComponentToGrab) {
		if (!PhysicsHandle) { return; }
		//  attach physics handle
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
	}
}

void UGrabber::Release() {
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	/// Setup query parameter
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast to reach distance
	FHitResult PhysicsBody;
	GetWorld()->LineTraceSingleByObjectType(
		OUT PhysicsBody,
		GetOwner()->GetActorLocation(),
		GetEndOfReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return PhysicsBody;
}

FVector UGrabber::GetEndOfReach() const
{
	PlayerPOV CurrentPOV = GetPlayerPOV();
	return CurrentPOV.Location + (Reach * CurrentPOV.Rotation.Vector());
}

PlayerPOV UGrabber::GetPlayerPOV() const
{
	PlayerPOV CurrentPOV;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT CurrentPOV.Location, OUT CurrentPOV.Rotation);
	return CurrentPOV;
}