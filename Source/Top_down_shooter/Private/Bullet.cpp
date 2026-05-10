// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 碰撞组件
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(20.0f);// 子弹碰撞半径（cm）
	// 设置预设响应
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetRootComponent(CollisionComp);
	// 开启刚体碰撞事件通知
	CollisionComp->SetNotifyRigidBodyCollision(true);
	
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
	// UE_LOG(LogTemp, Log, TEXT("同步前的Speed=%.1f, InitialSpeed=%.1f"), BulletSpeed, ProjectileComp->InitialSpeed);
	// 必须在 Super::BeginPlay() 之前同步速度
	if (ProjectileComp)
	{
		ProjectileComp->InitialSpeed = BulletSpeed;
		ProjectileComp->MaxSpeed = BulletSpeed;
	}
	// UE_LOG(LogTemp, Log, TEXT("同步后的Speed=%.1f, InitialSpeed=%.1f"), BulletSpeed, ProjectileComp->InitialSpeed);
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	
	if (CollisionComp)
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit); // 绑定回调函数
		if (GetInstigator())
		{
			CollisionComp->IgnoreActorWhenMoving(GetInstigator(), true);
		}
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("[Bullet] 生成，Damage=%.1f, Speed=%.1f, LifeSpan=%.1f, InitialSpeed=%.1f"), Damage, BulletSpeed, LifeSpan, ProjectileComp->InitialSpeed);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// 安全检查
	if (!OtherActor || OtherActor == this || OtherActor == GetInstigator()) // 检查碰撞到的Actor
	{
		return;
	}
	if (Cast<ACharacter>(OtherActor)) // 只有碰撞到敌人才触发applyDamage
	{
		// 输出伤害和碰撞的对象
		UE_LOG(LogTemp, Log, TEXT("[Bullet] 碰撞对象：%s, damage: %.1f"), *OtherActor->GetName(), Damage);
		// 施加伤害，伤害来源可实名填ACharacter
		UGameplayStatics::ApplyDamage(
			OtherActor, 
			Damage, 
			GetInstigator() ? GetInstigator()->GetController() : nullptr, 
			this, 
			UDamageType::StaticClass()
			);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[Bullet] 击中障碍物 %s，无伤害"), *OtherActor->GetName());
	}
	// 只要碰撞就删除
	Destroy();
}

