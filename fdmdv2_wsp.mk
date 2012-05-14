.PHONY: clean All

All:
	@echo ----------Building project:[ test_hamlib - Release ]----------
	@cd "hamlib-3.0\test_hamlib" && "mingw32-make.exe"  -j 2 -f "test_hamlib.mk"
clean:
	@echo ----------Cleaning project:[ test_hamlib - Release ]----------
	@cd "hamlib-3.0\test_hamlib" && "mingw32-make.exe"  -j 2 -f "test_hamlib.mk" clean
