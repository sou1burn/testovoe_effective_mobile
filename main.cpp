#include "BMP.h"


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        throw std::runtime_error("Too less args \n");
    }

    BmpReader Reader;

    try
    {
        Reader.openBMP(argv[1]);
        Reader.displayBMP();
        Reader.closeBMP();
    }
    catch (std::runtime_error& e){
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
    
}