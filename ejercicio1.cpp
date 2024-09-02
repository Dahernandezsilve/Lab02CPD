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

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>

#define INFILE "numeros_generados.csv"
#define OUTFILE "numeros_ordenados.csv"

using namespace std;

void generateRandomNumbersOnFile(int N) {
    ofstream writeNumbers(INFILE);
    if (!writeNumbers) {
        cerr << "🚨 Error creating file " << INFILE << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        writeNumbers << rand() % (N / 2) + 1;
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
        while ((data[l] - p) < 0) l++;
        while ((data[h] - p) > 0) h--;
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

    printf("\n🗂️ Generating random list with %d elements...\n", N);
    generateRandomNumbersOnFile(N);

    printf("\n📖 Reading numbers from file...\n");
    int* Array = readNumbersFromFile(N);

    printf("\n📚 Sorting numbers with qsort...\n");
    quicksort(Array, 0, N - 1);

    printf("\n- First Elements: %d, %d, %d\n", Array[0], Array[1], Array[2]);
    printf("- Medium Elements: %d, %d, %d\n", Array[N / 4], Array[N / 4 + 1], Array[N / 4 + 2]);
    printf("- Last Elements: %d, %d, %d\n", Array[N - 3], Array[N - 2], Array[N - 1]);

    printf("\n📝 Writing sorted numbers to file...\n");
    writeSortedNumbersToFile(Array, N);

    printf("\n✅ Sorted numbers written to file %s\n", OUTFILE);
    delete[] Array;

    return 0;
}
