# BAT.man

This tool converts an EXE to a .bat file, which uses base64 for the program binary data. A C++ program (main.cpp) pushes the data to the .bat file. In runtime the batch program uses certutil to regenerate the exe/PE version of the raw data stored in lines of code.

## Drag & Drop

- Just drop a valid PE / .exe onto the program in the same folder in file explorer. The resulting .bat file will be in dist/ folder

## CLI

- If you don't want to drag and drop...

```
./main ./programs/helloworld-64bit.exe
```

BAT.man also generates a file 'Run.vbs' in dist/ which can be double-clicked from explorer to open the program, if you prefer that - you would need to modify the VBS file or otherwise package the .bat alongside it for this to work long-term.

## Compiling Example

```
g++ main.cpp -o main -static

./main programs/helloworld-64bit.exe

./dist/man.bat
```
