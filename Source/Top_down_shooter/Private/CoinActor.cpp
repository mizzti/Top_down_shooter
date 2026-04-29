// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinActor.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" // 必须包含这个才能用 GetPlayerPawn
#include "GameFramework/Pawn.h"      // 必须包含这个才能识别 APawn 类型

// Sets default values
ACoinActor::ACoinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	// 创建组件时机：构造函数
	// 碰撞组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(40.0f);
	
	// 单独设置重叠响应
	// 设置对象类型为 WorldDynamic
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	// 对所有通道默认 Ignore
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 单独对 Pawn 通道设置 Overlap
	CollisionComp->SetCollisionResponseToChannel(
		ECC_GameTraceChannel1,
		ECR_Overlap
	);
	
	SetRootComponent(CollisionComp);
	
	// 旋转组件
	RotationTriggerComp = CreateDefaultSubobject<USphereComponent>(TEXT("RotationTriggerComp"));
	RotationTriggerComp->SetSphereRadius(150.0f);
	// 单独设置重叠响应通道
	RotationTriggerComp->SetCollisionObjectType(ECC_WorldDynamic);
	RotationTriggerComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	RotationTriggerComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	
	RotationTriggerComp->SetupAttachment(CollisionComp);
	
	// mesh组件
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 取消碰撞体积
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(CollisionComp);
	
	// 默认金币价值
	CoinValue = 1;
}

// Called when the game starts or when spawned
void ACoinActor::BeginPlay()
{
	Super::BeginPlay(); // 必须调用父类，让引擎完成基础初始
	
	// 随机初始朝向（只随机 Yaw 轴，绕Z轴转）
	float RandomYaw = FMath::RandRange(0.0f, 360.0f);
	SetActorRotation(FRotator(0.0f, RandomYaw, 0.0f));
	
	if (RotationTriggerComp)
	{
		RotationTriggerComp->OnComponentBeginOverlap.AddDynamic(this, &ACoinActor::OnOverlapBegin);
		RotationTriggerComp->OnComponentEndOverlap.AddDynamic(this, &ACoinActor::OnOverlapEnd);
	}
}

void ACoinActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UE_LOG(LogTemp, Warning, TEXT("CoinActor EndPlay, Reason: %d"), EndPlayReason);
}

// Called every frame
void ACoinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}

void ACoinActor::OnPickup_Implementation(AActor* Picker)
{
	if (Picker)
	{
		UE_LOG(LogTemp, Warning, TEXT("CoinActor: %s OnPickup函数被调用, value = %d"), *Picker->GetName(), CoinValue);
		Destroy();// 拾取后被销毁
	}
}

void ACoinActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<APawn>(OtherActor))
	{
		SetActorTickEnabled(true); // 玩家进入范围开启Tick
		UE_LOG(LogTemp, Warning, TEXT("[CoinActor] 玩家靠近，开始旋转"));
	}
}

void ACoinActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && Cast<APawn>(OtherActor))
	{
		SetActorTickEnabled(false);
		UE_LOG(LogTemp, Warning, TEXT("[CoinActor] 玩家离开，停止旋转"));
	}
}

