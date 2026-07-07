#include "HCTree.hpp"
#include "Helper.hpp"
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    //parse command line args
    if (argc != 3) {
        cerr << "Sorry! Invalid arguments." << endl;
        return 1;
    }

    //open input file to read
    FancyInputStream in(argv[1]);
    if (!in.good()) {
        cerr << "Sorry! Invalid input file!" << endl;
        return 1;
    }

    //empty compressed file
    if (in.filesize() == 0) {
        FancyOutputStream out (argv[2]);
        return 0;
    }

    //read header
    int uniqueCharacters = in.read_int();
    vector<int> counts(256, 0);
    long totalBytes = 0;

    //read symbol-freq pairs
    for (int i = 0; i < uniqueCharacters; i++) {
        unsigned char symbol = in.read_byte();
        int count = in.read_int();
        counts[symbol] = count;
        totalBytes += count;
    }

    //reconstruct tree using counts
    HCTree tree;
    if (uniqueCharacters > 0) {
        tree.build(counts);
    }

    //open output file for writing
    FancyOutputStream out((argv[2]));

    //decode bits to bytes
    for (long i = 0; i < totalBytes; i++) {
        unsigned char symbol = tree.decode(in);
        out.write_byte(symbol);
    }

    return 0;
} 
