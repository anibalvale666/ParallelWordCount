// WordCount.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.

#include <iostream>
#include <thread>  
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <cctype>
#include <chrono>
#include <string>

using namespace std;

std::vector<std::string> allwords;
//vector<pair<string, int>> wordCount;
ifstream file;

bool lectura()
{
    int tam_vec = 100000000;

    string word;
    allwords.clear();
    int i = 0;

    while (file >> word && i < tam_vec)
    {
        i++;
        allwords.push_back(word);
        //allwords[i++] = word;
    }

    std::cout << "cantidad de palabras " << allwords.size() << endl;
    std::cout << "tamanio del vector en bytes " << sizeof(string) * 94000000 << endl;
    return allwords.empty();
}


std::map<std::string, size_t> contar(std::vector<std::string> const& words)
{
    auto wordCount = std::map<std::string, size_t>{};
    for (auto const& word : words)
    {
        ++wordCount[word];
    }

    std::cout << "palabras contadas" << wordCount.size() << endl;
    return wordCount;
}


auto wordCount = std::map<std::string, size_t>{};
std::map<std::string, size_t> localWorldCount[6];
void contarP( int ini, int end,int localIte)
{
   
    string word;
    for (unsigned it = ini;it<end;it++)
    {
        word = allwords[it];
        //++wordCount[word];
        ++localWorldCount[localIte][word];
    }

    std::cout << "palabras contadas" << localWorldCount[localIte].size() << endl;
    //res = wordCount;
    return;
}


void print(std::map<std::string, size_t> mymap)
{
    // show content:
    for (std::map<string, size_t>::iterator it = mymap.begin(); it != mymap.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n'; 
}

void save(string arc)
{
    ofstream file;
    file.open(arc);
    //std::cout << "palabras csssontadas" << wordCount.size() << endl;
    for (std::map<string, size_t>::iterator it = wordCount.begin(); it != wordCount.end(); ++it)
    {
        //std::cout << it->first << " => " << it->second << '\n';
        file << it->first << " => " << it->second << '\n';

    }
    file.close();
}

  
int main()
{
    //inicializamos allwords
    //for (int i = 0; i < 50000000; i++)
        //allwords.push_back("");

    //file.open("texto_prueba2.txt");
    file.open("C:/Users/nibar/Documents/texto5G.txt");

    bool finish = true;
    unsigned iteration=0;
    double principal,principal_lect;
    principal = principal_lect = 0;


    while (true)
    {
        //----------------------------------lectura---------------------------------------//
        auto start_lect = std::chrono::steady_clock::now();
        //finish = lectura("texto_prueba2.txt");
        finish = lectura();
        auto end_lect = std::chrono::steady_clock::now();
 
        if (finish) break; //condicion de termino 


        std::cout << "iteracion nro: " << iteration++ << endl;

        //------------------------------creacion de hilos-------------------------------------------//
        //const auto numThread = std::thread::hardware_concurrency();
        //int part_per_thread = allwords.size() /numThread;
        unsigned numTread = 6;
        int part_per_thread = allwords.size() / numTread;
        std::vector<std::thread> threads;
        auto start_p = std::chrono::steady_clock::now();

        std::map<std::string, size_t> localWorldCount[6];


        unsigned i;
        for (i = 0; i < numTread - 1; i++)
        {
            std::cout << i * part_per_thread << " " << (i + 1) * part_per_thread << endl;
            threads.push_back(std::thread(contarP, i * part_per_thread, (i + 1) * part_per_thread, i));
        }
        std::cout << i * part_per_thread << " " << allwords.size() << endl;
        threads.push_back(std::thread(contarP, i * part_per_thread, allwords.size(), i));
        //threads.push_back(std::thread(contar, allwords));
        for (auto& th : threads) th.join();
        threads.clear();


        auto end_p = std::chrono::steady_clock::now();
        //contadas = contar(allwords);
        //print(contadas);
        std::cout << "lectura de todas las palabras: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_lect - start_lect).count() << "ms\n"
            << "procesamiento principal: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_p - start_p).count() << "ms\n\n\n\n";

        principal += std::chrono::duration_cast<std::chrono::milliseconds>(end_p - start_p).count();
        principal_lect += std::chrono::duration_cast<std::chrono::milliseconds>(end_lect - start_lect).count();
    }

    for (int ii = 0; ii < 6; ii++)
        for (std::map<string, size_t>::iterator jj = localWorldCount[ii].begin(); jj != localWorldCount[ii].end(); ++jj)
            wordCount[jj->first] += jj->second;

    save("out.txt");
        std::cout << "lectura total de todas las palabras: " << principal_lect << "ms\n"
            << "procesamiento principal total: " << principal << "ms\n\n\n";

    file.close();

    return 0;
}


