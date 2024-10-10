// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGrapplePlayerCharacter() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	UE_GRAPPLE_API UClass* Z_Construct_UClass_AGrapplePlayerCharacter();
	UE_GRAPPLE_API UClass* Z_Construct_UClass_AGrapplePlayerCharacter_NoRegister();
	UPackage* Z_Construct_UPackage__Script_UE_Grapple();
// End Cross Module References
	void AGrapplePlayerCharacter::StaticRegisterNativesAGrapplePlayerCharacter()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AGrapplePlayerCharacter);
	UClass* Z_Construct_UClass_AGrapplePlayerCharacter_NoRegister()
	{
		return AGrapplePlayerCharacter::StaticClass();
	}
	struct Z_Construct_UClass_AGrapplePlayerCharacter_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGrapplePlayerCharacter_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_UE_Grapple,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AGrapplePlayerCharacter_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGrapplePlayerCharacter_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "PlayerAndGM/GrapplePlayerCharacter.h" },
		{ "ModuleRelativePath", "Public/PlayerAndGM/GrapplePlayerCharacter.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGrapplePlayerCharacter_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGrapplePlayerCharacter>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AGrapplePlayerCharacter_Statics::ClassParams = {
		&AGrapplePlayerCharacter::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AGrapplePlayerCharacter_Statics::Class_MetaDataParams), Z_Construct_UClass_AGrapplePlayerCharacter_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_AGrapplePlayerCharacter()
	{
		if (!Z_Registration_Info_UClass_AGrapplePlayerCharacter.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AGrapplePlayerCharacter.OuterSingleton, Z_Construct_UClass_AGrapplePlayerCharacter_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AGrapplePlayerCharacter.OuterSingleton;
	}
	template<> UE_GRAPPLE_API UClass* StaticClass<AGrapplePlayerCharacter>()
	{
		return AGrapplePlayerCharacter::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGrapplePlayerCharacter);
	AGrapplePlayerCharacter::~AGrapplePlayerCharacter() {}
	struct Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_PlayerAndGM_GrapplePlayerCharacter_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_PlayerAndGM_GrapplePlayerCharacter_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AGrapplePlayerCharacter, AGrapplePlayerCharacter::StaticClass, TEXT("AGrapplePlayerCharacter"), &Z_Registration_Info_UClass_AGrapplePlayerCharacter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AGrapplePlayerCharacter), 3461914360U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_PlayerAndGM_GrapplePlayerCharacter_h_3245213782(TEXT("/Script/UE_Grapple"),
		Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_PlayerAndGM_GrapplePlayerCharacter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_PlayerAndGM_GrapplePlayerCharacter_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
