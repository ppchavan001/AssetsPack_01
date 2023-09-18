// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "NetworkingWrapperFunctionLibrary.generated.h"



UENUM(BlueprintType)
enum class ENetWrapLogVerbosity : uint8
{
	/** returns without logging */
	NoLogging = 0,

	/** Always prints a fatal error to console (and log file) and crashes (even if logging is disabled) */
	Fatal,

	/**
		* Prints an error to console (and log file).
		* Commandlets and the editor collect and report errors. Error messages result in commandlet failure.
		*/
	Error,

	/**
	* Prints a warning to console (and log file).
	* Commandlets and the editor collect and report warnings. Warnings can be treated as an error.
	*/
	Warning,

	/** Prints a message to console (and log file) */
	Display,

	/** Prints a message to a log file (does not print to console) */
	Log,

	/**
	* Prints a verbose message to a log file (if Verbose logging is enabled for the given category,
	* usually used for detailed logging)
	*/
	Verbose,

	/**
	* Prints a verbose message to a log file (if VeryVerbose logging is enabled,
	* usually used for detailed logging that would otherwise spam output)
	*/
	VeryVerbose

};
#define GETENUMSTRING(etype, evalue) ( (FindObject(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

/**
 *
 */
UCLASS()
class NETWORKINGWRAPPER_API UNetworkingWrapperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	* tries to convert given string to a object of given class
	* the string must be json representation of the object
	*/
	UFUNCTION(BlueprintCallable)
	static UObject* String2UObject(const FString& string, UClass* classType);

	/*
	* Converts the given object to a json string
	*/
	UFUNCTION(BlueprintCallable)
	static FString UObject2String(const UObject* packet);

	/*
	* calls UDP_Send on all objects with UDP Send interface
	* with given parameters
	*/
	UFUNCTION(BlueprintCallable, Category = "Networking Wrapper | UDP")
	static void UDP_SendStr(const UObject* WorldContextObject, const FString& Data);

	/*
	* calls UDP_Send on all objects with UDP Send interface
	* with given parameters
	*/
	UFUNCTION(BlueprintCallable, Category = "Networking Wrapper | UDP")
	static void UDP_SendObj(const UObject* WorldContextObject, const UObject* Data);


	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, Keywords = "log print", AdvancedDisplay = "2", DevelopmentOnly), Category = "Networking Wrapper")
	static void NW_PrintString(const UObject* WorldContextObject,
		const FString InString = "",
		ENetWrapLogVerbosity LogVerbosity = ENetWrapLogVerbosity::Log,
		bool bPrintToScreen = true,
		bool bPrintToLog = true,
		float Duration = 2.f,
		int MaxStackDataDepth = 1);

};
