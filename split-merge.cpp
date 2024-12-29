#include <iostream>
#include <fstream>
#include <string>
#include <vector>


/* the size of the parts in MB's*/
const long long int buffMemSize{ 1024 * 1024 * 512 };

using namespace std;

void splitFile(string& inputFile, long long int partSize) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) {
        cerr << "Cannot open file." << endl;
        return;
    }

    inFile.seekg(0, ios::end);
    long long int fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);

    /* Part count */
    long long int partCount = (fileSize + partSize - 1) / partSize;  

    for (int i = 0; i < partCount; ++i) {
        string partFileName = inputFile + ".part" + to_string(i + 1) + ".dat";
        ofstream outFile(partFileName, ios::binary);
        if (!outFile) {
            cerr << "Unable to create part file" << i + 1 << endl;
            return;
        }

        vector<char> buffer(partSize);
        inFile.read(buffer.data(), partSize);
        outFile.write(buffer.data(), inFile.gcount());

        cout << "Part " << partFileName << " is written. (" << i + 1 << ")" << endl;
    }

    inFile.close();
}

void mergeFiles(string& outputFile, string& inputFilePrefix, long long int partCount) {
    ofstream outFile(outputFile, ios::binary);
    if (!outFile) {
        cerr << "Cannot open main file" << endl;
        return;
    }

    for ( long long int i = 0; i < partCount; ++i) {
        string partFileName = inputFilePrefix + ".part" + to_string(i + 1) + ".dat";
        ifstream inFile(partFileName, ios::binary);
        if (!inFile) {
            cerr << "Cannot open part file: " << partFileName << endl;
            return;
        }

        /* Write to the main file */
        vector<char> buffer(buffMemSize); 
        while (inFile.read(buffer.data(), buffer.size()) || inFile.gcount() > 0) {
            outFile.write(buffer.data(), inFile.gcount());
        }

        inFile.close();
        cout << "Part  " << partFileName << " is merged." << endl;
    }

    outFile.close();
}

int main() {
    string inputFile{ "data.img" };
    string outputFile{ "mergeddata.img" };
    long long int partSize{ 1024 * 1024 * 512 }; 
    long long int partCount{ 5 };
    
    splitFile(inputFile, partSize);



    mergeFiles(outputFile, inputFile, partCount);
    return 0;
}
// 1