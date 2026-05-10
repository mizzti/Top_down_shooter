// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

// 前向声明（减少编译依赖）
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class TOP_DOWN_SHOOTER_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 碰撞组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;
	// 视觉网格组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	// 子弹移动轨迹的组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float Damage = 1.0f;
	
	// 子弹移动速度（cm/s），修改后会同步到 ProjectileComp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float BulletSpeed = 2000.0f;
	
	// 子弹生命周期
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float LifeSpan = 3.0f;
	
	// 碰撞函数
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);
};
