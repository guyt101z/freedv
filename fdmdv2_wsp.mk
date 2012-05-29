.PHONY: clean All

All:
	@echo ----------Building project:[ fdmdv2 - Release ]----------
	@"mingw32-make.exe"  -j 2 -f "fdmdv2.mk"
clean:
	@echo ----------Cleaning project:[ fdmdv2 - Release ]----------
	@"mingw32-make.exe"  -j 2 -f "fdmdv2.mk" clean
