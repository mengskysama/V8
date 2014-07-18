# Microsoft Developer Studio Project File - Name="FloatFR_sbrenclib_aac" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FloatFR_sbrenclib_aac - Win32 aacPlus Mono Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FloatFR_sbrenclib_aac.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FloatFR_sbrenclib_aac.mak" CFG="FloatFR_sbrenclib_aac - Win32 aacPlus Mono Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FloatFR_sbrenclib_aac - Win32 CT Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "FloatFR_sbrenclib_aac - Win32 CT Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FloatFR_sbrenclib_aac - Win32 CT Mono Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FloatFR_sbrenclib_aac - Win32 CT Mono Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_D/CT"
# PROP Intermediate_Dir "Win32_D/CT/obj"
# PROP Target_Dir ""
# ADD BASE CPP /MTd /W3 /GX /ZI /Od /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "AAC_BSFORMAT" /D "WIN32" /FR /FD /GZ /c
# SUBTRACT BASE CPP /nologo /YX
# ADD CPP /MTd /W3 /GX /ZI /Od /Op /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /I "..\FloatFR_fastaacenc" /I "..\FloatFR_fastaacenc\src" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /FD /GZ /c
# SUBTRACT CPP /nologo /Fr /YX
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_O/CT"
# PROP Intermediate_Dir "Win32_O/CT/obj"
# PROP Target_Dir ""
# ADD BASE CPP /MT /W3 /GX /Op /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "AAC_BSFORMAT" /D "WIN32" /FD /c
# SUBTRACT BASE CPP /nologo /YX
# ADD CPP /MT /W3 /GX /Od /Op /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /I "..\FloatFR_fastaacenc" /I "..\FloatFR_fastaacenc\src" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /FD /c
# SUBTRACT CPP /nologo /Fr /YX
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_O/mono_CT"
# PROP Intermediate_Dir "Win32_O/mono_CT/obj"
# PROP Target_Dir ""
# ADD BASE CPP /MT /W3 /GX /Od /Op /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /I "..\FloatFR_fastaacenc" /I "..\FloatFR_fastaacenc\src" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "PARAMETRICSTEREO" /FD /c
# SUBTRACT BASE CPP /nologo /Fr /YX
# ADD CPP /MT /W3 /GX /Od /Op /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /I "..\FloatFR_fastaacenc" /I "..\FloatFR_fastaacenc\src" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "MONO_ONLY" /FD /c
# SUBTRACT CPP /nologo /Fr /YX
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_D/mono_CT"
# PROP Intermediate_Dir "Win32_D/mono_CT/obj"
# PROP Target_Dir ""
# ADD BASE CPP /MTd /W3 /GX /ZI /Od /Op /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /I "..\FloatFR_fastaacenc" /I "..\FloatFR_fastaacenc\src" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "PARAMETRICSTEREO" /FD /GZ /c
# SUBTRACT BASE CPP /nologo /Fr /YX
# ADD CPP /MTd /W3 /GX /ZI /Od /Op /I ".\src" /I "..\util" /I "." /I "..\FloatFR_bitbuflib" /I "..\FloatFRlib" /I "..\FloatFR_fastaacenc" /I "..\FloatFR_fastaacenc\src" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "MONO_ONLY" /FD /GZ /c
# SUBTRACT CPP /nologo /Fr /YX
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "FloatFR_sbrenclib_aac - Win32 CT Debug"
# Name "FloatFR_sbrenclib_aac - Win32 CT Release"
# Name "FloatFR_sbrenclib_aac - Win32 CT Mono Release"
# Name "FloatFR_sbrenclib_aac - Win32 CT Mono Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\bit_sbr.c
# End Source File
# Begin Source File

SOURCE=.\src\code_env.c
# End Source File
# Begin Source File

SOURCE=.\src\env_bit.c
# End Source File
# Begin Source File

SOURCE=.\src\env_est.c
# End Source File
# Begin Source File

SOURCE=.\src\fram_gen.c
# End Source File
# Begin Source File

SOURCE=.\src\freq_sca.c
# End Source File
# Begin Source File

SOURCE=.\src\hybrid.c

!IF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Debug"

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Release"

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\invf_est.c
# End Source File
# Begin Source File

SOURCE=.\src\mh_det.c
# End Source File
# Begin Source File

SOURCE=.\src\nf_est.c
# End Source File
# Begin Source File

SOURCE=.\src\ps_bitenc.c

!IF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Debug"

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Release"

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\ps_enc.c

!IF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Debug"

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Release"

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FloatFR_sbrenclib_aac - Win32 CT Mono Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\qmf_enc.c
# End Source File
# Begin Source File

SOURCE=.\src\sbr_main.c
# End Source File
# Begin Source File

SOURCE=.\src\sbr_misc.c
# End Source File
# Begin Source File

SOURCE=.\src\sbr_ram.c
# End Source File
# Begin Source File

SOURCE=.\src\sbr_rom.c
# End Source File
# Begin Source File

SOURCE=.\src\ton_corr.c
# End Source File
# Begin Source File

SOURCE=.\src\tran_det.c
# End Source File
# End Group
# End Target
# End Project
