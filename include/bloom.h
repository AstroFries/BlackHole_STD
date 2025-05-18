#include <vector>

class Bloom{
  public:
    Bloom();
    Bloom(int, int, std::vector<float>* power, std::vector<unsigned char>* pixels);
    void init(int,int,std::vector<float>* power, std::vector<unsigned char>* pixels);
    void bloom_gauss(float,float);
    void direct();
  private:
    int width_,height_;
    std::vector<unsigned char>* pixels_;
    std::vector<float>* power_;
};