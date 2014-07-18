# Microsoft Developer Studio Project File - Name="FloatFR_aacPlusdec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=FloatFR_aacPlusdec - Win32 CT Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FloatFR_aacPlusdec_mpeg4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FloatFR_aacPlusdec_mpeg4.mak" CFG="FloatFR_aacPlusdec - Win32 CT Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FloatFR_aacPlusdec - Win32 CT Release" (based on "Win32 (x86) Console Application")
!MESSAGE "FloatFR_aacPlusdec - Win32 CT Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "FloatFR_aacPlusdec - Win32 CT Mono Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "FloatFR_aacPlusdec - Win32 CT Mono Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "FloatFR_aacPlusdec - Win32 CT Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_O/CT"
# PROP Intermediate_Dir "Win32_O/CT/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_sbrdeclib" /I "audiolib" /I "FloatFR_bitbuflib" /I "util" /I "libisomediafile/src" /I "libisomediafile/w32" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "LP_SBR_ONLY" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /Op /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_aacdec/src" /I "FloatFR_sbrdeclib" /I "lib" /I "lib/w32" /I "FloatFR_bitbuflib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"Win32_O/CT/enhAacPlusDec.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy     .\lib\w32\ct-libisomedia.dll     .\Win32_O\CT
# End Special Build Tool

!ELSEIF  "$(CFG)" == "FloatFR_aacPlusdec - Win32 CT Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_D/CT"
# PROP Intermediate_Dir "Win32_D/CT/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_sbrdeclib" /I "audiolib" /I "FloatFR_bitbuflib" /I "util" /I "libisomediafile/src" /I "libisomediafile/w32" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "PARAMETRICSTEREO_3GPP" /D "POSTFILTER" /FR /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /Op /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_aacdec/src" /I "FloatFR_sbrdeclib" /I "lib" /I "lib/w32" /I "FloatFR_bitbuflib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Win32_D/CT/enhAacPlusDec.exe" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy     .\lib\w32\ct-libisomedia.dll     .\Win32_D\CT
# End Special Build Tool

!ELSEIF  "$(CFG)" == "FloatFR_aacPlusdec - Win32 CT Mono Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_D/mono_CT"
# PROP Intermediate_Dir "Win32_D/mono_CT/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /Op /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_aacdec/src" /I "FloatFR_sbrdeclib" /I "audiolib" /I "FloatFR_bitbuflib" /I "libisomediafile/src" /I "libisomediafile/w32" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "PARAMETRICSTEREO_3GPP" /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /Op /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_aacdec/src" /I "FloatFR_sbrdeclib" /I "lib" /I "lib/w32" /I "FloatFR_bitbuflib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "MONO_ONLY" /D "LP_SBR_ONLY" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Win32_D/CT/ctdec.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Win32_D/mono_CT/enhAacPlusDec_mono.exe" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy     .\lib\w32\ct-libisomedia.dll     .\Win32_D\mono_CT
# End Special Build Tool

!ELSEIF  "$(CFG)" == "FloatFR_aacPlusdec - Win32 CT Mono Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_O/mono_CT"
# PROP Intermediate_Dir "Win32_O/mono_CT/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Od /Op /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_aacdec/src" /I "FloatFR_sbrdeclib" /I "audiolib" /I "FloatFR_bitbuflib" /I "libisomediafile/src" /I "libisomediafile/w32" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "PARAMETRICSTEREO_3GPP" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /Od /Op /I "FloatFRlib" /I "FloatFR_aacdec" /I "FloatFR_aacdec/src" /I "FloatFR_sbrdeclib" /I "lib" /I "lib/w32" /I "FloatFR_bitbuflib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "MONO_ONLY" /D "LP_SBR_ONLY" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"Win32_O/CT/ctdec.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"Win32_O/mono_CT/enhAacPlusDec_mono.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy     .\lib\w32\ct-libisomedia.dll     .\Win32_O\mono_CT
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "FloatFR_aacPlusdec - Win32 CT Release"
# Name "FloatFR_aacPlusdec - Win32 CT Debug"
# Name "FloatFR_aacPlusdec - Win32 CT Mono Debug"
# Name "FloatFR_aacPlusdec - Win32 CT Mono Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\fileifc.c
# End Source File
# Begin Source File

SOURCE=.\src\main.c
# End Source File
# Begin Source File

SOURCE=.\src\spline_resampler.c
# End Source File
# End Group
# Begin Source File

SOURCE=".\lib\w32\ct-libisomedia.lib"
# End Source File
# Begin Source File

SOURCE=.\lib\w32\audiolib.lib
# End Source File
# End Target
# End Project
