#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

// 이미지 데이터를 램(CPU)에 올리기 위한 클래스

CLASS_PTR(Image)
class Image 
{
public:
    static ImageUPtr Load(const std::string& filepath);
    ~Image();

    const uint8_t* GetData() const { return m_data; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetChannelCount() const { return m_channelCount; }

private:
    Image() {};
    bool LoadWithStb(const std::string& filepath);
    int m_width { 0 };
    int m_height { 0 };
    int m_channelCount { 0 };

    // m_width * m_height * channel 만큼의 길이를 가짐, 왜냐? 인덱스 하나당 RGB값 중 하나를 표현
    uint8_t* m_data { nullptr };
};

#endif // __IMAGE_H__