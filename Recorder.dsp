# Microsoft Developer Studio Project File - Name="Recorder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Recorder - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Recorder.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Recorder.mak" CFG="Recorder - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Recorder - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Recorder - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Recorder - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Recorder - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 WCDPcie/WCDPcieDriver.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Recorder - Win32 Release"
# Name "Recorder - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckCardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChSelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxRange.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarExBottomAcq.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarExBottomPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarExTopChnSel.cpp
# End Source File
# Begin Source File

SOURCE=.\FftComplex.cpp
# End Source File
# Begin Source File

SOURCE=.\FftConfDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasureAddDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasureEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasureItem.cpp
# End Source File
# Begin Source File

SOURCE=.\PhyCard.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupPropertyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupRightDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Product.cpp
# End Source File
# Begin Source File

SOURCE=.\Recorder.cpp
# End Source File
# Begin Source File

SOURCE=.\Recorder.rc
# End Source File
# Begin Source File

SOURCE=.\RecorderDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RecorderView.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveAsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StoragePathDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualCard.cpp
# End Source File
# Begin Source File

SOURCE=.\WatchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveData.cpp
# End Source File
# Begin Source File

SOURCE=.\WndManage.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BgDlg.h
# End Source File
# Begin Source File

SOURCE=.\CheckCardDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChSelDlg.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxRange.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarEx.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarExBottomAcq.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarExBottomPlay.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarExTopChnSel.h
# End Source File
# Begin Source File

SOURCE=.\FftComplex.h
# End Source File
# Begin Source File

SOURCE=.\FftConfDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MarkDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasureAddDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasureDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasureEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasureItem.h
# End Source File
# Begin Source File

SOURCE=.\MyDef.h
# End Source File
# Begin Source File

SOURCE=.\PhyCard.h
# End Source File
# Begin Source File

SOURCE=.\PlayBar.h
# End Source File
# Begin Source File

SOURCE=.\PopupPropertyDlg.h
# End Source File
# Begin Source File

SOURCE=.\PopupRightDlg.h
# End Source File
# Begin Source File

SOURCE=.\Product.h
# End Source File
# Begin Source File

SOURCE=.\Recorder.h
# End Source File
# Begin Source File

SOURCE=.\RecorderDoc.h
# End Source File
# Begin Source File

SOURCE=.\RecorderView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveAsDlg.h
# End Source File
# Begin Source File

SOURCE=.\SaveDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StoragePathDlg.h
# End Source File
# Begin Source File

SOURCE=.\VirtualCard.h
# End Source File
# Begin Source File

SOURCE=.\WatchDlg.h
# End Source File
# Begin Source File

SOURCE=.\WaveData.h
# End Source File
# Begin Source File

SOURCE=.\WndManage.h
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_clos.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_mark.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_play.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_rest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_scal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_spee.bmp
# End Source File
# Begin Source File

SOURCE=.\res\c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_m.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_z.cur
# End Source File
# Begin Source File

SOURCE=.\res\Recorder.ico
# End Source File
# Begin Source File

SOURCE=.\res\Recorder.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RecorderDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
