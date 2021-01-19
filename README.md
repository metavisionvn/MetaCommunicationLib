# MtVL Meta vision library
Vision library multi-platform.
Based on
- OpenCV 4.2.0
- QT 5.12.8
- Eigen 3.3.7

## Build on Raspberry
- QMAKE_CXXFLAGS_RELEASE += -mfpu=neon
- QMAKE_CFLAGS_RELEASE += -mfpu=neon

Enable SIMD function
