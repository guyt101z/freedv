set(FREEDV_SOURCES
	fdmdv2_main.cpp
	fdmdv2_pa_wrapper.cpp
	fdmdv2_plot.cpp
	fdmdv2_plot_scalar.cpp
	fdmdv2_plot_scatter.cpp
	fdmdv2_plot_spectrum.cpp
	fdmdv2_plot_waterfall_linux.cpp
	dlg_about.cpp
	dlg_audiooptions.cpp
	dlg_comports.cpp
	dlg_filter.cpp
	sox_biquad.c
	topFrame.cpp
	varicode.c
	comp.h
	fdmdv2_defines.h
	fdmdv2_main.h
	fdmdv2_pa_wrapper.h
	fdmdv2_plot.h
	fdmdv2_plot_scalar.h
	fdmdv2_plot_scatter.h
	fdmdv2_plot_spectrum.h
	fdmdv2_plot_waterfall_linux.h
	dlg_about.h dlg_audiooptions.h
	dlg_comports.h
	dlg_filter.h
	sox.h
	sox_biquad.h
	topFrame.h
	varicode.h
	varicode_table.h
	version.h
)

add_executable(freedv ${FREEDV_SOURCES})
target_link_libraries(freedv ${FREEDV_LINK_LIBS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS freedv
	RUNTIME DESTINATION bin)
