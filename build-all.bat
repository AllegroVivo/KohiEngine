@ECHO OFF
REM Build everything

ECHO "Building Everything..."

@REM PUSHD engine
@REM CALL build.bat
@REM POPD
@REM IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

@REM PUSHD testbed
@REM CALL build.bat
@REM POPD
@REM IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Engine
make -f "Makefile.engine.windows.mak" all
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Testbed
make -f "Makefile.testbed.windows.mak" all
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Tests
make -f "Makefile.tests.windows.mak" all
if %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully!"
@REM PAUSE