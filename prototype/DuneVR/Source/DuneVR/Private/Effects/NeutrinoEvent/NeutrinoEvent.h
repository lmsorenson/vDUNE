#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeutrinoTrack.h"
#include "NeutrinoEvent.generated.h"

UCLASS()
class ANeutrinoEvent : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ANeutrinoEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
    TSubclassOf<ANeutrinoTrack> track_bp_;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    TArray<class ANeutrinoTrack*> track_list_;
};
