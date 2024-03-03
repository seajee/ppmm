#include "ppmm.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>

ppmm::Image::Image(const std::string &filepath)
{
    m_FilePath = filepath;

    // Open input file stream
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        return;
    }

    // Check PPM magic
    std::string magic;
    ifs >> std::setw(2) >> magic;

    if (magic != "P3") {
        return;
    }

    // Read header
    ifs >> m_Width;
    ifs >> m_Height;
    ifs >> m_Depth;

    m_Size = m_Width * m_Height * m_Format;

    // Allocate memory for image data
    m_Data = (unsigned char*)malloc(sizeof(unsigned char) * m_Size);
    if (m_Data == NULL) {
        return;
    }

    // Read image data
    for (int i = 0; i < m_Size; i += m_Format) {
        int r, g, b;

        ifs >> r;
        ifs >> g;
        ifs >> b;

        m_Data[i + 0] = r;
        m_Data[i + 1] = g;
        m_Data[i + 2] = b;
    }

    // Close the file stream
    ifs.close();
}

ppmm::Image::Image(int width, int height, int depth)
{
    Initialize(width, height, depth);
}

ppmm::Image::Image()
{
}

ppmm::Image::~Image()
{
    free(m_Data);
}

bool ppmm::Image::Initialize(int width, int height, int depth)
{
    // Reset member variables
    m_Width = width;
    m_Height = height;
    m_Depth = depth;

    // Free memory for reinitialization
    if (m_Data != NULL) {
        free(m_Data);
    }

    m_Size = width * height * m_Format;

    // Allocate memory for image data
    m_Data = (unsigned char*)malloc(sizeof(unsigned char) * m_Size);
    if (m_Data == NULL) {
        return false;
    }

    // Fill image data with zeros
    memset(m_Data, 0, m_Size);

    return true;
}

bool ppmm::Image::IsOpen(void)
{
    return m_Data != NULL;
}

bool ppmm::Image::WriteToFile(const std::string &filepath)
{
    // Open output file stream
    std::ofstream ofs(filepath);
    if (!ofs.is_open()) {
        return false;
    }

    // Write PPM header
    ofs << "P3" << std::endl
        << m_Width << " " << m_Height << std::endl
        << m_Depth << std::endl;

    // Write image data
    for (int i = 0; i < m_Size; ++i) {
        ofs << (int)m_Data[i];

        // For every row of the image write a line break
        if (i % (m_Width*m_Format) == m_Width*m_Format - 1) {
            ofs << std::endl;
        } else {
            ofs << " ";
        }
    }

    // Close the file stream
    ofs.close();

    return true;
}

unsigned char& ppmm::Image::operator[](size_t index)
{
    return m_Data[index];
}

ppmm::Image& ppmm::Image::operator<<(unsigned char data)
{
    m_Data[m_Seek++] = data;
    return *this;
}

void ppmm::Image::Seek(size_t index)
{
    m_Seek = index;
}

int ppmm::Image::GetWidth(void)  { return m_Width;  }
int ppmm::Image::GetHeight(void) { return m_Height;  }
int ppmm::Image::GetFormat(void) { return m_Format; }
int ppmm::Image::GetSize(void)   { return m_Size;   }
int ppmm::Image::GetDepth(void)  { return m_Depth;  }

ppmm::Filter::Filter(Image &img) :
    m_Image(img)
{
    m_ImageSize = m_Image.GetSize();
    m_ImageFormat = m_Image.GetFormat();
    m_ImageDepth = m_Image.GetDepth();
}

void ppmm::Filter::Grayscale(void)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        float rl = (float)m_Image[i + 0] / m_ImageDepth;
        float gl = (float)m_Image[i + 1] / m_ImageDepth;
        float bl = (float)m_Image[i + 2] / m_ImageDepth;

        float y = 0.2126*rl + 0.7152*gl + 0.0722*bl;

        m_Image[i + 0] = (unsigned char)(y*m_ImageDepth);
        m_Image[i + 1] = (unsigned char)(y*m_ImageDepth);
        m_Image[i + 2] = (unsigned char)(y*m_ImageDepth);
    }
}

