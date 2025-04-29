目前是测试阶段。
pull后请首先 git submodule update --init --recursive

为了生成glad:
在根目录下:
pip install glad \
python -m glad --profile core --api gl=3.3 --generator=c --out-path=external/glad