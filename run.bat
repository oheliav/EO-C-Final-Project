@echo off

if "%1" == "" (
    echo Usage: %0 file1 [file2 ...]
    echo Please provide at least one file name as an argument.
    pause
    exit /b
)

echo Compiling the program...
gcc main.c preAssembler.c firstPass.c secondPass.c globals.c dataStructures.c util.c errors.c bitUtils.c -o assembler -mconsole
if %errorlevel% neq 0 (
    echo Compilation failed. Please check for errors.
    pause
    exit /b
)

echo Running the assembler on the provided files...

:loop
if "%1" == "" goto end
    echo Processing file: %1.asm
    assembler.exe %1
    if %errorlevel% neq 0 (
        echo Assembler failed to run on %1.asm. Please check for errors.
    ) else (
        echo Program ran successfully for %1.asm.
    )
    shift
goto loop

:end
echo All files processed.
pause
