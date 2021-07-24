
#include <exception>
#include "GLWindow.h"


constinit int windowWidth = 800;
constinit int windowHeight = 800;




constinit float attributeArray1[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// 左下
                                    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,// 右上
                                    0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,// 左上
                                    0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f// 右下
};
constinit float attributeArray[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 左下
                                    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// 右上
                                    0.0f, 0.5f, 0.0f, 0.0f, 1.0f,// 左上
                                    0.5f, 0.0f, 1.0f, 1.0f, 1.0f// 右下
};
constinit uint EBOArray[] = {
        2, 1, 0,
        0, 1, 3
};

int main(const int argc, const char *argv[]) {
    try {
        Ace::GLWindow window(800, 600, "computer graphics");
        window.generateVao();
        window.generateEBO(EBOArray);
        window.generateVBO(attributeArray, {{0, 2},
                                            {1, 3}});

        window.loadShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl", "config/variablesLocation.json");
//        window.load2DTexture("Images/brick.png");
        window.setBackgroundColor(0.0f, 1.0f, 1.0f, 0.0f);
        window.render();
    } catch (std::exception &e) {
        return 1;
    }
    return 0;
}