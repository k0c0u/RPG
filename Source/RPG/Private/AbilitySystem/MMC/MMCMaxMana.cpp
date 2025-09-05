// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMCMaxMana.h"

#include "AbilitySystem/AttributeSet/BaseAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMCMaxMana::UMMCMaxMana()
{
	IntelligenceDef.AttributeToCapture = UBaseAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParams, Intelligence);
	Intelligence = FMath::Max(Intelligence, 0.f);

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()))
	{
		const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
		return 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
	}
	return -1.f;
}
