#include "ppmm.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

ppmm::Image::Image(const std::string &filepath)
{
    m_FilePath = filepath;

    // Open input file stream
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        return;
    }

    // Check PPM magic
    char magic_cstr[3] = { 0 };
    ifs.read(magic_cstr, 2);
    std::string magic(magic_cstr);

    if (magic != "P3") {
        return;
    }

    // Read header
    ifs >> m_Width;
    ifs >> m_Height;
    ifs >> m_Depth;

    m_Size = m_Width * m_Height * m_Format;

    // Allocate memory for image data
    m_Data = (unsigned char*)malloc(
            sizeof(unsigned char) * m_Size);
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

ppmm::Image::~Image()
{
    free(m_Data);
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
        << "255" << std::endl;

    // Write image data
    for (int i = 0; i < m_Width*m_Height*m_Format; ++i) {
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

bool ppmm::Image::Write(void)
{
    return WriteToFile(m_FilePath);
}

unsigned char& ppmm::Image::operator[](size_t index)
{
    return m_Data[index];
}

int ppmm::Image::GetWidth(void)  { return m_Width;  }
int ppmm::Image::GetHeight(void) { return m_Width;  }
int ppmm::Image::GetFormat(void) { return m_Format; }
int ppmm::Image::GetSize(void)   { return m_Size;   }

ppmm::Filter::Filter(Image &img) :
    m_Image(img)
{
    m_ImageSize = m_Image.GetSize();
    m_ImageFormat = m_Image.GetFormat();
}

void ppmm::Filter::Grayscale(void)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        float rl = (float)m_Image[i + 0] / 255.0;
        float gl = (float)m_Image[i + 1] / 255.0;
        float bl = (float)m_Image[i + 2] / 255.0;

        float y = 0.2126*rl + 0.7152*gl + 0.0722*bl;

        m_Image[i + 0] = (unsigned char)(y*255);
        m_Image[i + 1] = (unsigned char)(y*255);
        m_Image[i + 2] = (unsigned char)(y*255);
    }
}

void ppmm::Filter::Inverted(void)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        unsigned char r = m_Image[i + 0];
        unsigned char g = m_Image[i + 1];
        unsigned char b = m_Image[i + 2];

        m_Image[i + 0] = 255 - r;
        m_Image[i + 1] = 255 - g;
        m_Image[i + 2] = 255 - b;
    }
}

void ppmm::Filter::Multiply(float factor)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        float rl = (float)m_Image[i + 0] / 255.0;
        float gl = (float)m_Image[i + 1] / 255.0;
        float bl = (float)m_Image[i + 2] / 255.0;

        rl *= factor; if (rl > 1.0) rl = 1.0;
        gl *= factor; if (gl > 1.0) gl = 1.0;
        bl *= factor; if (bl > 1.0) bl = 1.0;

        m_Image[i + 0] = (unsigned char)(rl*255);
        m_Image[i + 1] = (unsigned char)(gl*255);
        m_Image[i + 2] = (unsigned char)(bl*255);
    }
}

void ppmm::Filter::Multiply(float r, float g, float b)
{
    for (int i = 0; i < m_ImageSize; i += m_ImageFormat) {
        float rl = (float)m_Image[i + 0] / 255.0;
        float gl = (float)m_Image[i + 1] / 255.0;
        float bl = (float)m_Image[i + 2] / 255.0;

        rl *= r; if (rl > 1.0) rl = 1.0;
        gl *= g; if (gl > 1.0) gl = 1.0;
        bl *= b; if (bl > 1.0) bl = 1.0;

        m_Image[i + 0] = (unsigned char)(rl*255);
        m_Image[i + 1] = (unsigned char)(gl*255);
        m_Image[i + 2] = (unsigned char)(bl*255);
    }
}
