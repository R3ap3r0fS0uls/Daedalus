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

void LogOther(int32_t value)
{
	Log::Print(std::to_string(value));
}

// Only Called Once, if you need to hook shit, declare some global non changing values
void ExampleMod::InitializeMod()
{

	UE4::InitSDK();
	SetupHooks();

	REGISTER_FUNCTION(WriteToFile);

	MinHook::Init(); //Uncomment if you plan to do hooks

	//UseMenuButton = true; // Allows Mod Loader To Show Button
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

void ExampleMod::ProcessFunction(UE4::UObject* pCallObject, UE4::UFunction* pUFunc)
{
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

void ExampleMod::BeginPlay(UE4::AActor* Actor)
{
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
