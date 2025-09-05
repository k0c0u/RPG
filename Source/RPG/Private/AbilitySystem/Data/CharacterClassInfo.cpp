// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassInfo(const ECharacterClass CharacterClass)
{
	return CharactersClassInformation.FindChecked(CharacterClass);
}
