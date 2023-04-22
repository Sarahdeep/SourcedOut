//
// Created by march on 21.04.2023.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>
#include <set>

#include <boost/tokenizer.hpp>


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

        else if (modif[i] != '\0')
            res += modif[i];
        else{
            res += '\n';
        }
    }
    return res;
}

std::vector <std::string> tbm_tokenizer(const std::string &text){
    boost::char_separator <char> sep(" {}();,\"\0\'");
    std::vector <std::string> res;
    boost::tokenizer < boost::char_separator <char> > tokens(text, sep);

    for (const std::string &s: tokens) {
        if (!s.empty() && s[0] != '\n' && s[0] != '\0'){
            res.push_back(s);
        }
    }
    return res;
}

// % = intersection(A, B) / union(A, B)
double Jaccard_metric(const std::vector <std::string>& tokens1, const std::vector <std::string>& tokens2){
    std::set <std::string> s1;
    std::set <std::string> s2;

    for (auto &i : tokens1) s1.insert(i);
    for (auto &i : tokens2) s2.insert(i);


    std::set<std::string> intersect_sets;
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                     std::inserter(intersect_sets, intersect_sets.begin()));

    std::set<std::string> union_sets;
    set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
                     std::inserter(union_sets, union_sets.begin()));

    std::cout << intersect_sets.size() << " " << union_sets.size() << std::endl;

    return static_cast<double> (intersect_sets.size()) / static_cast<double> (union_sets.size());
}

double Livenstain_dist(std::vector<std::string> tokens1, std::vector <std::string> tokens2){
    unsigned long n = tokens1.size();
    unsigned long m = tokens2.size();
    int x, y, z;

    std::vector <std::vector <int> > lev (n, std::vector <int> (m, 0));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (std::min(i, j) == 0){
                lev[i][j] = std::max(i, j);
            }
            else{
                x = lev[i-1][j];
                y = lev[i][j-1];
                z = lev[i-1][j-1];
                lev[i][j] = std::min(x, std::min(y, z));
                if (tokens1[i] != tokens2[j]){
                    lev[i][j]++;
                }
            }
        }
    }

    return lev[n-1][m-1];
}

std::pair<double, double> textCompare(std::istream& fin1, std::istream& fin2){
    std::string line;

    std::string text1( (std::istreambuf_iterator<char>(fin1) ),
                      (std::istreambuf_iterator<char>()    ) );

    std::string text2( (std::istreambuf_iterator<char>(fin2) ),
                      (std::istreambuf_iterator<char>()    ) );

    std::string non_comm_text1 = deleteComms(text1);
    std::string non_comm_text2 = deleteComms(text2);

    std::vector <std::string> tokens1 = tbm_tokenizer(non_comm_text1);
    std::vector <std::string> tokens2 = tbm_tokenizer(non_comm_text2);

    double res1 = Jaccard_metric(tokens1, tokens2);
    double res2 = 1 - Livenstain_dist(tokens1, tokens2) / std::max(tokens1.size(), tokens2.size());

    return {res1, res2};
}

int main(){

    std::ifstream fin1;
    fin1.open("text-basic-metrics/code1.txt");
    assert(fin1.is_open());

    std::ifstream fin2;
    fin2.open("text-basic-metrics/code2.txt");
    assert(fin2.is_open());

    std::pair <double, double> metrics_res = textCompare(fin1, fin2);

    std::cout << "Jaccard metric "<< metrics_res.first << "\nLivenstein distance: " << metrics_res.second;
    fin1.close();
    fin2.close();
}