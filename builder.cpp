#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

void createRCFile(const string& rcName, const string& iconName) {
    ofstream rc(rcName);
    rc << "1 ICON \"" << iconName << "\"\n";
    rc.close();
}

int main() {
    cout << "Important (No .cpp extension and no spaces in filenames)\n";
    cout << "How to use: Put this program in the same folder as your cpp file then run this\n\n";

    cout << "Enter C++ Filename: ";
    string filename;
    getline(cin, filename);

    cout << "Enter output .exe filename (or /= for same as source): ";
    string exeFilename;
    getline(cin, exeFilename);

    cout << "Enter logo filename (ICO only) if any, else leave blank: ";
    string logo;
    getline(cin, logo);

    if (exeFilename == "/=") exeFilename = filename;
    string resourceO = "";
    if (!logo.empty()) {
        string iconFile = logo;
        if (logo.size() >= 4 && logo.substr(logo.size() - 4) == ".png") {
            iconFile = filename + "_icon.ico";
            string convertCmd = "magick convert \"" + logo + "\" -define icon:auto-resize=256,128,64,48,32,16 \"" + iconFile + "\"";
            cout << "Converting PNG to ICO...\n";
            system(convertCmd.c_str());
        }
        string rcFile = "temp.rc";
        createRCFile(rcFile, iconFile);

        resourceO = "resource.o";
        string windresCmd = "windres \"" + rcFile + "\" -O coff -o \"" + resourceO + "\"";
        cout << "Creating resource object...\n";
        system(windresCmd.c_str());
    }

    string command = "g++ -static -static-libgcc -static-libstdc++ -o \"" + exeFilename + ".exe\" \"" + filename + ".cpp\"";
    if (!resourceO.empty()) command += " \"" + resourceO + "\"";

    cout << "\nCompiling...\n";
    int status = system(command.c_str());

    if (status == 0) {
        cout << filename << ".cpp has been Successfully Compiled as " << exeFilename << ".exe\n";
    } else {
        cout << filename << ".cpp has been Unsuccessfully Compiled as " << exeFilename << ".exe\n";
    }

    system("pause");
    return 0;
}
