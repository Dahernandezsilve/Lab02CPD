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
 * Curso: Data Science
 * Sección: 10
 *
 * Guatemala, 1 de septiembre de 2024
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <omp.h>

#define INFILE "numeros_generados_paralelos.csv"
#define OUTFILE "numeros_ordenados_paralelos.csv"

using namespace std;

void generateRandomNumbersOnFile(int N) {
    ofstream writeNumbers(INFILE);
    if (!writeNumbers) {
        cerr << "🚨 Error creating file " << INFILE << endl;
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int randomNumber = rand() % (N / 2) + 1;
        #pragma omp critical
        {
            writeNumbers << randomNumber;
            if (i < N - 1) {
                writeNumbers << ",";
            }
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

    #pragma omp task shared(data)
    quicksort(data, lo, h);

    #pragma omp task shared(data)
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

    double startTime, endTime;

    // Generación de números aleatorios
    printf("\n🗂️ Generating random list with %d elements...\n", N);
    startTime = omp_get_wtime();
    generateRandomNumbersOnFile(N);
    endTime = omp_get_wtime();
    printf("\n⏱️  Time for generating random numbers: %.6f seconds\n", endTime - startTime);

    // Lectura de números
    printf("\n📖 Reading numbers from file...\n");
    int* Array = readNumbersFromFile(N);

    // Ordenamiento con quicksort paralelo
    printf("\n📚 Sorting numbers with parallel quicksort...\n");
    startTime = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        quicksort(Array, 0, N - 1);
    }
    endTime = omp_get_wtime();

    printf("\n- First Elements: %d, %d, %d\n", Array[0], Array[1], Array[2]);
    printf("- Medium Elements: %d, %d, %d\n", Array[N / 4], Array[N / 4 + 1], Array[N / 4 + 2]);
    printf("- Last Elements: %d, %d, %d\n", Array[N - 3], Array[N - 2], Array[N - 1]);
    printf("\n⏱️  Time for sort ing numbers: %.6f seconds\n", endTime - startTime);

    // Escritura de números ordenados
    printf("\n📝 Writing sorted numbers to file...\n");
    writeSortedNumbersToFile(Array, N);

    printf("\n✅ Sorted numbers written to file %s\n", OUTFILE);
    delete[] Array;

    return 0;
}
