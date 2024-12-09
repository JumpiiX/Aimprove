// Fill out your copyright notice in the Description page of Project Settings.


#include "TopdownCameraPawn.h"

// Sets default values
ATopdownCameraPawn::ATopdownCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATopdownCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATopdownCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATopdownCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

