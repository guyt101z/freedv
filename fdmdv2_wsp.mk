.PHONY: clean All

All:
	@echo ----------Building project:[ pa_cppbinding_test - Release ]----------
	@cd "pa_cppbinding_test" && "mingw32-make.exe"  -j 2 -f "pa_cppbinding_test.mk"
clean:
	@echo ----------Cleaning project:[ pa_cppbinding_test - Release ]----------
	@cd "pa_cppbinding_test" && "mingw32-make.exe"  -j 2 -f "pa_cppbinding_test.mk" clean
