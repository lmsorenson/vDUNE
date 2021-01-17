#include "../../Public/Neutrino/NeutrinoEvent.h"
#include "../../Public/Neutrino/NeutrinoTrack.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "JsonObjectConverter.h"
#include <cmath>

#include "NeutrinoEventStruct.h"

// Sets default values
ANeutrinoEvent::ANeutrinoEvent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>( TEXT("RootComponent") );
    RootComponent = SphereComponent;
    SphereComponent->InitSphereRadius( 40.0f );
}

// Called when the game starts or when spawned
void ANeutrinoEvent::BeginPlay()
{
	Super::BeginPlay();
}

void ANeutrinoEvent::PreInitializeComponents()
{
    Super::PreInitializeComponents();

}

void ANeutrinoEvent::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    FString FilePath = "/Users/lucassorenson/Code/dune/Subversion/trunk/prototype/DuneVR/Content/information.json";
    FString FileData = "";
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("DID NOT FIND FILE"));
        return;
    }
    FNeutrinoEventList events;
    FFileHelper::LoadFileToString(FileData, *FilePath);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FileData);

    if (FJsonObjectConverter::JsonObjectStringToUStruct(FileData, &events, 0, 0))
    {
        auto x_range = events.EventList[4].Metadata.XAxisMaximum - events.EventList[4].Metadata.XAxisMinimum;
        auto y_range = events.EventList[4].Metadata.YAxisMaximum - events.EventList[4].Metadata.YAxisMinimum;
        auto z_range = events.EventList[4].Metadata.ZAxisMaximum - events.EventList[4].Metadata.ZAxisMinimum;
        auto x_center = events.EventList[4].Metadata.XAxisMinimum + x_range/2;
        auto y_center = events.EventList[4].Metadata.YAxisMinimum + y_range/2;
        auto z_center = events.EventList[4].Metadata.ZAxisMinimum + z_range/2;

        UBoxComponent* event_bounds = NewObject<UBoxComponent>( this );
        event_bounds->SetRelativeLocation(FVector(x_center, y_center, z_center));
        event_bounds->InitBoxExtent(FVector(x_range/2, y_range/2, z_range/2));
        event_bounds->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        event_bounds->RegisterComponent();

        int i = 0;
        for(auto &track : events.EventList[4].Tracks)
        {
            auto track_x_range = track.Metadata.XAxisMaximum - track.Metadata.XAxisMinimum;
            auto track_y_range = track.Metadata.YAxisMaximum - track.Metadata.YAxisMinimum;
            auto track_z_range = track.Metadata.ZAxisMaximum - track.Metadata.ZAxisMinimum;
            auto track_x_center = track.Metadata.XAxisMinimum + track_x_range/2;
            auto track_y_center = track.Metadata.YAxisMinimum + track_y_range/2;
            auto track_z_center = track.Metadata.ZAxisMinimum + track_z_range/2;

            auto new_track = GetWorld()->SpawnActor<ANeutrinoTrack>(track_bp_.Get(), GetActorLocation(), GetActorRotation());
//            new_track->add_bounds(FVector(track_x_center, track_y_center, track_z_center), FVector(track_x_range, track_y_center, track_z_center));
            new_track->add_points(track.Points);
            new_track->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            track_list_.Add(new_track);
            i++;
        }
    }
    else
    {
        UE_LOG(LogClass, Error, TEXT("Failed to convert!"));
    }
}

// Called every frame
void ANeutrinoEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

