// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UE_Grapple/Public/GrappleGM.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGrappleGM() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UE_GRAPPLE_API UClass* Z_Construct_UClass_AGrappleGM();
	UE_GRAPPLE_API UClass* Z_Construct_UClass_AGrappleGM_NoRegister();
	UPackage* Z_Construct_UPackage__Script_UE_Grapple();
// End Cross Module References
	void AGrappleGM::StaticRegisterNativesAGrappleGM()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AGrappleGM);
	UClass* Z_Construct_UClass_AGrappleGM_NoRegister()
	{
		return AGrappleGM::StaticClass();
	}
	struct Z_Construct_UClass_AGrappleGM_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGrappleGM_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_UE_Grapple,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AGrappleGM_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGrappleGM_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "GrappleGM.h" },
		{ "ModuleRelativePath", "Public/GrappleGM.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGrappleGM_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGrappleGM>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AGrappleGM_Statics::ClassParams = {
		&AGrappleGM::StaticClass,
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
		0x009002ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AGrappleGM_Statics::Class_MetaDataParams), Z_Construct_UClass_AGrappleGM_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_AGrappleGM()
	{
		if (!Z_Registration_Info_UClass_AGrappleGM.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AGrappleGM.OuterSingleton, Z_Construct_UClass_AGrappleGM_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AGrappleGM.OuterSingleton;
	}
	template<> UE_GRAPPLE_API UClass* StaticClass<AGrappleGM>()
	{
		return AGrappleGM::StaticClass();
	}
	AGrappleGM::AGrappleGM(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGrappleGM);
	AGrappleGM::~AGrappleGM() {}
	struct Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_GrappleGM_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_GrappleGM_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AGrappleGM, AGrappleGM::StaticClass, TEXT("AGrappleGM"), &Z_Registration_Info_UClass_AGrappleGM, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AGrappleGM), 2898020435U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_GrappleGM_h_2191485832(TEXT("/Script/UE_Grapple"),
		Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_GrappleGM_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_oleor_Documents_GitHub_UniStuff_Projects_Grapple_UE_Grapple_Source_UE_Grapple_Public_GrappleGM_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
