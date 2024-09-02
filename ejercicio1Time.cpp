/******************************************************************************
 * Universidad del Valle de Guatemala
 * 18 Avenida 11-95, Zona 15 Guatemala
 * Facultad de Ingeniería
 * Departamento de Computación
 *
 * Laboratorio #2
 *
 * Integrantes:
 * Diego Alexander Hernández Silvestre, 21270
 * Linda Inés Jiménez Vides, 21169
 *
 * Curso: Computación Paralela y Distribuida
 * Sección: 10
 *
 * Guatemala, 1 de septiembre de 2024
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <random>

#define INFILE "numeros_generados.csv"
#define OUTFILE "numeros_ordenados.csv"

using namespace std;
using namespace std::chrono;

void generateRandomNumbersOnFile(int N) {
    vector<int> Array(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, N / 2);

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
            Array[i] = dis(gen);
        }
    }

    ofstream writeNumbers(INFILE);
    if (!writeNumbers) {
        cerr << "🚨 Error creating file " << INFILE << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        writeNumbers << Array[i];
        if (i < N - 1) {
            writeNumbers << ",";
        }
    }
    writeNumbers << endl;
    writeNumbers.close();
}

void quicksort(int* data, int lo, int hi) {
    if (lo >= hi) return;
    int l = lo;
    int h = hi;
    int p = data[(hi + lo) / 2];
    while (l <= h) {
        while (data[l] < p) l++;
        while (data[h] > p) h--;
        if (l <= h) {
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++;
            h--;
        }
    }
    quicksort(data, lo, h);
    quicksort(data, l, hi);
}

int* readNumbersFromFile(int N) {
    ifstream readNumbers(INFILE);
    if (!readNumbers) {
        cerr << "🚨 Error reading file " << INFILE << endl;
        exit(EXIT_FAILURE);
    }
    int* Array = new int[N];
    string line;
    if (getline(readNumbers, line)) {
        stringstream ss(line);
        string number;
        int i = 0;
        while (getline(ss, number, ',')) {
            Array[i++] = stoi(number);
        }
    }
    readNumbers.close();
    return Array;
}

void writeSortedNumbersToFile(int* Array, int N) {
    ofstream writeSortedNumbers(OUTFILE);
    if (!writeSortedNumbers) {
        cerr << "🚨 Error creating file " << OUTFILE << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        writeSortedNumbers << Array[i];
        if (i < N - 1) {
            writeSortedNumbers << ",";
        }
    }
    writeSortedNumbers << endl;
    writeSortedNumbers.close();
}

int main(int argc, char* argv[]) {
    int N;
    cout << "📝 Input a quantity of random numbers to generate:  ";
    cin >> N;

    auto start = high_resolution_clock::now();
    cout << "\n🗂️ Generating random list with " << N << " elements...\n";
    generateRandomNumbersOnFile(N);
    auto end = high_resolution_clock::now();
    duration<double> generateDuration = end - start;
    cout << "\n⏱️  Random list generated in " << generateDuration.count() << " seconds\n";

    start = high_resolution_clock::now();
    cout << "\n📖 Reading numbers from file...\n";
    int* Array = readNumbersFromFile(N);
    end = high_resolution_clock::now();
    duration<double> readDuration = end - start;
    cout << "\n⏱️  Numbers read in " << readDuration.count() << " seconds\n";

    start = high_resolution_clock::now();
    cout << "\n📚 Sorting numbers with quicksort...\n";
    quicksort(Array, 0, N - 1);
    end = high_resolution_clock::now();
    duration<double> sortDuration = end - start;
    cout << "\n- First Elements: " << Array[0] << ", " << Array[1] << ", " << Array[2] << "\n";
    cout << "- Medium Elements: " << Array[N / 4] << ", " << Array[N / 4 + 1] << ", " << Array[N / 4 + 2] << "\n";
    cout << "- Last Elements: " << Array[N - 3] << ", " << Array[N - 2] << ", " << Array[N - 1] << "\n";
    cout << "\n⏱️  Numbers sorted in " << sortDuration.count() << " seconds\n";

    cout << "\n📝 Writing sorted numbers to file...\n";
    writeSortedNumbersToFile(Array, N);

    cout << "\n✅ Sorted numbers written to file " << OUTFILE << "\n";
    delete[] Array;

    return 0;
}
