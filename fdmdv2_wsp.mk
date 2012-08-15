.PHONY: clean All

All:
	@echo ----------Building project:[ pa_enum - Release ]----------
	@cd "pa_enum" && "mingw32-make.exe"  -j 2 -f "pa_enum.mk"
clean:
	@echo ----------Cleaning project:[ pa_enum - Release ]----------
	@cd "pa_enum" && "mingw32-make.exe"  -j 2 -f "pa_enum.mk" clean
