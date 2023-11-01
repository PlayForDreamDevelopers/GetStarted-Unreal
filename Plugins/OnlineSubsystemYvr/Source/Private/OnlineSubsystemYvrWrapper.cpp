#include "OnlineSubsystemYvrWrapper.h"

#if PLATFORM_ANDROID
bool OnlineSubsystemYvrWrapper::InitPlatformSDK(long AppId)
{
	bool Ret = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "initYvrPlatformSdk", "(J)Z", false);
		Ret = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method, AppId);
	}
	return Ret;
}

jobject OnlineSubsystemYvrWrapper::PopMessage()
{
	jobject Ret = nullptr;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_PopMessage", "()Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
	return Ret;
}


int OnlineSubsystemYvrWrapper::GetRequestId(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_getRequestId", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetRequestType(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_getRequestType", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

bool OnlineSubsystemYvrWrapper::IsMessageError(jobject Obj)
{
	bool Ret = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_isError", "(Ljava/lang/Object;)Z", false);
		Ret = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetErrorMessage(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_getErrorMsg", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetErrorCode(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_getErrorCode", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

void OnlineSubsystemYvrWrapper::FreeRequest(int RequestId)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_freeRequest", "(I)V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, RequestId);
	}
}

int OnlineSubsystemYvrWrapper::GetLoggedInUser()
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "yvr_user_GetLoggedInUser", "()I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetAccountId(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getYvrAccountId", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetUserName(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getYvrUserName", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetUserIcon(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getYvrUserIcon", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetUserSex(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getYvrUserSex", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::AddCount(FString Name, long Count)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring AchievementName = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "addCount", "(Ljava/lang/String;J)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, AchievementName, Count);
		Env->DeleteLocalRef(AchievementName);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::AddFields(FString Name, FString Fields)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring AchievementName = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		jstring AchievementFields = Env->NewStringUTF(TCHAR_TO_UTF8(*Fields));
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "addFields", "(Ljava/lang/String;Ljava/lang/String;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, AchievementName, AchievementFields);
		Env->DeleteLocalRef(AchievementName);
		Env->DeleteLocalRef(AchievementFields);

	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::UnlockAchievement(FString Name)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring AchievementName = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "unlockAchievement", "(Ljava/lang/String;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, AchievementName);
		Env->DeleteLocalRef(AchievementName);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetAllDefinitions()
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getAllDefinitions", "()I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetDefinitionByNames(TArray<FString> Names)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto AchievementNames = NewScopedJavaObject(Env, Env->NewObjectArray(Names.Num(), FJavaWrapper::JavaStringClass, NULL));
		if (AchievementNames)
		{
			for (int Index = 0; Index < Names.Num(); ++Index)
			{
				auto StringValue = FJavaHelper::ToJavaString(Env, Names[Index]);
				Env->SetObjectArrayElement(*AchievementNames, Index, *StringValue);
			}
		}

		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getDefinitionByNames", "([Ljava/lang/String;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, *AchievementNames);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetSizeOfAllDefinitions(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getSizeOfAllDefinitions", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

jobject OnlineSubsystemYvrWrapper::GetElementOfDefinitions(jobject Obj, int Index)
{
	jobject Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getElementOfDefinitions", "(Ljava/lang/Object;I)Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj, Index);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetIdFromElementOfDefinitions(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getIdFromElementOfDefinitions", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetApiNameFromElementOfDefinitions(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getApiNameFromElementOfDefinitions", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetAchievementTypeFromElementOfDefinitions(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getAchievementTypeFromElementOfDefinitions", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetPolicyFromElementOfDefinitions(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getPolicyFromElementOfDefinitions", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetTargetFromElementOfDefinitions(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getTargetFromElementOfDefinitions", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetBitfieldLengthFromElementOfDefinitions(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getBitfieldLengthFromElementOfDefinitions", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

bool OnlineSubsystemYvrWrapper::IsAchievedFromElementOfDefinitions(jobject Obj)
{
	bool Ret = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getIsAchievedFromElementOfDefinitions", "(Ljava/lang/Object;)Z", false);
		Ret = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetTitleFromElementOfDefinitions(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getTitleFromElementOfDefinitions", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetDescriptionFromElementOfDefinitions(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getDescriptionFromElementOfDefinitions", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetUnlockedDescriptionFromElementOfDefinitions(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getUnlockedDescriptionFromElementOfDefinitions", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

bool OnlineSubsystemYvrWrapper::IsSecretFromElementOfDefinitions(jobject Obj)
{
	bool Ret = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getIsSecretFromElementOfDefinitions", "(Ljava/lang/Object;)Z", false);
		Ret = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetLockedImageFromElementOfDefinitions(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getLockedImageFromElementOfDefinitions", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetUnlockedImageFromElementOfDefinitions(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getUnlockedImageFromElementOfDefinitions", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

long OnlineSubsystemYvrWrapper::GetCreatedTimeFromElementOfDefinitions(jobject Obj)
{
	long Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getCreatedTimeFromElementOfDefinitions", "(Ljava/lang/Object;)J", false);
		Ret = FJavaWrapper::CallLongMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

long OnlineSubsystemYvrWrapper::GetUpdateTimeFromElementOfDefinitions(jobject Obj)
{
	long Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getUpdateTimeFromElementOfDefinitions", "(Ljava/lang/Object;)J", false);
		Ret = FJavaWrapper::CallLongMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetAllProgress()
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "GetAllProgress", "()I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetProgressByName(TArray<FString> Names)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		auto AchievementNames = NewScopedJavaObject(Env, Env->NewObjectArray(Names.Num(), FJavaWrapper::JavaStringClass, NULL));
		if (AchievementNames)
		{
			for (int Index = 0; Index < Names.Num(); ++Index)
			{
				auto StringValue = FJavaHelper::ToJavaString(Env, Names[Index]);
				Env->SetObjectArrayElement(*AchievementNames, Index, *StringValue);
			}
		}

		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "GetProgressByName", "([Ljava/lang/String;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, *AchievementNames);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetSizeOfAchievementProgress(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getSizeOfAchievementProgress", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

jobject OnlineSubsystemYvrWrapper::GetElementOfAchievementProgress(jobject Obj, int Index)
{
	jobject Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getElementOfAchievementProgress", "(Ljava/lang/Object;I)Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj, Index);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetIdFromElementOfAchievementProgress(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getIdFromElementOfAchievementProgress", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

jobject OnlineSubsystemYvrWrapper::GetDefinitionFromElementOfAchievementProgress(jobject Obj)
{
	jobject Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getDefinitionFromElementOfAchievementProgress", "(Ljava/lang/Object;)Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetNameFromDefinitionOfAchievementProgress(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getNameFromDefinitionOfAchievementProgress", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetTargetFromDefinitionOfAchievementProgress(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getTargetFromDefinitionOfAchievementProgress", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetCountProgressFromElementOfAchievementProgress(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getCountProgressFromElementOfAchievementProgress", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetBitfieldProgressFromElementOfAchievementProgress(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getBitfieldProgressFromElementOfAchievementProgress", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

bool OnlineSubsystemYvrWrapper::IsUnlockedFromElementOfAchievementProgress(jobject Obj)
{
	bool Ret = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "isUnlockedFromElementOfAchievementProgress", "(Ljava/lang/Object;)Z", false);
		Ret = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

long OnlineSubsystemYvrWrapper::GetUnlockTimeFromElementOfAchievementProgress(jobject Obj)
{
	long Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getUnlockTimeFromElementOfAchievementProgress", "(Ljava/lang/Object;)J", false);
		Ret = FJavaWrapper::CallLongMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetViewerEntitled()
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getViewerEntitled", "()I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
	return Ret;
}

bool OnlineSubsystemYvrWrapper::IsViewerEntitled(jobject Obj)
{
	bool Ret = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "isViewerEntitled", "(Ljava/lang/Object;)Z", false);
		Ret = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetYvrFriends()
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getYvrFriends", "()I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetFriendsSize(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getFriendsSize", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

jobject OnlineSubsystemYvrWrapper::GetItemOfFriendsList(jobject Obj, int Index)
{
	jobject Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getItemOfFriendsList", "(Ljava/lang/Object;I)Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj, Index);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetActIdOfFriendItem(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getActIdOfFriendItem", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetNickOfFriendItem(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getNickOfFriendItem", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetAgeOfFriendItem(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getAgeOfFriendItem", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetSexOfFriendItem(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getSexOfFriendItem", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetIconOfFriendItem(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getIconOfFriendItem", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetOnlineOfFriendItem(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getOnlineOfFriendItem", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

jobject OnlineSubsystemYvrWrapper::GetUsingAppOfFriendItem(jobject Obj)
{
	jobject Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getUsingAppOfFriendItem", "(Ljava/lang/Object;)Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetScoverOfUsingApp(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getScoverOfUsingApp", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetTypeOfUsingApp(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getTypeOfUsingApp", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetPkgOfUsingApp(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getPkgOfUsingApp", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetNameOfUsingApp(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getNameOfUsingApp", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetYvrFriendInfo(int Index)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getYvrFriendInfo", "(I)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Index);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetActIdOfUser(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getActIdOfUser", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetNickOfUser(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getNickOfUser", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetAgeOfUser(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getAgeOfUser", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetSexOfUser(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getSexOfUser", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetIconOfUser(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getIconOfUser", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetOnlineOfUser(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getOnlineOfUser", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::WriteLeaderboardItem(FString Name, float Score, void* Data, int DataLength, bool ForceUpdate)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring LeaderboardName = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		jbyteArray LeaderboardData = Env->NewByteArray(DataLength);
		Env->SetByteArrayRegion(LeaderboardData, 0, DataLength, reinterpret_cast<const jbyte*>(Data));

		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "addItem", "(Ljava/lang/String;D[BJI)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, LeaderboardName, (double)Score, LeaderboardData, (long)DataLength, (int)ForceUpdate);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::ReadLeaderboardItems(FString Name, long Size, FString FilterType, FString StartAt)
{
	int Ret = 0;

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring LeaderboardName = Env->NewStringUTF(TCHAR_TO_UTF8(*Name));
		jstring LeaderboardFilterType = Env->NewStringUTF(TCHAR_TO_UTF8(*FilterType));
		jstring LeaderboardStartAt = Env->NewStringUTF(TCHAR_TO_UTF8(*StartAt));

		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "reqLeaderboardList", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, LeaderboardName, LeaderboardFilterType, LeaderboardStartAt, Size);
	}

	return Ret;
}

jobject OnlineSubsystemYvrWrapper::GetLeaderboardList(jobject Obj)
{
	jobject Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getReqLDBLists", "(Ljava/lang/Object;)Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

int OnlineSubsystemYvrWrapper::GetLeaderboardItemSize(jobject Obj)
{
	int Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getSizeOfLDBLists", "(Ljava/lang/Object;)I", false);
		Ret = FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

jobject OnlineSubsystemYvrWrapper::GetItemOfLeaderboard(jobject Obj, int Index)
{
	jobject Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getItemOfLDBLists", "(Ljava/lang/Object;I)Ljava/lang/Object;", false);
		Ret = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj, Index);
	}
	return Ret;
}

long OnlineSubsystemYvrWrapper::GetLeaderboardUserId(jobject Obj)
{
	long Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getLeaderboardUserIdOfCurrLDB", "(Ljava/lang/Object;)J", false);
		Ret = FJavaWrapper::CallLongMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

FString OnlineSubsystemYvrWrapper::GetLeaderboardUserNickName(jobject Obj)
{
	FString Ret = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getLeaderboardUserNameOfCurrLDB", "(Ljava/lang/Object;)Ljava/lang/String;", false);
		jstring JavaString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
		if (JavaString != NULL)
		{
			const char* JavaChars = Env->GetStringUTFChars(JavaString, 0);
			Ret = FString(UTF8_TO_TCHAR(JavaChars));
			Env->ReleaseStringUTFChars(JavaString, JavaChars);
			Env->DeleteLocalRef(JavaString);
		}
	}
	return Ret;
}

long OnlineSubsystemYvrWrapper::GetLeaderboardRank(jobject Obj)
{
	long Ret = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getLeaderboardSortOfCurrLDB", "(Ljava/lang/Object;)J", false);
		Ret = FJavaWrapper::CallLongMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}

double OnlineSubsystemYvrWrapper::GetLeaderboardScore(jobject Obj)
{
	double Ret = 0.0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "getLeaderboardScoreOfCurrLDB", "(Ljava/lang/Object;)D", false);
		Ret = FJavaWrapper::CallDoubleMethod(Env, FJavaWrapper::GameActivityThis, Method, Obj);
	}
	return Ret;
}
#endif