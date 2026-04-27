// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CoinActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TOP_DOWN_SHOOTER_API ACoinActor : public AActor // ue自动加上前缀
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoinActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// 创建碰撞组件(作为RootCompnent)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;
	// 旋转速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	float RotationSpeed = 90.0f;
	// 旋转距离设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	float ActivateDistance = 300.0f;
	// 创建网格组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 CoinValue;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Coin")
	void OnPickup(AActor* Picker); // 蓝图使用
	
	virtual void OnPickup_Implementation(AActor* Picker); // 代码使用
	
private:
	// 存储旋转信息
	UPROPERTY()
	APawn* PlayerPawn = nullptr;
};
