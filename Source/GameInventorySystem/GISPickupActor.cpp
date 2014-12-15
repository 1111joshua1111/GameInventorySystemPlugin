// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "GameInventorySystem.h"
#include "GISInventoryBaseComponent.h"

#include "Widgets/GISLootContainerBaseWidget.h"

#include "GISItemData.h"

#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

#include "GISPickupActor.h"

AGISPickupActor::AGISPickupActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
}

void AGISPickupActor::BeginPlay()
{
	for (TSubclassOf<UGISItemData> Item : ItemToLootClass)
	{
		UGISItemData* itemData = ConstructObject<UGISItemData>(Item, this);
		ItemToLoot.Add(itemData);
	}
}
void AGISPickupActor::OpenLootWindow()
{
	//if (LootWidget)
	//{
	//	ULocalPlayer* Player = GetWorld()->GetFirstLocalPlayerFromController(); //temporary
	//	LootWidget->SetPlayerContext(FLocalPlayerContext(Player)); //temporary
	//	LootWidget->Initialize();
	//	int32 ItemCount = ItemToLoot.Num();
	//	TArray<FGISLootSlotInfo> LootSlotInfos;
	//	for (int32 Index = 0; Index < ItemCount; Index++)
	//	{
	//		FGISLootSlotInfo LootInfo;
	//		LootInfo.SlotIndex = Index;
	//		LootInfo.SlotData = ItemToLoot[Index];
	//		LootSlotInfos.Add(LootInfo);
	//	}
	//	LootWidget->ItemsInfos = LootSlotInfos;
	//	LootWidget->OwningPickupActor = this;
	//	LootWidget->InitializeLootWidget();
	//	LootWidget->AddToViewport();
	//}

}
void AGISPickupActor::CloseLootWindow()
{

}
void AGISPickupActor::LootAllItems()
{
	//if (InteractingInventory.IsValid())
	//{
	//	InteractingInventory->LootAllItems(this);
	//}
}

void AGISPickupActor::LootSingleItem(int32 ItemIndexIn)
{
	//if (InteractingInventory.IsValid())
	//{
	//	InteractingInventory->LootOneItem(ItemIndexIn, this);
	//}
}
void AGISPickupActor::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGISPickupActor, ItemToLootClass);
}
bool AGISPickupActor::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const UGISItemData* SlotItem : ItemToLoot)
	{
		if (SlotItem)
		{
			WroteSomething |= Channel->ReplicateSubobject(const_cast<UGISItemData*>(SlotItem), *Bunch, *RepFlags);
		}
	}
	return WroteSomething;
}
void AGISPickupActor::GetSubobjectsWithStableNamesForNetworking(TArray<UObject*>& Objs)
{
	for (const UGISItemData* SlotItem : ItemToLoot)
		{
			if (SlotItem)
			{
				Objs.Add(const_cast<UGISItemData*>(SlotItem));
			}
		}
}

/** Called on the actor when a new subobject is dynamically created via replication */
void AGISPickupActor::OnSubobjectCreatedFromReplication(UObject *NewSubobject)
{
	Super::OnSubobjectCreatedFromReplication(NewSubobject);

	UGISItemData* newItemData = Cast<UGISItemData>(NewSubobject);
	if (newItemData)
	{
		ItemToLoot.Add(newItemData);
	}
}

/** Called on the actor when a new subobject is dynamically created via replication */
void AGISPickupActor::OnSubobjectDestroyFromReplication(UObject *NewSubobject)
{
	Super::OnSubobjectDestroyFromReplication(NewSubobject);
}