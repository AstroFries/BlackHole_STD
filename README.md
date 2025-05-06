目前是测试阶段。
pull后请首先 git submodule update --init --recursive

为了生成glad:
在根目录下:
pip install glad \
python -m glad --profile core --api gl=3.3 --generator=c --out-path=external/glad

如果要运行测地线测试GEODESIC_TEST,请确保环境下安装了python，且对应python安装了numpy以及qt5agg
如果不想运行，可以在CMakeLists中把python那一部分删去。