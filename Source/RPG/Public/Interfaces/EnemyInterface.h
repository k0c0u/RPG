
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"


UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};



 
class RPG_API IEnemyInterface
{
	GENERATED_BODY()

	
public:

	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
