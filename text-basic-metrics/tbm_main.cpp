//
// Created by march on 21.04.2023.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>


std::string deleteComms(const std::string& text){
    std::string modif;
    std::string res;

    std::stringstream ss;
    std::string line;

    ss << text;

    while(getline(ss, line)){
        line.pop_back();
        line.push_back('\0');
        modif += line;
    }
    std::cout << "modif: \n" << modif << std::endl;
//    modif = text;

    bool s_comm = false;
    bool m_comm = false;

    for (int i = 0; i < modif.size(); i++){
        if (s_comm && modif[i] == '\0')
            s_comm = false;
        else if  (m_comm && modif[i] == '*' && modif[i + 1] == '/')
            m_comm = false,  i++;
        else if (s_comm || m_comm)
            continue;
        else if (modif[i] == '/' && modif[i+1] == '/')
            s_comm = true, i++;
        else if (modif[i] == '/' && modif[i+1] == '*')
            m_comm = true,  i++;

        else if (modif[i] == '\0')
            res += '\n';
        else{
            res += modif[i];
        }
    }
    return res;
}

int main(){
    std::ifstream fin;
    fin.open("text-basic-metrics/code1.txt");
    assert(fin.is_open());

    std::string line;
    std::string text( (std::istreambuf_iterator<char>(fin) ),
                    (std::istreambuf_iterator<char>()    ) );
//    while(getline(fin, line)){
//        std::cout << text.size() << std::endl;
//        text += line;
//    }
    fin.close();

    std::cout << text;

    std::string non_comm_text = deleteComms(text);

    std::cout << non_comm_text;
}