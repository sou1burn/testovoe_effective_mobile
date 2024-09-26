#include "BMP.h"



void BmpReader::openBMP(const std::string& filename)
{
    bmp_file.open(filename, std::ios::binary);
    
    if (!bmp_file)
    {
        throw std::runtime_error("File not opened\n");
    }
    
    bmp_file.read((char*)(&file_header), sizeof(BmpFileHeader));

    if (file_header.file_type != 0x4D42)
    {
        throw std::runtime_error("File is not .bmp\n");
    }

    bmp_file.read((char*)(&info_header), sizeof(BmpInfoHeader));

    //unused part
    /*if (info_header.bit_count == 32)
    {
        if (info_header.size >= (sizeof(BmpInfoHeader) + sizeof(BmpColorHeader)))
        {
            bmp_file.read((char*)(&color_header), sizeof(BmpColorHeader));

            check_color_header(color_header);
        }
        else
        {

        }
    }*/

    std::cout << "File Type: " << std::hex << file_header.file_type << std::endl;
    std::cout << "File Size: " << file_header.file_size << std::endl;
    std::cout << "Offset: " << file_header.offset << std::endl;
    std::cout << "Info Header Size: " << info_header.size << std::endl;
    std::cout << "Width: " << info_header.width << std::endl;
    std::cout << "Height: " << info_header.height << std::endl;
    std::cout << "Bit Count: " << info_header.bit_count << std::endl;
    std::cout << "Compression: " << info_header.compression << std::endl;


    if (info_header.bit_count != 24 && info_header.bit_count != 32)
    {
        throw std::runtime_error("Unsupported format\n");
    }


    int data_size = file_header.file_size - file_header.offset;

    pixel_data.resize(data_size);

    bmp_file.seekg(file_header.offset, std::ios::beg);

    bmp_file.read(reinterpret_cast<char*>(pixel_data.data()), data_size);

}

void BmpReader::displayBMP() const
{
    
    std::string output = "output.txt";
    std::ofstream out(output);

    if (!out.is_open())
    {
        throw std::runtime_error("Output file not opened!\n");
    }

    int height = info_header.height;
    int width = info_header.width;

    int bit_count = info_header.bit_count / 8;
    int padding = ((4 - (width * bit_count)) % 4) % 4;

    for (int y = height;  y >= 0; --y)
    {
        for (int x = 0 ; x < width; ++x)
        {
            int pixel_idx = y * ((width * bit_count) + padding) + (x * bit_count);
            uint8_t blue = pixel_data[pixel_idx];
            uint8_t green = pixel_data[pixel_idx + 1];
            uint8_t red = pixel_data[pixel_idx + 2];
            
            if (isBlack(red, green, blue))
            {
                out << " # ";
                std::cout << " # ";
            }
            else if (red == 255 && green == 255 && blue == 255)
            {
                out << " / ";
                std::cout << " / ";
            }
            else
            {
                out << " . ";
                std::cout << " . ";
            }
        }
        out << "\n";
        std::cout << "\n";
    }
    out.close();


#ifdef _WIN32
    system("start notepad.exe output.txt");
#else
    //it crashes so dont mention it
    //system("nano output.txt");
#endif

}


void BmpReader::closeBMP()
{
    if (bmp_file.is_open())
    {
        bmp_file.close();
    }
}

BmpReader::~BmpReader()
{
    closeBMP();
}


//unused
void BmpReader::check_color_header(BmpColorHeader& color_header) const
{
        BmpColorHeader expected_color_header;
        if(expected_color_header.red_mask != color_header.red_mask ||
            expected_color_header.blue_mask != color_header.blue_mask ||
            expected_color_header.green_mask != color_header.green_mask ||
            expected_color_header.alpha_mask != color_header.alpha_mask)
        {

            throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
        }
        
        if(expected_color_header.color_space_type != color_header.color_space_type) 
        {
            throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
        }
}


bool isBlack(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t threshold = 36;
    return red < threshold && green < threshold && blue < threshold;
}
