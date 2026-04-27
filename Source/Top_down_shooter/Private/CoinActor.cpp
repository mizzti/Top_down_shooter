// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinActor.h"
#include "Kismet/GameplayStatics.h" // 必须包含这个才能用 GetPlayerPawn
#include "GameFramework/Pawn.h"      // 必须包含这个才能识别 APawn 类型

// Sets default values
ACoinActor::ACoinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 创建组件时机：构造函数
	// 碰撞组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(40.0f);
	// 设置为重叠模式，消除碰撞体积
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(CollisionComp);
	
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
	Super::BeginPlay(); // 必须调用父类，让引擎完成基础初始化
	
	UE_LOG(LogTemp, Warning, TEXT("[CoinActor] BeginPlay - RotationSpeed: %.1f"), RotationSpeed);
	UE_LOG(LogTemp, Warning, TEXT("CoinValue: %d"), CoinValue);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
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
	
	// 玩家不存在就返回，防止空指针
	if (!PlayerPawn)
	{
		return;
	}
	float Distance = GetDistanceTo(PlayerPawn);
	UE_LOG(LogTemp, Warning, TEXT("玩家与金币的距离: %f"), Distance);
	
	if (Distance <= ActivateDistance)
	{
		UE_LOG(LogTemp, Warning, TEXT("进入旋转范围"));
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}

void ACoinActor::OnPickup_Implementation(AActor* Picker)
{
	if (Picker)
	{
		UE_LOG(LogTemp, Warning, TEXT("CoinActor: %s OnPickup函数被调用, value = %d"), *Picker->GetName(), CoinValue);
	}
	
	Destroy();// 拾取后被销毁
}

