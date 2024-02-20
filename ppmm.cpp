#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

namespace ppmm
{
    class Image
    {
    public:
        Image(const std::string &filepath)
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

        bool IsOpen(void)
        {
            return m_Data != NULL;
        }

        bool WriteToFile(const std::string &filepath)
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

        bool Write(void)
        {
            return WriteToFile(m_FilePath);
        }

        unsigned char& operator[](size_t index)
        {
            return m_Data[index];
        }

        int GetWidth(void)  { return m_Width;  }
        int GetHeight(void) { return m_Width;  }
        int GetFormat(void) { return m_Format; }
        int GetSize(void)   { return m_Size;   }

        ~Image()
        {
            free(m_Data);
        }

    private:
        std::string m_FilePath;

        unsigned char *m_Data = NULL;
        int m_Size = 0;
        int m_Width = 0;
        int m_Height = 0;
        int m_Format = 3;
        int m_Depth = 0;
    };

    class Filter
    {
    public:
        Filter(Image &img) :
            m_Image(img)
        {
            m_ImageSize = m_Image.GetSize();
            m_ImageFormat = m_Image.GetFormat();
        }

        void Grayscale(void)
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

        void Inverted(void)
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

    private:
        Image &m_Image;
        int m_ImageSize = 0;
        int m_ImageFormat = 0;
    };
}

int main(void)
{
    using namespace ppmm;

    Image img("cat.ppm");
    if (!img.IsOpen()) {
        std::cerr << "ERROR: Could not open file" << std::endl;
        return 1;
    }

    Filter filter(img);

    filter.Grayscale();

    if (!img.WriteToFile("cat_grayscale.ppm")) {
        std::cerr << "ERROR: Could not write file" << std::endl;
        return 1;
    }

    filter.Inverted();

    if (!img.WriteToFile("cat_inverted.ppm")) {
        std::cerr << "ERROR: Could not write file" << std::endl;
        return 1;
    }

    return 0;
}
