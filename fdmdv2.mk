##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=fdmdv2
ConfigurationName      :=Release
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "C:\Users\wittend\Projects\Radio\fdmdv2"
ProjectPath            := "C:\Users\wittend\Projects\Radio\fdmdv2"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=wittend
Date                   :=5/28/2012
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)__WX__ 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="C:\Users\wittend\Projects\Radio\fdmdv2\fdmdv2.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
CmpOptions             := -O2 -Wall $(shell wx-config --cxxflags --unicode=yes --debug=no) $(Preprocessors)
C_CmpOptions           := -O2 -Wall $(shell wx-config --cxxflags --unicode=yes --debug=no) $(Preprocessors)
LinkOptions            :=  -mwindows -s $(shell wx-config --debug=no --libs --unicode=yes)
IncludePath            :=  $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)sndfile-1 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)./libsndfile/bin 


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
WXWIN:=C:\bin\wxWidgets-2.9.2
PATH:=$(WXWIN)\lib\gcc_dll;$(PATH)
WXCFG:=gcc_dll\mswu
UNIT_TEST_PP_SRC_DIR:=C:\bin\UnitTest++-1.3
Objects=$(IntermediateDirectory)/topFrame$(ObjectSuffix) $(IntermediateDirectory)/dlg_about$(ObjectSuffix) $(IntermediateDirectory)/dlg_audio$(ObjectSuffix) $(IntermediateDirectory)/dlg_options$(ObjectSuffix) $(IntermediateDirectory)/dlg_comports$(ObjectSuffix) $(IntermediateDirectory)/fdmdv2_plot$(ObjectSuffix) $(IntermediateDirectory)/fdmdv2_main$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/topFrame$(ObjectSuffix): topFrame.cpp $(IntermediateDirectory)/topFrame$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "C:/Users/wittend/Projects/Radio/fdmdv2/topFrame.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/topFrame$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/topFrame$(DependSuffix): topFrame.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/topFrame$(ObjectSuffix) -MF$(IntermediateDirectory)/topFrame$(DependSuffix) -MM "C:/Users/wittend/Projects/Radio/fdmdv2/topFrame.cpp"

$(IntermediateDirectory)/topFrame$(PreprocessSuffix): topFrame.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/topFrame$(PreprocessSuffix) "C:/Users/wittend/Projects/Radio/fdmdv2/topFrame.cpp"

$(IntermediateDirectory)/dlg_about$(ObjectSuffix): dlg_about.cpp $(IntermediateDirectory)/dlg_about$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_about.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/dlg_about$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlg_about$(DependSuffix): dlg_about.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlg_about$(ObjectSuffix) -MF$(IntermediateDirectory)/dlg_about$(DependSuffix) -MM "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_about.cpp"

$(IntermediateDirectory)/dlg_about$(PreprocessSuffix): dlg_about.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlg_about$(PreprocessSuffix) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_about.cpp"

$(IntermediateDirectory)/dlg_audio$(ObjectSuffix): dlg_audio.cpp $(IntermediateDirectory)/dlg_audio$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_audio.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/dlg_audio$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlg_audio$(DependSuffix): dlg_audio.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlg_audio$(ObjectSuffix) -MF$(IntermediateDirectory)/dlg_audio$(DependSuffix) -MM "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_audio.cpp"

$(IntermediateDirectory)/dlg_audio$(PreprocessSuffix): dlg_audio.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlg_audio$(PreprocessSuffix) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_audio.cpp"

$(IntermediateDirectory)/dlg_options$(ObjectSuffix): dlg_options.cpp $(IntermediateDirectory)/dlg_options$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_options.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/dlg_options$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlg_options$(DependSuffix): dlg_options.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlg_options$(ObjectSuffix) -MF$(IntermediateDirectory)/dlg_options$(DependSuffix) -MM "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_options.cpp"

$(IntermediateDirectory)/dlg_options$(PreprocessSuffix): dlg_options.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlg_options$(PreprocessSuffix) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_options.cpp"

$(IntermediateDirectory)/dlg_comports$(ObjectSuffix): dlg_comports.cpp $(IntermediateDirectory)/dlg_comports$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_comports.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/dlg_comports$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/dlg_comports$(DependSuffix): dlg_comports.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dlg_comports$(ObjectSuffix) -MF$(IntermediateDirectory)/dlg_comports$(DependSuffix) -MM "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_comports.cpp"

$(IntermediateDirectory)/dlg_comports$(PreprocessSuffix): dlg_comports.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dlg_comports$(PreprocessSuffix) "C:/Users/wittend/Projects/Radio/fdmdv2/dlg_comports.cpp"

$(IntermediateDirectory)/fdmdv2_plot$(ObjectSuffix): fdmdv2_plot.cpp $(IntermediateDirectory)/fdmdv2_plot$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "C:/Users/wittend/Projects/Radio/fdmdv2/fdmdv2_plot.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/fdmdv2_plot$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fdmdv2_plot$(DependSuffix): fdmdv2_plot.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fdmdv2_plot$(ObjectSuffix) -MF$(IntermediateDirectory)/fdmdv2_plot$(DependSuffix) -MM "C:/Users/wittend/Projects/Radio/fdmdv2/fdmdv2_plot.cpp"

$(IntermediateDirectory)/fdmdv2_plot$(PreprocessSuffix): fdmdv2_plot.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fdmdv2_plot$(PreprocessSuffix) "C:/Users/wittend/Projects/Radio/fdmdv2/fdmdv2_plot.cpp"

$(IntermediateDirectory)/fdmdv2_main$(ObjectSuffix): fdmdv2_main.cpp $(IntermediateDirectory)/fdmdv2_main$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "C:/Users/wittend/Projects/Radio/fdmdv2/fdmdv2_main.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/fdmdv2_main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fdmdv2_main$(DependSuffix): fdmdv2_main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fdmdv2_main$(ObjectSuffix) -MF$(IntermediateDirectory)/fdmdv2_main$(DependSuffix) -MM "C:/Users/wittend/Projects/Radio/fdmdv2/fdmdv2_main.cpp"

$(IntermediateDirectory)/fdmdv2_main$(PreprocessSuffix): fdmdv2_main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fdmdv2_main$(PreprocessSuffix) "C:/Users/wittend/Projects/Radio/fdmdv2/fdmdv2_main.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/topFrame$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/topFrame$(DependSuffix)
	$(RM) $(IntermediateDirectory)/topFrame$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dlg_about$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dlg_about$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dlg_about$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dlg_audio$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dlg_audio$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dlg_audio$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dlg_options$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dlg_options$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dlg_options$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/dlg_comports$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/dlg_comports$(DependSuffix)
	$(RM) $(IntermediateDirectory)/dlg_comports$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/fdmdv2_plot$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/fdmdv2_plot$(DependSuffix)
	$(RM) $(IntermediateDirectory)/fdmdv2_plot$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/fdmdv2_main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/fdmdv2_main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/fdmdv2_main$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) "C:\Users\wittend\Projects\Radio\fdmdv2\.build-release\fdmdv2"


