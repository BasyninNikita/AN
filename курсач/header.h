#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include<vector>
#include <experimental\filesystem>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std::experimental::filesystem;

class ilogger
{
public:
    virtual void log(const std::string& message) = 0;
    virtual ~ilogger() {;}
};

class antivirus_scaner  {

public:
    ilogger *logger;
    int counter=0;
    std::vector<std::string> virus_strings;

    void set_logger(ilogger *_logger)
    {
        logger = _logger;
    }

    void read_data_of_dangerous_files() {
        std::ifstream fin("data.txt");
        std::string str;

        while (std::getline(fin, str)) {
            virus_strings.push_back(str);
        }
        fin.close();
    }

    void move_file(std::string filname, std::string name) {
        ++counter;
        const char * from, *to;
        from = filname.c_str();
        name = "carantin\\" + name;
        to = name.c_str();
        rename(from, to);

        //if (logger != NULL)
            logger->log("File has been moved in carantin \n");
        //std::cout << "File has been moved in carantin" << std::endl;
    }

    void checking_dyrectory(const path & checking) {
        path name_of_directory = checking;
        for (const directory_entry& x : directory_iterator{ name_of_directory }) {
            if (is_directory(x.path())) {
                checking_dyrectory(x.path());
            }
            else {
                std::string name = x.path().stem().generic_string(), extension = x.path().extension().generic_string();
                if (extension == ".exe" || extension == ".rar") {
                    if (is_dangerous(x.path().generic_string())) {
                        move_file(x.path().generic_string(), x.path().filename().generic_string());
                    }
                }
            }
        }
    }

    bool checking_file(std::string & buf) {

        for (auto i : virus_strings) {
            if (buf.find(i) != std::string::npos) {
                logger->log("This file is dangerous! \n");
                //std::cout << "This file is dangerous!" << std::endl;
                return true;
            }
        }
        logger->log("No problem with this file" );
        //std::cout << "No problem with this file" << std::endl;
        return false;
    }

    bool is_dangerous(std::string name) {
        bool succsess = false;
        std::ifstream ifile(name, std::ofstream::binary);

        if (ifile.is_open()) {
            std::string string="\n " + name + " is cheking now... \n";
            logger->log(string);
            //std::cout << std::endl << name << " is checking now..." << std::endl;

            ifile.seekg(0, ifile.end);
            int length = ifile.tellg();
            ifile.seekg(0, ifile.beg);
            char * buffer = new char[++length];
            buffer[length - 1] = '\0';
            ifile.read((char*)buffer, length);

            std::string str;
            for (auto i = 0; i < length; ++i) {
                str += buffer[i];
            }

            succsess = checking_file(str);

            delete[] buffer;
            ifile.close();
        }
        else {
            //std::cout << std::endl << name << " has been already deleted or does not exist" << std::endl;
            std::string string="\n " + name + " has been already deleted or does not exist \n";
            logger->log(string);
        }
        return succsess;
    }

    std::wstring to_LPCWSTR(const std::string& s)
    {
        int len;
        int slength = (int)s.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }

    void check_registry() {
        HKEY hKey;
        std::vector< std::pair<std::string, std::string> > strings_of_registry;

        if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), NULL, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
        {
            TCHAR lpData[1024] = { 0 };
            TCHAR data[1024] = { 0 };
            std::string str1, str2;
            DWORD buffersize = sizeof(lpData);
            DWORD num, MaxValueNameLen;

            RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &num, &MaxValueNameLen, NULL, NULL, NULL);

            for (DWORD i = 0; i < num; i++) {
                buffersize = MaxValueNameLen + 1;
                lpData[0] = '\0';
                data[0] = '\0';
                RegEnumValue(hKey, i, lpData, &buffersize, NULL, NULL, NULL, NULL);

                buffersize = 256;
                RegQueryValueEx(hKey, lpData, NULL, NULL, (LPBYTE)data, &buffersize);

                for (auto i = 0; lpData[i] != '\0'; ++i) {
                    str1 += lpData[i];
                }

                auto index = 0;
                if (data[0] == '\"') {
                    index = 1;
                }
                while (data[index] != '.' || data[index + 1] != 'e' || data[index + 2] != 'x' || data[index + 3] != 'e') {
                    str2 += data[index++];
                }
                str2 += data[index];
                str2 += data[index + 1];
                str2 += data[index + 2];
                str2 += data[index + 3];

                strings_of_registry.push_back(std::make_pair(str1, str2));
                str1.clear();
                str2.clear();
            }
        }

        for (auto i : strings_of_registry) {
            std::string str="\n Value: " + i.first + "\n Key: " + i.second + "\n";
            logger->log(str);
            //std::cout << std::endl << "Value: " << i.first << std::endl <<
              //  "Key: " << i.second;
            if (is_dangerous(i.second)) {

                std::wstring stemp1 = to_LPCWSTR(i.first);
                std::wstring stemp2 = to_LPCWSTR(i.second);
                LPCWSTR str1 = stemp1.c_str();
                LPCWSTR str2 = stemp2.c_str();

                RegDeleteValue(hKey, str1);
                RegDeleteKey(hKey, str2);
                logger->log("This record has been deleted \n");
                //std::cout << "This record has been deleted" << std::endl;

                move_file(path(i.second).generic_string(), path(i.second).filename().generic_string());
            }
        }

        RegCloseKey(hKey);
    }

public:
    int counter_()
    {
       return this->counter;
    }
    void new_count()
    {
        counter=0;
    }
    antivirus_scaner() {
        read_data_of_dangerous_files();
    }

    void scan_filesystem(std::string str) {
        checking_dyrectory(str);
    }
    void registry()
    {
       check_registry();
    }
};
