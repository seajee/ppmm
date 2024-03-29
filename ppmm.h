#pragma once

#include <string>

namespace ppmm
{
    class Image
    {
    public:
        Image(const std::string &filepath);
        Image(int width, int height, int depth);
        Image();
        ~Image();

        bool Initialize(int width, int height, int depth);
        bool IsOpen(void);
        bool WriteToFile(const std::string &filepath);

        unsigned char& operator[](size_t index);
        Image& operator<<(unsigned char data);

        void Seek(size_t index);

        int GetWidth(void);
        int GetHeight(void);
        int GetFormat(void);
        int GetSize(void);
        int GetDepth(void);

    private:
        std::string m_FilePath;

        unsigned char *m_Data = NULL;
        int m_Width = 0;
        int m_Height = 0;
        int m_Format = 3;
        int m_Size = 0;
        int m_Depth = 0;

        size_t m_Seek = 0;
    };

    class Filter
    {
    public:
        Filter(Image &img);

        void Grayscale(void);
        void Inverted(void);
        void Multiply(float factor);
        void Multiply(float r, float g, float b);

    private:
        Image &m_Image;
        int m_ImageSize = 0;
        int m_ImageFormat = 0;
        int m_ImageDepth = 0;
    };

    class RLECompression
    {
    public:
        bool WriteToFile(Image &img, const std::string &filepath);
        bool ReadFromFile(const std::string &filepath, Image &img);
    };
}
