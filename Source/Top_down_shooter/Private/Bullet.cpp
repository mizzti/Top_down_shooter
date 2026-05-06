// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 碰撞组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(20.0f);// 子弹碰撞半径（cm）
	// 设置预设响应
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetRootComponent(CollisionComp);
	
	// Mesh组件
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 创建投射物移动组件
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	
	// 飞行速度
	ProjectileComp->InitialSpeed = BulletSpeed;
	ProjectileComp->MaxSpeed = BulletSpeed;
	// 子弹头部朝向飞行方向
	ProjectileComp->bRotationFollowsVelocity = false; // 球形可改为false
	// 不受重力（直线飞行）
	ProjectileComp->ProjectileGravityScale = 0.0f;
	// 碰撞到物体是否反射
	ProjectileComp->bShouldBounce = false;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("同步前的Speed=%.1f, InitialSpeed=%.1f"), BulletSpeed, ProjectileComp->InitialSpeed);
	// 必须在 Super::BeginPlay() 之前同步速度
	if (ProjectileComp)
	{
		ProjectileComp->InitialSpeed = BulletSpeed;
		ProjectileComp->MaxSpeed = BulletSpeed;
	}
	UE_LOG(LogTemp, Log, TEXT("同步后的Speed=%.1f, InitialSpeed=%.1f"), BulletSpeed, ProjectileComp->InitialSpeed);
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);

	UE_LOG(LogTemp, Warning, TEXT("[Bullet] 生成，Damage=%.1f, Speed=%.1f, LifeSpan=%.1f, InitialSpeed=%.1f"), Damage, BulletSpeed, LifeSpan, ProjectileComp->InitialSpeed);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

