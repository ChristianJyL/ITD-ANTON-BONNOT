#include "include/file.hpp"
#include "include/utils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::string load_str_File(std::filesystem::path const& path){
    std::ifstream file {make_absolute_path(path)};
    if (!file.is_open()) {
        std::cout << "Could not open file: " << path << std::endl;
        throw std::runtime_error{"Could not open file"};
    }
    std::string content;
    std::string line;

    while (getline(file, line)){
        if (line[0] != '#'){
            content += line + "\n";
        }    
    }
    file.close();
    return content;
}


bool isValid_ITD(std::string ITDText){
    std::string line;
    std::istringstream stream(ITDText); // Transform the string into a stream
    int count = 0;
    while (getline(stream,line)){
        if (count==0){
            if (line != "ITD"){
                return false;
            }
        }
        if (count==1){
            //map ????.png/.jpeg
            std::string map = line.substr(0,3);
            if (map != "map"){
                return false;
            }
            std::string map_extension = line.substr(line.size()-4,4);
            auto it = std::find(map_extension.begin(), map_extension.end(), is_img);
            if (it == map_extension.end()){
                return false;
            }

        }
        if (count==2){
            //path 255 255 255
            std::string path = line.substr(0,4);
            if (path != "path"){
                return false;
            }
            std::string path_color = line.substr(5,3);
            if (path_color.size() != 3){
                return false;
            }
            for (char c : path_color){
                if (!isdigit(c)){
                    return false;
                }
            }
        }

    }
    
}