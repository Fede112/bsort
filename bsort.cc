#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cstring> // memcpy
#include <unistd.h>



unsigned char * RadixSort(unsigned char * pData, std::size_t count, std::size_t record_size, std::size_t key_size, std::size_t key_offset = 0)
{
    typedef uint8_t k_t [key_size];
    typedef uint8_t record_t [record_size];


    size_t mIndex[key_size][256] = {0};            /* index matrix */
    unsigned char * pTemp = new unsigned char [count*sizeof(record_t)];
    unsigned char * pDst, * pSrc;
    size_t i,j,m,n;
    k_t u;
    record_t v;

    for(i = 0; i < count; i++)                  /* generate histograms */
    {         
        std::memcpy(&u, (pData + record_size*i + key_offset), sizeof(u));
        
        for(j = 0; j < key_size; j++)
        {
            mIndex[j][(size_t)(u[j])]++;
            // mIndex[j][(size_t)(u & 0xff)]++;
            // u >>= 8;
        }       
    }
    for(j = 0; j < key_size; j++)             /* convert to indices */
    {
        n = 0;
        for(i = 0; i < 256; i++)
        {
            m = mIndex[j][i];
            mIndex[j][i] = n;
            n += m;
        }       
    }

    pDst = pTemp;                       /* radix sort */
    pSrc = pData;
    for(j = 0; j < key_size; j++)
    {
        for(i = 0; i < count; i++)
        {
            std::memcpy(&u, (pSrc + record_size*i + key_offset), sizeof(u));
            std::memcpy(&v, (pSrc + record_size*i), sizeof(v));
            m = (size_t)(u[j]);
            // m = (size_t)(u >> (j<<3)) & 0xff;
            std::memcpy(pDst + record_size*mIndex[j][m]++, &v, sizeof(v));
        }
        std::swap(pSrc, pDst);
        
    }
    delete[] pTemp;
    return(pSrc);
}

int main(int argc, char *argv[])
{
    int opt;
    unsigned int record_size{4};
    unsigned int key_size{4};
    unsigned int key_offset{0};
    std::string output{"outfile.bin"}; 

    while ((opt = getopt(argc, argv, "r:k:s:o:")) != -1) 
    {
        switch (opt) 
        {
        // case 'v':
          // verbosity += 1;
          // break;
        case 'r':
            record_size = atoi(optarg);
            break;
        case 'k':
            key_size = atoi(optarg);
            break;
        case 's':
            key_offset = atoi(optarg);
            break;
        case 'o':
            output = optarg;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-r record length in bytes] [-k key length in bytes] [-s key initial byte] [-o output filename] filename \n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

   if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }

   printf("name argument = %s\n", argv[optind]);


    std::ifstream infile (argv[optind], std::ifstream::binary);
    unsigned long int inbytes{0};
    unsigned long int lines;
    char * bufferA;
    if (infile)
    {
        infile.seekg (0, infile.end);
        inbytes = infile.tellg();
        infile.seekg (0, infile.beg);
        bufferA = new char [inbytes];
        lines = inbytes/record_size;

        std::cerr << "Reading " << inbytes << " characters... ";
        // read data as a block:
        infile.read (bufferA,inbytes);
        std::cerr << "all characters read successfully from " << argv[optind] << "\n";
    }
    else
    {
      std::cout << "error: only " << infile.gcount() << " could be read \n";
      return 1;
    }
    infile.close();

    // Radix serial sort
    RadixSort((unsigned char *) bufferA, lines, record_size, key_size, key_offset);

    // Print sorted output
    auto outfile = std::fstream(output, std::ios::out | std::ios::binary);
    outfile.write((char*)bufferA, inbytes);


    delete[] bufferA;
    outfile.close();
    // // Screen print data
    // unsigned int * p = (unsigned int *) bufferA;
    // for (unsigned int i = 0; i < lines; ++i)
    // {
    //     std::cout << p[2*i] << '\t' << p[2*i+1] << '\n';
    // }

    return 0;
}