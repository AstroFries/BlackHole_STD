#include "OpenGLRenderer.h"
#include "bloom.h"
#include <windows.h>
#include <vector>
#include <chrono>

int main() {
    
    const int width = 800;
    const int height = 600;
    OpenGLRenderer renderer(width, height, "OpenGL Renderer");
    std::vector<unsigned char> pixels(width * height * 3, 0);
    std::vector<float> pixels_d(width * height * 3, 0);

    Bloom bloomer(width, height, &pixels_d, &pixels);
    for (int i = 0; i < width * height * 3; i += 3) {
        float x = (float)(i/3 % width) / width;
        float y = (float)(i/3 / width) / width;
        if ((x-0.5)*(x-0.5)+(y-0.5)*(y-0.5) > 0.05*0.05){
            pixels_d[i] = 35;     // R
            pixels_d[i + 1] = 35;   // G
            pixels_d[i + 2] = 35;   // B
        }else{
            pixels_d[i] = 265;     // R
            pixels_d[i + 1] = 265;   // G
            pixels_d[i + 2] = 265;   // B
        }
    }
    bloomer.direct();
    int k = 0;
    float max_p = 0;
    
    //renderer.updatePixels(pixels);
    //renderer.render();
    //system("pause");
    renderer.updatePixels(pixels);
    renderer.render();
    //system("pause");
    auto start = std::chrono::high_resolution_clock::now();
    bloomer.bloom_gauss(12, 1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    printf("bloom_gauss_1 spent: %.3f ms\n", elapsed.count());

    renderer.updatePixels(pixels);
    renderer.render();

    start = std::chrono::high_resolution_clock::now();
    bloomer.bloom_gauss(250, 2.5);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    printf("bloom_gauss_2 spent: %.3f ms\n", elapsed.count());
    renderer.updatePixels(pixels);
    renderer.render();
    system("pause");
    return 0;
}