#pragma once
#ifndef _INCLUDE_WWISE_H_
#define _INCLUDE_WWISE_H_

#ifndef _DEBUG
#define AK_OPTIMIZED
#endif



#include "Wwise/SDK/include/AK/SoundEngine/Common/AkMemoryMgr.h"
#include "Wwise/SDK/include/AK/SoundEngine/Common/AkModule.h" 
#include "Wwise/SDK/include/AK/SoundEngine/Common/IAkStreamMgr.h"               // Streaming Manager
#include "Wwise/SDK/include/AK/Tools/Common/AkPlatformFuncs.h"                 // Thread defines            // Sample low-level I/O implementation
#include "Wwise/SDK/include/AK/SoundEngine/Common/AkSoundEngine.h"                // Sound engine
#include "Wwise/SDK/include/AK/MusicEngine/Common/AkMusicEngine.h" 

// Include for communication between Wwise and the game -- Not needed in the release version
#ifndef AK_OPTIMIZED
#include "Wwise/SDK/include/AK/Comm/AkCommunication.h"
#endif // AK_OPTIMIZED

#ifdef AK_OPTIMIZED

#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Release/lib/AkSoundEngine.lib")
#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Release/lib/AkMusicEngine.lib")
#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Release/lib/AkMemoryMgr.lib")
#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Release/lib/AkStreamMgr.lib")

#else

#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Debug/lib/AkSoundEngine.lib")
#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Debug/lib/AkMusicEngine.lib")
#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Debug/lib/AkMemoryMgr.lib")
#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Debug/lib/AkStreamMgr.lib")

#endif

#ifndef AK_OPTIMIZED
#include "Wwise/SDK/include/AK/Comm/AkCommunication.h"
#pragma comment( lib, "Wwise/SDK/UWP_Win32_vc150/Debug/lib/CommunicationCentral.lib")
#endif

namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size);
	void FreeHook(void * in_ptr);
	void * VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType);
#endif
}

#endif



