#include "bloom.h"
#include <iostream>
#include <algorithm>
Bloom::Bloom(){
    pixels_ = nullptr;
}

Bloom::Bloom(int width, int height, std::vector<float>* power, std::vector<unsigned char>* pixels){
    init(width, height, power, pixels);
}

void Bloom::init(int width, int height, std::vector<float>* power, std::vector<unsigned char>* pixels){
    if (pixels == nullptr)throw("pixels == nullptr!");
    pixels_ = pixels;
    width_ = width;
    height_ = height;
    power_ = power;
    if (width_ * height_ * 3!= pixels_->size())throw("width * height dont match with pixels.size()!");
}

void Bloom::bloom_gauss(float r, float u){
    float normalize_k = 0.564 / r;
    std::vector<float> buffer1,buffer2;
    buffer1.resize(width_ * height_ * 3,0);
    buffer2.resize(width_ * height_ * 3,0);
    for (int i = 0; i < width_ * height_ ; ++i){
        float u = std::max(std::max((*power_)[3 * i],(*power_)[3 * i + 1]),(*power_)[3 * i + 1]);
        if (u > 230){
            for (int k = 0; k < 3; ++k)buffer1[3 * i + k] = (*power_)[3 * i + k];
        }else {
            for (int k = 0; k < 3; ++k)buffer1[3 * i + k] = (*power_)[3 * i + k] * pow(u/230,3);
        }
    }
    //std::cout << 1 <<std::endl;
    for (int x = 0; x < width_; ++x){for (int y = 0; y < height_; ++y){
        for (int k = 0; k < 3; ++k){
            if (buffer1[3 * x + 3 * y * width_ + k] < 1)continue;
            for (int i = std::max(x - (int)(2*r),0); i < std::min(x + (int)(2*r),width_); ++i){
                buffer2[3 * i + 3 * y * width_ + k] += 
                    (normalize_k * buffer1[3 * x + 3 * y * width_ + k] * pow(2.72,-pow(i-x,2)/r/r));
            }
        }
    }}
    
    buffer1.resize(width_ * height_ * 3,0);
    
    for (int x = 0; x < width_; ++x){for (int y = 0; y < height_; ++y){
        for (int k = 0; k < 3; ++k){
            if (buffer2[3 * x + 3 * y * width_ + k] < 1)continue;
            for (int j = std::max(y - (int)(2*r),0); j < std::min(y + (int)(2*r),height_); ++j){
                buffer1[3 * x + 3 * j * width_ + k] += 
                    (normalize_k * buffer2[3 * x + 3 * y * width_ + k] * pow(2.72,-pow(j-y,2)/r/r));
            }
        }
    }}
    
    for (int i = 0 ; i < width_ * height_ * 3; ++i){
        (*power_)[i] += u*buffer1[i]; 
        (*pixels_)[i] = (unsigned char)std::clamp((*power_)[i], 0.0f, 255.0f);
    }
}

void Bloom::direct(){
    for (int i = 0 ; i < width_ * height_ * 3; ++i){
        (*pixels_)[i] = (unsigned char)std::clamp((*power_)[i], 0.0f, 255.0f);
    }
}
