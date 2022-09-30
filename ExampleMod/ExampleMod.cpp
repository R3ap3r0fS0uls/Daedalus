#include "ExampleMod.h"
#include "SDK.hpp"
#include "Utilities/MinHook.h"
#include "Ue4.hpp"
#include <filesystem>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <shlobj_core.h>
#include <direct.h>
#include <json/json.h>

BPFUNCTION(WriteToFile)
{
	std::cout << "WriteToFile" << std::endl;
	struct InputParams
	{
		UE4::FString NameTest;
	};
	auto Inputs = stack->GetInputParams<InputParams>();
	stack->SetOutput<UE4::FString>("OutPutString", L"KboyGang");
	stack->SetOutput<bool>("ReturnValue", true);
}


// Only Called Once, if you need to hook shit, declare some global non changing values
void ExampleMod::InitializeMod()
{

	UE4::InitSDK();
	SetupHooks();

	REGISTER_FUNCTION(WriteToFile);

	int32_t x = 0;

	MinHook::Init(); //Uncomment if you plan to do hooks

	UseMenuButton = true; // Allows Mod Loader To Show Button
}

void ExampleMod::InitGameState()
{
}

template<typename T>
static T* FindObject(const std::string& name)
{
	for (int i = 0; i < UE4::UObject::GObjects->GetAsChunckArray().Num(); ++i)
	{
		auto object = UE4::UObject::GObjects->GetAsChunckArray().GetByIndex(i).Object;

		if (object == nullptr)
		{
			continue;
		}

		if (object->GetFullName() == name)
		{
			return static_cast<T*>(object);
		}
	}
	return nullptr;
}

Json::Value metaItem(std::string x, int32_t id)
{
	Json::Value result;
	result["ItemStaticRow"] = x;
	result["ID"] = id;
	return result;
}

UE4::UObject* DropShip;

void ExampleMod::ProcessFunction(UE4::UObject* pCallObject, UE4::UFunction* pUFunc)
{
	if (pCallObject->GetClass()->GetFullName() == "BlueprintGeneratedClass BP_DropShip.BP_DropShip_C" && pUFunc->GetFullName() == "Function BP_DropShip.BP_DropShip_C.OnProspectSessionEnded")
	{
		UE4::UFunction* GetInventoryIds = FindObject<UE4::UFunction>("Function Icarus.InventoryComponent.GetInventoryIds");
		UE4::UFunction* GetInventory = FindObject<UE4::UFunction>("Function Icarus.InventoryComponent.GetInventory");

		UE4::UObject* Inventory = *(UE4::UObject**)(DropShip + 0x338);

		struct
		{
			struct UE4::TArray<struct FInventoryIDEnum> ReturnValue;
		}GetInventoryIdsParams;

		struct
		{
			struct FInventoryIDEnum InventoryID;
			UE4::UObject* ReturnValue;
		}GetInventoryParams;

		Inventory->ProcessEvent(GetInventoryIds, &GetInventoryIdsParams);

		GetInventoryParams.InventoryID = GetInventoryIdsParams.ReturnValue[0];

		Inventory->ProcessEvent(GetInventory, &GetInventoryParams);

		UE4::UObject* UInventory = GetInventoryParams.ReturnValue;

		UE4::UObject* FInventorySlotsFastArray = (UE4::UObject*)(UInventory + 0xe8);

		struct UE4::TArray<FInventorySlot> Slots = *(struct UE4::TArray<FInventorySlot>*)(FInventorySlotsFastArray + 0x108);

		PWSTR appdata = NULL;
		if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &appdata) == S_OK) {
			char dest[MAX_PATH];
			wcstombs(dest, appdata, MAX_PATH);

			std::string mainPath = dest;
			std::string restPath = "\\Icarus\\Saved\\Offline\\Loadout\\Slot_0.json";

			std::string finalPath = mainPath.append(restPath);

			std::ifstream file(finalPath);

			Json::Value completeJson;

			Json::Reader reader;

			reader.parse(file, completeJson);

			std::ofstream myFile(finalPath);;

			int32_t Id = 0;

			completeJson["MetaItems"].clear();

			for (int32_t i = 0; i < Slots.Count; i++)
			{
				if (Slots[i].ItemData.ItemStaticData.RowName.GetName() != "None")
				{
					if (Slots[i].ItemData.ItemDynamicData.Count > 0)
					{

						for (int32_t i2 = 0; i2 < Slots[i].ItemData.ItemDynamicData.Count; i2++)
						{
							if (Slots[i].ItemData.ItemDynamicData[i2].PropertyType == EDynamicItemProperties::ItemableStack)
							{
								for (int32_t i3 = 0; i3 < Slots[i].ItemData.ItemDynamicData[i2].Value; i3++)
								{
									completeJson["MetaItems"].append(metaItem(Slots[i].ItemData.ItemStaticData.RowName.GetName(), Id));

									Id++;
								}
							}
						}
					}
					else
					{
						completeJson["MetaItems"].append(metaItem(Slots[i].ItemData.ItemStaticData.RowName.GetName(), Id));

						Id++;
					}
				}
			}

			Json::StreamWriterBuilder builder;
			builder["indentation"] = "";
			const std::string output = Json::writeString(builder, completeJson);

			myFile << output << std::endl;

			file.close();
			myFile.close();
		}
		else
		{
			Log::Error("Loadout file not found.");
		}
	}
}

static UE4::APlayerController* GetPlayerController() 
{
	static auto fn = FindObject<UE4::UFunction>("Function Engine.GameplayStatics.GetPlayerController");
	auto GameplayStatics = FindObject<UE4::UGameplayStatics>("Class Engine.GameplayStatics");

	struct
	{
		class UE4::UObject* WorldContextObject;
		int PlayerIndex;
		class UE4::APlayerController* ReturnValue;
	}params;

	params.WorldContextObject = UE4::UWorld::GetWorld();
	params.PlayerIndex = 0;

	GameplayStatics->ProcessEvent(fn, &params);

	return params.ReturnValue;
}

void LogOther(int32_t value)
{
	Log::Print(std::to_string(value));
}

void ExampleMod::BeginPlay(UE4::AActor* Actor)
{
	if (Actor->GetClass()->GetFullName() == "BlueprintGeneratedClass BP_DropShip.BP_DropShip_C")
	{
		DropShip = Actor;
	}
}

void ExampleMod::PostBeginPlay(std::wstring ModActorName, UE4::AActor* Actor)
{
	// Filters Out All Mod Actors Not Related To Your Mod
	std::wstring TmpModName(ModName.begin(), ModName.end());
	if (ModActorName == TmpModName)
	{
		//Sets ModActor Ref
		ModActor = Actor;
	}
}
void ExampleMod::DX11Present(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView)
{
}

void ExampleMod::OnModMenuButtonPressed()
{
}

void ExampleMod::DrawImGui()
{

}
