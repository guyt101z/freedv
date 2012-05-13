##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=portaudio
ConfigurationName      :=Release
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "C:\Users\wittend\Projects\audio"
ProjectPath            := "C:\Users\wittend\Projects\audio\portaudio"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=wittend
Date                   :=5/2/2012
CodeLitePath           :="C:\Program Files\CodeLite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="C:\Users\wittend\Projects\audio\portaudio\portaudio.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
CmpOptions             :=  $(Preprocessors)
C_CmpOptions           :=  $(Preprocessors)
LinkOptions            :=  -O2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src/os/common $(IncludeSwitch)../src/os/win $(IncludeSwitch)../src/os/hostapi $(IncludeSwitch)../include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
LibPath                := $(LibraryPathSwitch). 


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
WXWIN:=C:\bin\wxWidgets-2.9.2
PATH:=$(WXWIN)\lib\gcc_dll;$(PATH)
WXCFG:=gcc_dll\mswu
UNIT_TEST_PP_SRC_DIR:=C:\bin\UnitTest++-1.3
Objects=$(IntermediateDirectory)/win_pa_win_coinitialize$(ObjectSuffix) $(IntermediateDirectory)/win_pa_win_hostapis$(ObjectSuffix) $(IntermediateDirectory)/win_pa_win_util$(ObjectSuffix) $(IntermediateDirectory)/win_pa_win_waveformat$(ObjectSuffix) $(IntermediateDirectory)/win_pa_win_wdmks_utils$(ObjectSuffix) $(IntermediateDirectory)/win_pa_x86_plain_converters$(ObjectSuffix) $(IntermediateDirectory)/dsound_pa_win_ds$(ObjectSuffix) $(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(ObjectSuffix) $(IntermediateDirectory)/common_pa_allocation$(ObjectSuffix) $(IntermediateDirectory)/common_pa_converters$(ObjectSuffix) \
	$(IntermediateDirectory)/common_pa_cpuload$(ObjectSuffix) $(IntermediateDirectory)/common_pa_debugprint$(ObjectSuffix) $(IntermediateDirectory)/common_pa_dither$(ObjectSuffix) $(IntermediateDirectory)/common_pa_front$(ObjectSuffix) $(IntermediateDirectory)/common_pa_process$(ObjectSuffix) $(IntermediateDirectory)/common_pa_ringbuffer$(ObjectSuffix) $(IntermediateDirectory)/common_pa_stream$(ObjectSuffix) $(IntermediateDirectory)/common_pa_trace$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "C:\Users\wittend\Projects\audio\.build-release"
	@echo rebuilt > "C:\Users\wittend\Projects\audio\.build-release\portaudio"

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/win_pa_win_coinitialize$(ObjectSuffix): src/os/win/pa_win_coinitialize.c $(IntermediateDirectory)/win_pa_win_coinitialize$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_coinitialize.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/win_pa_win_coinitialize$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win_pa_win_coinitialize$(DependSuffix): src/os/win/pa_win_coinitialize.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win_pa_win_coinitialize$(ObjectSuffix) -MF$(IntermediateDirectory)/win_pa_win_coinitialize$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_coinitialize.c"

$(IntermediateDirectory)/win_pa_win_coinitialize$(PreprocessSuffix): src/os/win/pa_win_coinitialize.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win_pa_win_coinitialize$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_coinitialize.c"

$(IntermediateDirectory)/win_pa_win_hostapis$(ObjectSuffix): src/os/win/pa_win_hostapis.c $(IntermediateDirectory)/win_pa_win_hostapis$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_hostapis.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/win_pa_win_hostapis$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win_pa_win_hostapis$(DependSuffix): src/os/win/pa_win_hostapis.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win_pa_win_hostapis$(ObjectSuffix) -MF$(IntermediateDirectory)/win_pa_win_hostapis$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_hostapis.c"

$(IntermediateDirectory)/win_pa_win_hostapis$(PreprocessSuffix): src/os/win/pa_win_hostapis.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win_pa_win_hostapis$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_hostapis.c"

$(IntermediateDirectory)/win_pa_win_util$(ObjectSuffix): src/os/win/pa_win_util.c $(IntermediateDirectory)/win_pa_win_util$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_util.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/win_pa_win_util$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win_pa_win_util$(DependSuffix): src/os/win/pa_win_util.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win_pa_win_util$(ObjectSuffix) -MF$(IntermediateDirectory)/win_pa_win_util$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_util.c"

$(IntermediateDirectory)/win_pa_win_util$(PreprocessSuffix): src/os/win/pa_win_util.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win_pa_win_util$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_util.c"

$(IntermediateDirectory)/win_pa_win_waveformat$(ObjectSuffix): src/os/win/pa_win_waveformat.c $(IntermediateDirectory)/win_pa_win_waveformat$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_waveformat.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/win_pa_win_waveformat$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win_pa_win_waveformat$(DependSuffix): src/os/win/pa_win_waveformat.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win_pa_win_waveformat$(ObjectSuffix) -MF$(IntermediateDirectory)/win_pa_win_waveformat$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_waveformat.c"

$(IntermediateDirectory)/win_pa_win_waveformat$(PreprocessSuffix): src/os/win/pa_win_waveformat.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win_pa_win_waveformat$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_waveformat.c"

$(IntermediateDirectory)/win_pa_win_wdmks_utils$(ObjectSuffix): src/os/win/pa_win_wdmks_utils.c $(IntermediateDirectory)/win_pa_win_wdmks_utils$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_wdmks_utils.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/win_pa_win_wdmks_utils$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win_pa_win_wdmks_utils$(DependSuffix): src/os/win/pa_win_wdmks_utils.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win_pa_win_wdmks_utils$(ObjectSuffix) -MF$(IntermediateDirectory)/win_pa_win_wdmks_utils$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_wdmks_utils.c"

$(IntermediateDirectory)/win_pa_win_wdmks_utils$(PreprocessSuffix): src/os/win/pa_win_wdmks_utils.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win_pa_win_wdmks_utils$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_win_wdmks_utils.c"

$(IntermediateDirectory)/win_pa_x86_plain_converters$(ObjectSuffix): src/os/win/pa_x86_plain_converters.c $(IntermediateDirectory)/win_pa_x86_plain_converters$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_x86_plain_converters.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/win_pa_x86_plain_converters$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win_pa_x86_plain_converters$(DependSuffix): src/os/win/pa_x86_plain_converters.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win_pa_x86_plain_converters$(ObjectSuffix) -MF$(IntermediateDirectory)/win_pa_x86_plain_converters$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_x86_plain_converters.c"

$(IntermediateDirectory)/win_pa_x86_plain_converters$(PreprocessSuffix): src/os/win/pa_x86_plain_converters.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win_pa_x86_plain_converters$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/os/win/pa_x86_plain_converters.c"

$(IntermediateDirectory)/dsound_pa_win_ds$(ObjectSuffix): src/hostapi/dsound/pa_win_ds.c $(IntermediateDirectory)/dsound_pa_win_ds$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/hostapi/dsound/pa_win_ds.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/dsound_pa_win_ds$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dsound_pa_win_ds$(DependSuffix): src/hostapi/dsound/pa_win_ds.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dsound_pa_win_ds$(ObjectSuffix) -MF$(IntermediateDirectory)/dsound_pa_win_ds$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/hostapi/dsound/pa_win_ds.c"

$(IntermediateDirectory)/dsound_pa_win_ds$(PreprocessSuffix): src/hostapi/dsound/pa_win_ds.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dsound_pa_win_ds$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/hostapi/dsound/pa_win_ds.c"

$(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(ObjectSuffix): src/hostapi/dsound/pa_win_ds_dynlink.c $(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/hostapi/dsound/pa_win_ds_dynlink.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(DependSuffix): src/hostapi/dsound/pa_win_ds_dynlink.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(ObjectSuffix) -MF$(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/hostapi/dsound/pa_win_ds_dynlink.c"

$(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(PreprocessSuffix): src/hostapi/dsound/pa_win_ds_dynlink.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/hostapi/dsound/pa_win_ds_dynlink.c"

$(IntermediateDirectory)/common_pa_allocation$(ObjectSuffix): src/common/pa_allocation.c $(IntermediateDirectory)/common_pa_allocation$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_allocation.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_allocation$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_allocation$(DependSuffix): src/common/pa_allocation.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_allocation$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_allocation$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_allocation.c"

$(IntermediateDirectory)/common_pa_allocation$(PreprocessSuffix): src/common/pa_allocation.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_allocation$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_allocation.c"

$(IntermediateDirectory)/common_pa_converters$(ObjectSuffix): src/common/pa_converters.c $(IntermediateDirectory)/common_pa_converters$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_converters.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_converters$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_converters$(DependSuffix): src/common/pa_converters.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_converters$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_converters$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_converters.c"

$(IntermediateDirectory)/common_pa_converters$(PreprocessSuffix): src/common/pa_converters.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_converters$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_converters.c"

$(IntermediateDirectory)/common_pa_cpuload$(ObjectSuffix): src/common/pa_cpuload.c $(IntermediateDirectory)/common_pa_cpuload$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_cpuload.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_cpuload$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_cpuload$(DependSuffix): src/common/pa_cpuload.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_cpuload$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_cpuload$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_cpuload.c"

$(IntermediateDirectory)/common_pa_cpuload$(PreprocessSuffix): src/common/pa_cpuload.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_cpuload$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_cpuload.c"

$(IntermediateDirectory)/common_pa_debugprint$(ObjectSuffix): src/common/pa_debugprint.c $(IntermediateDirectory)/common_pa_debugprint$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_debugprint.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_debugprint$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_debugprint$(DependSuffix): src/common/pa_debugprint.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_debugprint$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_debugprint$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_debugprint.c"

$(IntermediateDirectory)/common_pa_debugprint$(PreprocessSuffix): src/common/pa_debugprint.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_debugprint$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_debugprint.c"

$(IntermediateDirectory)/common_pa_dither$(ObjectSuffix): src/common/pa_dither.c $(IntermediateDirectory)/common_pa_dither$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_dither.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_dither$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_dither$(DependSuffix): src/common/pa_dither.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_dither$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_dither$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_dither.c"

$(IntermediateDirectory)/common_pa_dither$(PreprocessSuffix): src/common/pa_dither.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_dither$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_dither.c"

$(IntermediateDirectory)/common_pa_front$(ObjectSuffix): src/common/pa_front.c $(IntermediateDirectory)/common_pa_front$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_front.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_front$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_front$(DependSuffix): src/common/pa_front.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_front$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_front$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_front.c"

$(IntermediateDirectory)/common_pa_front$(PreprocessSuffix): src/common/pa_front.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_front$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_front.c"

$(IntermediateDirectory)/common_pa_process$(ObjectSuffix): src/common/pa_process.c $(IntermediateDirectory)/common_pa_process$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_process.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_process$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_process$(DependSuffix): src/common/pa_process.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_process$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_process$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_process.c"

$(IntermediateDirectory)/common_pa_process$(PreprocessSuffix): src/common/pa_process.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_process$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_process.c"

$(IntermediateDirectory)/common_pa_ringbuffer$(ObjectSuffix): src/common/pa_ringbuffer.c $(IntermediateDirectory)/common_pa_ringbuffer$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_ringbuffer.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_ringbuffer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_ringbuffer$(DependSuffix): src/common/pa_ringbuffer.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_ringbuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_ringbuffer$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_ringbuffer.c"

$(IntermediateDirectory)/common_pa_ringbuffer$(PreprocessSuffix): src/common/pa_ringbuffer.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_ringbuffer$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_ringbuffer.c"

$(IntermediateDirectory)/common_pa_stream$(ObjectSuffix): src/common/pa_stream.c $(IntermediateDirectory)/common_pa_stream$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_stream.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_stream$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_stream$(DependSuffix): src/common/pa_stream.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_stream$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_stream$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_stream.c"

$(IntermediateDirectory)/common_pa_stream$(PreprocessSuffix): src/common/pa_stream.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_stream$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_stream.c"

$(IntermediateDirectory)/common_pa_trace$(ObjectSuffix): src/common/pa_trace.c $(IntermediateDirectory)/common_pa_trace$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_trace.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/common_pa_trace$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/common_pa_trace$(DependSuffix): src/common/pa_trace.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/common_pa_trace$(ObjectSuffix) -MF$(IntermediateDirectory)/common_pa_trace$(DependSuffix) -MM "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_trace.c"

$(IntermediateDirectory)/common_pa_trace$(PreprocessSuffix): src/common/pa_trace.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/common_pa_trace$(PreprocessSuffix) "C:/Users/wittend/Projects/audio/portaudio/src/common/pa_trace.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/win_pa_win_coinitialize$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_coinitialize$(DependSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_coinitialize$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_hostapis$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_hostapis$(DependSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_hostapis$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_util$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_util$(DependSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_util$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_waveformat$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_waveformat$(DependSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_waveformat$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_wdmks_utils$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_wdmks_utils$(DependSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_win_wdmks_utils$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_x86_plain_converters$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_x86_plain_converters$(DependSuffix)
	$(RM) $(IntermediateDirectory)/win_pa_x86_plain_converters$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dsound_pa_win_ds$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dsound_pa_win_ds$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dsound_pa_win_ds$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dsound_pa_win_ds_dynlink$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_allocation$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_allocation$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_allocation$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_converters$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_converters$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_converters$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_cpuload$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_cpuload$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_cpuload$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_debugprint$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_debugprint$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_debugprint$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_dither$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_dither$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_dither$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_front$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_front$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_front$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_process$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_process$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_process$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_ringbuffer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_ringbuffer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_ringbuffer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_stream$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_stream$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_stream$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_trace$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_trace$(DependSuffix)
	$(RM) $(IntermediateDirectory)/common_pa_trace$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile)
	$(RM) "C:\Users\wittend\Projects\audio\.build-release\portaudio"


