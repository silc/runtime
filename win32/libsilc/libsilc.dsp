# Microsoft Developer Studio Project File - Name="libsilc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libsilc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libsilc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libsilc.mak" CFG="libsilc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libsilc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libsilc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libsilc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBSILC_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W2 /GX /O2 /I ".\\" /I "..\\" /I "..\..\\" /I "..\..\includes" /I "..\..\lib\silccore" /I "..\..\lib\silcske" /I "..\..\lib\silcmath" /I "..\..\lib\silcmath\mpi" /I "..\..\lib\silcutil" /I "..\..\lib\silccrypt" /I "..\..\lib\silcsim" /I "..\..\lib\trq" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBSILC_EXPORTS" /D "MP_API_COMPATIBLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /machine:I386 /def:"libsilc.def"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libsilc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBSILC_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W2 /Gm /GX /ZI /Od /I ".\\" /I "..\\" /I "..\..\\" /I "..\..\includes" /I "..\..\lib\silccore" /I "..\..\lib\silcske" /I "..\..\lib\silcmath" /I "..\..\lib\silcmath\mpi" /I "..\..\lib\silcutil" /I "..\..\lib\silccrypt" /I "..\..\lib\silcsim" /I "..\..\lib\trq" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBSILC_EXPORTS" /D "MP_API_COMPATIBLE" /D "SILC_DEBUG" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /debug /machine:I386 /def:"libsilc.def" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "libsilc - Win32 Release"
# Name "libsilc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "silccore"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silccore\silcauth.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcchannel.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silccommand.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcid.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcidcache.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcnotify.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcpacket.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcpayload.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcprivate.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcprotocol.c
# End Source File
# End Group
# Begin Group "silcske"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silcske\groups.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcske\payload.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcske\silcske.c
# End Source File
# End Group
# Begin Group "silcutil"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silcutil\silcbuffmt.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcconfig.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silchashtable.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silclog.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcmemory.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcnet.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcschedule.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcsockconn.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcutil.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\win32\silcwin32mutex.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\win32\silcwin32net.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\win32\silcwin32schedule.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\win32\silcwin32sockconn.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\win32\silcwin32thread.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\win32\silcwin32util.c
# End Source File
# End Group
# Begin Group "silcmath"

# PROP Default_Filter ""
# Begin Group "mpi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\mpi.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\mplogic.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\mpmontg.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\mpprime.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\lib\silcmath\modinv.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mp_mpi.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpbin.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\silcprimegen.c
# End Source File
# End Group
# Begin Group "silccrypt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silccrypt\aes.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\blowfish.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\cast.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\mars.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\md5.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\none.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\pkcs1.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rc5.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rc6.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rsa.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\sha1.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silccipher.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silchash.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silchmac.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silcpkcs.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silcrng.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\twofish.c
# End Source File
# End Group
# Begin Group "trq"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_sort.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_001.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_002.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_003.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_004.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_005.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_006.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_007.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_008.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_009.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x_010.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_sort.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_001.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_002.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_003.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_004.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_005.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_006.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_007.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_008.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_009.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_010.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_011.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_012.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_013.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_014.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_015.c
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x_016.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "silccore No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silccore\silcauth.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcchannel.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silccommand.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcid.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcidcache.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcmode.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcnotify.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcpacket.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcpayload.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcprivate.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccore\silcprotocol.h
# End Source File
# End Group
# Begin Group "silcske No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silcske\groups.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcske\groups_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcske\payload.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcske\silcske.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcske\silcske_status.h
# End Source File
# End Group
# Begin Group "silcutil No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silcutil\silcbuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcbuffmt.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcbufutil.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silchashtable.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silclog.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcmemory.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcmutex.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcnet.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcschedule.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcschedule_i.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcsockconn.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcthread.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcutil\silcutil.h
# End Source File
# End Group
# Begin Group "silcmath No. 1"

# PROP Default_Filter ""
# Begin Group "mpi No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\logtab.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\montmulf.h
# End Source File
# Begin Source File

SOURCE="..\..\lib\silcmath\mpi\mpi-config.h"
# End Source File
# Begin Source File

SOURCE="..\..\lib\silcmath\mpi\mpi-priv.h"
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\mpi.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\mplogic.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\mpi\mpprime.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\lib\silcmath\mp_mpi.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\silcmath.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silcmath\silcmp.h
# End Source File
# End Group
# Begin Group "silccrypt No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\silccrypt\aes.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\blowfish.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\blowfish_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\cast.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\cast_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\ciphers.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\ciphers_def.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\mars.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\mars_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\md5.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\md5_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\none.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\pkcs1.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rc5.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rc5_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rc6.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rc6_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rijndael_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rsa.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\rsa_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\sha1.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\sha1_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silccipher.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silcdh.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silchash.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silchmac.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silcpkcs.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\silcrng.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\twofish.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\silccrypt\twofish_internal.h
# End Source File
# End Group
# Begin Group "trq No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\trq\silcdlist.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\silclist.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_conf.h.sample
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_f.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_deque_x.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_f.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\trq\trq_list_x.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\silcdefs.h
# End Source File
# Begin Source File

SOURCE=..\trq_conf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project