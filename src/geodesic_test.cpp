#include "geodesic.h"
#include "matplotlibcpp.h"
#include <iostream>

int main(){
    //std::cout << "Hello, Geodesic!" << std::endl;
    Py_Initialize();
    PyRun_SimpleString("import matplotlib; matplotlib.use('Qt5Agg')");
    PyRun_SimpleString("import matplotlib; print('Current backend:', matplotlib.get_backend())");

    // ׼������
    std::vector<double> x, y;
    for (int i = 0; i < 100; ++i) {
        x.push_back(i * 0.1);
        y.push_back(std::sin(i * 0.1));
    }

    // ����ͼ��
    matplotlibcpp::plot(x, y, "b-"); // ��ɫʵ��
    matplotlibcpp::title("Simple Sine Wave");
    matplotlibcpp::xlabel("X");
    matplotlibcpp::ylabel("sin(X)");

    // ��ʾͼ��
    matplotlibcpp::show();

    // ���� Python ����
    Py_Finalize();

    return 0;
}