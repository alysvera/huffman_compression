#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main (int argc, char* argv[]) {

    //parse command lines args
    if (argc != 3) {
        cerr << "Sorry! Invalid arguments." << endl;
        return 1;
    }

    //open input file to read
    FancyInputStream in (argv[1]);
    if (!in.good()) {
        cerr << "Sorry! Invalid input file." << endl;
        return 1;
    }

    //empty file case
    if (in.filesize() == 0) {
        FancyOutputStream out(argv[2]);
        return 0;
    }

    //reading bytes and counting frequencies
    vector<int> counts(256, 0);
    int nextByte;
    int uniqueCharacters = 0;
    while ((nextByte = in.read_byte()) != -1) {
        if (counts[nextByte] == 0) {
            uniqueCharacters++;
        }
        counts[nextByte]++;
    }

    //build huffman tree
    HCTree tree;
    tree.build(counts);

    //open output file
    FancyOutputStream out(argv[2]);

    //number of unique characters
    out.write_int(uniqueCharacters);

    //writing symbol-freq pairs
    for (int i = 0; i < 256; i++) {
        if (counts[i] > 0) {
            out.write_byte((unsigned char)i);
            out.write_int(counts[i]);
        }
    }

    //moves back to beginning of input file
    in.reset();

    //translating bytes to bits and writing output
    while ((nextByte = in.read_byte()) != -1) {
        tree.encode((unsigned char)nextByte, out);
    }

    //files will close automatically
    return 0;
}