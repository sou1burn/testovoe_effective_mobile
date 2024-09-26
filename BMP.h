#pragma once
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <exception>
#include<cstdlib>

#ifndef BMP_h
#define Bmp_h

#pragma pack(push, 1)
struct BmpFileHeader
{
    uint16_t file_type {0x4D42};
    uint32_t file_size {0};
    uint16_t reserved1 {0};
    uint16_t reserved2 {0};
    uint32_t offset {0};
};


struct BmpInfoHeader 
{
    uint32_t size {0};
    int32_t width {0};
    int32_t height {0};

    uint16_t planes {1};
    uint16_t bit_count {0};
    uint32_t compression {0};
    uint32_t image_size {0};
    int32_t x_pixels_per_meter {0};
    int32_t y_pixels_per_meter {0};
    uint32_t colors_used {0};
    uint32_t colors_important {0};
};


struct BmpColorHeader {
    uint32_t red_mask{ 0x00ff0000 };         
    uint32_t green_mask{ 0x0000ff00 };       
    uint32_t blue_mask{ 0x000000ff };        
    uint32_t alpha_mask{ 0xff000000 };       
    uint32_t color_space_type{ 0x73524742 }; 
    uint32_t unused[16]{ 0 };
};

#pragma pack(pop)

class BmpReader
{
private:
    std::ifstream bmp_file;
    BmpFileHeader file_header;
    BmpInfoHeader info_header;
    BmpColorHeader color_header;
    std::vector<uint8_t> pixel_data;

public:
    ~BmpReader();
    void openBMP(const std::string& filename);
    void displayBMP() const;
    void closeBMP();
    void check_color_header(BmpColorHeader &color_header) const;
};

bool isBlack(uint8_t red, uint8_t green, uint8_t blue);
#endif