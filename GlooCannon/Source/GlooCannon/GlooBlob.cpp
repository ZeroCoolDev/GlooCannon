// Fill out your copyright notice in the Description page of Project Settings.


#include "GlooBlob.h"

// Sets default values
AGlooBlob::AGlooBlob()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Block);
	BoxCollision->SetGenerateOverlapEvents(true);
	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void AGlooBlob::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGlooBlob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

