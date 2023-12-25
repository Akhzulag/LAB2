#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <random>
#include <bitset>

using namespace std;

void coutByteToBits(const string& s)
{
    for (unsigned char byte : s) {
        std::cout << bitset<8>(byte);
    }
    std::cout<<"\n";
}

void coutByteToHex(const string& bytes)
{
    for (uint8_t byte : bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) ;
    }
    std::cout<<"\n";
}

std::string sha224(const std::string& input, int n) {
    unsigned char hash[SHA224_DIGEST_LENGTH];
    SHA256_CTX sha256;

    // Initialize SHA-256 context for SHA-224
    SHA224_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA224_Final(hash, &sha256);
    string res = std::string(reinterpret_cast<char*>(hash), SHA224_DIGEST_LENGTH);
    //  coutByteToHex(res);
    res = res.substr(res.length() - n);
    return res;
}

std::string sha224Full(const std::string& input, int n) {
    unsigned char hash[SHA224_DIGEST_LENGTH];
    SHA256_CTX sha256;

    // Initialize SHA-256 context for SHA-224
    SHA224_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA224_Final(hash, &sha256);
    string res = std::string(reinterpret_cast<char*>(hash), SHA224_DIGEST_LENGTH);

    return res;
}

std::string generateRandomBytes(int numBytes) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);

    std::string result;
    result.reserve(numBytes);

    for (int i = 0; i < numBytes; ++i) {
        result.push_back(dis(gen));
    }

    return result;
}

bool gen = false;
string r = "";
void R(std::string& x,int n)
{
    if(gen == false)
    {
        r = generateRandomBytes((128/8-n));
        gen = true;
    }
    x = r + x;
}
#include <chrono>
#include <omp.h>
#include <unordered_map>
#include <map>

vector<tuple<string,string>> buildTablePrecalculation(int K, int L, int n)
{

    vector<tuple<string,string>> arr(K);

    #pragma omp parallel for
        for(int i = 0; i < K; ++i)
        {
            string xi0 =  generateRandomBytes(n);
            string xij = xi0;
            for(int j = 0; j < L; ++j)
            {
                R(xij,n);
                xij = sha224(xij,n);
            }
            // coutByteToBits(xi0);
            // coutByteToBits(xij);
            arr[i] = make_tuple(xi0,xij);
            //if(i % 10000 == 0)
            //std::cout<<"i: "<<i<<"\n";
        }
        return arr;
}

string buildAttack(vector<tuple<string,string>>& table, unordered_map<string, int>& index, string ha, int K, int L, int n, int& countY)
{
    string y = ha;

    for(int j = 0; j < L; ++j)
    {
        auto it = index.find(y);
        if(it != index.end())
        {
            int i = index[y];
            string x = get<0>(table[i]);
            for(int m = 0; m < L - j - 1; ++m)
            {
                R(x,n);
                x = sha224(x,n);
                string x1 = x;

            }
            R(x,n);
            return x;
        }
        R(y,n);
        y = sha224(y,n);
        ++countY;
    }

    return "PROBLEMS";
}

void results(int K, int L, int n)
{
    std::cout<<"K: "<<K<<"; "<<"L: "<<L<<"\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    auto table = buildTablePrecalculation(K,L,n);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Time taken by function: " <<duration.count()<<" "<<double( duration.count())/double(60000000) << " microseconds" << std::endl;
    std::cout<<"TABLE BUILDED\n";
    unordered_map<string, int> index;
    for(int i = 0; i < K; ++i)
    {
        index[get<1>(table[i])] = i;
    }

    int s = 0;
    vector<int> results(10000,0);
    start_time = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for(int i = 0; i < 10000; ++i)
    {
        //std::cout<<std::dec<<i<<"\n";
        int count = 0;
        string random = generateRandomBytes(256/8);
        string has = sha224(random,n);
        string x = buildAttack(table,index,has,K,L,n,count);
        if( x != "PROBLEMS")
        {
            if(sha224(x,n) == has)
            {
                results[i] += 1;
                std::cout<<"g = ";coutByteToHex(random);
                std::cout<<"h(g) = ";coutByteToHex(sha224Full(random,n));
                std::cout<<"x = ";coutByteToHex(x);
                std::cout<<"sha_224(x) = ";coutByteToHex(sha224Full(x,n));
                std::cout<<std::dec<<"\n"<<"кількість різних значень y:"<< count;
                std::cout<<"\n";
            }
        }

    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Time taken by function: " <<duration.count()<<" "<<double( duration.count())/double(60000000) << " microseconds" << std::endl;
    std::cout<<"RESULTS:\n";
    for(auto i: results)
        s += i;
    std::cout<<"K: "<<K<<"; "<<"L: "<<L;
    std::cout<<std::dec<<"\n"<<"кількість успіхів "<<s<<"\n"<<(double)s/double(10000) * 100<<"%";

    std::cout<<"\n";
}

int main() {

    //  string has = sha224("abc",32/8);
    //  coutByteToHex(has);


//
    int n = 32 /8;
//
    int K00 = pow(2,20);
    int L00 = pow(2,10);

    results(K00, L00,n);
//
//    int K01 = pow(2,20);
//    int L01 = pow(2,11);
//
//    results(K01, L01,n);
//
//    int K02 = pow(2,20);
//    int L02 = pow(2,12);
//
//    results(K02, L02,n);
//
//    int K10 = pow(2,22);
//    int L10 = pow(2,10);
//
//    results(K10, L10,n);
//
//    int K11 = pow(2,22);
//    int L11 = pow(2,11);
//
//    results(K11, L11,n);
//
//    int K12 = pow(2,22);
//    int L12 = pow(2,12);
//
//    results(K12, L12,n);

//    int K20 = pow(2,24);
//    int L20 = pow(2,10);
//
//    results(K20, L20,n);
//
//    int K21 = pow(2,24);
//    int L21 = pow(2,11);
//
//    results(K21, L21,n);
//
//    int K22 = pow(2,24);
//    int L22 = pow(2,12);
//
//    results(K22, L22,n);


    return 0;
}

