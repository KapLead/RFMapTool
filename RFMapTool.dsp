# Microsoft Developer Studio Project File - Name="RFMapTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RFMapTool - Win32 Server Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RFMapTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RFMapTool.mak" CFG="RFMapTool - Win32 Server Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RFMapTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RFMapTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "RFMapTool - Win32 Server Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ODIN/R3Technology/Core/RFMapTool", FCOEAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RFMapTool - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\R3engine\1stClass" /I "..\R3engine\2ndClass" /I "..\R3engine\Common" /I "..\R3engine\D3d\include" /I "..\CharHeader" /I "..\R3particle" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\R3bin\RFMapTool.exe"

!ELSEIF  "$(CFG)" == "RFMapTool - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\R3engine\1stClass" /I "..\R3engine\2ndClass" /I "..\R3engine\Common" /I "..\R3engine\D3d\include" /I "..\CharHeader" /I "..\R3particle" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\R3bin\RFMapTool.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RFMapTool - Win32 Server Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RFMapTool___Win32_Server_Debug"
# PROP BASE Intermediate_Dir "RFMapTool___Win32_Server_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RFMapTool___Win32_Server_Debug"
# PROP Intermediate_Dir "RFMapTool___Win32_Server_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\R3engine\1stClass" /I "..\R3engine\2ndClass" /I "..\R3engine\Common" /I "..\R3engine\D3d\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\R3engine\1stClass" /I "..\R3engine\2ndClass" /I "..\R3engine\Common" /I "..\R3engine\D3d\include" /I "..\CharHeader" /I "..\R3engine\Extension" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\R3bin\RFMapTool.exe" /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\R3bin\RFMapTool.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RFMapTool - Win32 Release"
# Name "RFMapTool - Win32 Debug"
# Name "RFMapTool - Win32 Server Debug"
# Begin Group "MfcWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\RFMapTool.cpp
# End Source File
# Begin Source File

SOURCE=.\RFMapTool.h
# End Source File
# Begin Source File

SOURCE=.\RFMapToolDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RFMapToolDoc.h
# End Source File
# Begin Source File

SOURCE=.\RFMapToolView.cpp
# End Source File
# Begin Source File

SOURCE=.\RFMapToolView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Header Files No. 1"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\cConSol.h
# End Source File
# Begin Source File

SOURCE=.\CMyD3DApp.h
# End Source File
# Begin Source File

SOURCE=.\MyColorButton.h
# End Source File
# Begin Source File

SOURCE=.\MySpin.h
# End Source File
# Begin Source File

SOURCE=.\RightBack.h
# End Source File
# Begin Source File

SOURCE=.\RollupCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TabCollision.h
# End Source File
# Begin Source File

SOURCE=.\TabDummy.h
# End Source File
# Begin Source File

SOURCE=.\TabEntity.h
# End Source File
# Begin Source File

SOURCE=.\TabMagic.h
# End Source File
# Begin Source File

SOURCE=.\TabParticle.h
# End Source File
# Begin Source File

SOURCE=.\TabSound.h
# End Source File
# Begin Source File

SOURCE=.\ToolManager.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cConSol.cpp
# End Source File
# Begin Source File

SOURCE=.\CMyD3DApp.CPP
# End Source File
# Begin Source File

SOURCE=.\Crender.cpp
# End Source File
# Begin Source File

SOURCE=.\Crender.h
# End Source File
# Begin Source File

SOURCE=.\MyColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MySpin.cpp
# End Source File
# Begin Source File

SOURCE=.\RightBack.cpp
# End Source File
# Begin Source File

SOURCE=.\RollupCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\TabEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\TabMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\TabParticle.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSound.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolManager.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Coll_DN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Coll_UP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dummy_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dummy_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Entity_DN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Entity_UP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\magic_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\magic_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Particle_DN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Particle_UP.bmp
# End Source File
# Begin Source File

SOURCE=.\R3_16.ICO
# End Source File
# Begin Source File

SOURCE=.\R3_32.ICO
# End Source File
# Begin Source File

SOURCE=.\res\R3maptool.ico
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RFMapTool.clw
# End Source File
# Begin Source File

SOURCE=.\res\RFMapTool.ico
# End Source File
# Begin Source File

SOURCE=.\RFMapTool.rc
# End Source File
# Begin Source File

SOURCE=.\res\RFMapTool.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RFMapToolDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\sound_dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sound_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "D3D & Character"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
