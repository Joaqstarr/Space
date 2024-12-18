// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerShip.h"
#include "Components/WidgetComponent.h"
#include "Components/Player/PawnMotionWarpingComponent.h"
#include "Components/Player/TargetingHandlerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Weapons/Sword.h"

APlayerShip::APlayerShip() : AShip()
{
	TargetingManager = CreateDefaultSubobject<UTargetingHandlerComponent>(FName("TargetingHandler"));

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("AiStimuliSource"));

	TargetLockIndicator = CreateDefaultSubobject<UWidgetComponent>(FName("TargetLockIndicator"));
	TargetLockIndicator->SetWidgetSpace(EWidgetSpace::Screen);
	TargetLockIndicator->SetVisibility(false);
	TargetLockIndicator->SetDrawSize(FVector2D(30,30));
	TargetLockIndicator->SetupAttachment(ShipMeshComponent);

	PawnMotionWarpingComponent = CreateDefaultSubobject<UPawnMotionWarpingComponent>(FName("PawnMotionWarpingComponent"));
	//MotionWarpingComponent->InitializeComponent();
	
	bAlwaysRelevant = false;
}

FVector2D APlayerShip::GetMousePos(bool affectedByDeadzone) const
	{
	FVector2D mousePosNorm {MousePos/MouseRadius};

	if(affectedByDeadzone)
	{
		if(mousePosNorm.Length() < MouseDeadzone) return FVector2D::Zero();
		mousePosNorm.X = (mousePosNorm.X - MouseDeadzone)/(1 - MouseDeadzone);
		mousePosNorm.Y = (mousePosNorm.Y - MouseDeadzone)/(1 - MouseDeadzone);
	}
	return mousePosNorm;
}

ATargetable* APlayerShip::GetCurrentTarget()
{
	if(!TargetingManager)return nullptr;
	return TargetingManager->GetCurrentTarget();
}

UPawnMotionWarpingComponent* APlayerShip::GetMotionWarping()
{
	return PawnMotionWarpingComponent;
}




void APlayerShip::SetupSwordActor()
{
	if(SwordClass)
	{
		SwordActor = GetWorld()->SpawnActor<ASword>(SwordClass);
		SwordActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WingBladeSocket"));
		SwordActor->SetOwner(this);
		SwordActor->SetInstigator(this);
	}
}



void APlayerShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	TargetLockIndicator->SetWidget(TargetLockWidget);


	SetupSwordActor();
}

FVector2D APlayerShip::UpdateMousePos(const FVector2D& delta)
{
	MousePos.X += delta.X;
	MousePos.Y += delta.Y;

	if(MousePos.Length() > MouseRadius)
	{
		MousePos.Normalize();
		MousePos = MousePos * MouseRadius;
	}
	return MousePos;
}

FVector2D APlayerShip::UpdateStickLook(const FVector2D& stickLookInput)
{
	MousePos.X = MouseRadius * stickLookInput.X;
	MousePos.Y = MouseRadius * stickLookInput.Y;

	return MousePos;
}

void APlayerShip::ApplyNetworkedInput(const FShipInputState& input)
{
	if (IsLocallyControlled())
	{
		ApplyInput(input);

		if (!HasAuthority())
		{
			ServerSendInput(input);
		}
	}
}
void APlayerShip::ApplyInput(const FShipInputState& inputState)
{
	AddRoll(inputState.RollInput);
	AddPitch(inputState.PitchInput);
	AddYaw(inputState.YawInput);

	AddThrust(inputState.ForwardThrust, inputState.SidewaysThrust, inputState.VerticalThrust);
}

void APlayerShip::ServerSendInput_Implementation(const FShipInputState& inputState)
{
	ApplyInput(inputState);
}