void ppmm::Filter::Inverted(void)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        unsigned char r = m_Image[i + 0];
        unsigned char g = m_Image[i + 1];
        unsigned char b = m_Image[i + 2];

        m_Image[i + 0] = m_ImageDepth - r;
        m_Image[i + 1] = m_ImageDepth - g;
        m_Image[i + 2] = m_ImageDepth - b;
    }
}

void ppmm::Filter::Multiply(float factor)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        float rl = (float)m_Image[i + 0] / m_ImageDepth;
        float gl = (float)m_Image[i + 1] / m_ImageDepth;
        float bl = (float)m_Image[i + 2] / m_ImageDepth;

        rl *= factor; if (rl > 1.0) rl = 1.0;
        gl *= factor; if (gl > 1.0) gl = 1.0;
        bl *= factor; if (bl > 1.0) bl = 1.0;

        m_Image[i + 0] = (unsigned char)(rl*m_ImageDepth);
        m_Image[i + 1] = (unsigned char)(gl*m_ImageDepth);
        m_Image[i + 2] = (unsigned char)(bl*m_ImageDepth);
    }
}

void ppmm::Filter::Multiply(float r, float g, float b)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        float rl = (float)m_Image[i + 0] / m_ImageDepth;
        float gl = (float)m_Image[i + 1] / m_ImageDepth;
        float bl = (float)m_Image[i + 2] / m_ImageDepth;

        rl *= r; if (rl > 1.0) rl = 1.0;
        gl *= g; if (gl > 1.0) gl = 1.0;
        bl *= b; if (bl > 1.0) bl = 1.0;

        m_Image[i + 0] = (unsigned char)(rl*m_ImageDepth);
        m_Image[i + 1] = (unsigned char)(gl*m_ImageDepth);
        m_Image[i + 2] = (unsigned char)(bl*m_ImageDepth);
    }
}

bool ppmm::RLECompression::WriteToFile(Image &img, const std::string &filepath)
{
    // Check if image is valid
    if (!img.IsOpen()) {
        return false;
    }

    // Open output file stream
    std::ofstream ofs(filepath);
    if (!ofs.is_open()) {
        return false;
    }

    int width = img.GetWidth();
    int format = img.GetFormat();

    // Write PPM header
    ofs << "P3" << std::endl
        << width << " " << img.GetHeight() << std::endl
        << img.GetDepth() << std::endl;

    // Perform RLE
    unsigned char cur = img[0];
    int count = 1;

    for (int i = 1; i < img.GetSize(); ++i) {
        bool space = false;

        if (img[i] != cur) {
            ofs << count << " " << (int)cur;
            cur = img[i];
            count = 1;

            // For every row of the image write a break line
            if (i % (width*format) == width*format - 1) {
                ofs << std::endl;
            }

            space = true;
        } else {
            ++count;
        }

        // Write spaces only when dumping rle sequences
        if (space) {
            ofs << " ";
        }
    }

    ofs << count << " " << (int)cur;

    ofs.close()

    return true;
}

bool ppmm::RLECompression::ReadFromFile(const std::string &filepath, Image &img)
{
    // Open input file stream
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        return false;
    }

    // Check PPM magic
    std::string magic;
    ifs >> std::setw(2) >> magic;

    if (magic != "P3") {
        return false;
    }

    int width;
    int height;
    int depth;

    // Read header
    ifs >> width;
    ifs >> height;
    ifs >> depth;

    // Reinitialize image
    if (!img.Initialize(width, height, depth)) {
        return false;
    }

    // Read image data while decompressing it
    int count;
    int ptr = 0;

    while (ifs >> count) {
        int which;
        ifs >> which;

        for (int i = 0; i < count; ++i) {
            img[ptr] = (unsigned char)which;
            ++ptr;
        }
    }

    // Close the file stream
    ifs.close();

    return true;
}
