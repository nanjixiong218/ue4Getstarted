// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GETSTARTED_API UMainPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	class AMainPlayer* PlayerRef;

	UFUNCTION(BlueprintCallable, Category="Animation Properties")
	void UpdateAnimationProperties();

	// 动画蓝图初始化时调用，类似beginPlay
	virtual void NativeInitializeAnimation() override;
	
};
