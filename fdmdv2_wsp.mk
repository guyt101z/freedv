.PHONY: clean All

All:
	@echo ----------Building project:[ test_sndfile - Release ]----------
	@cd "libsndfile\Projects\w32-mingw" && "mingw32-make.exe"  -j 2 -f "test_sndfile.mk"
clean:
	@echo ----------Cleaning project:[ test_sndfile - Release ]----------
	@cd "libsndfile\Projects\w32-mingw" && "mingw32-make.exe"  -j 2 -f "test_sndfile.mk" clean
