
#include <exception>
#include "GLWindow.h"


constinit int windowWidth = 1080;
constinit int windowHeight = 1080;


constinit float attributeArray[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,// 左下
                                    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,// 右上
                                    0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,// 左上
                                    0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f// 右下
};
//constinit float attributeArray[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// 左下
//                                    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,// 右上
//                                    0.0f, 0.5f, 0.0f, 0.0f, 1.0f,// 左上
//                                    0.5f, 0.0f, 1.0f, 1.0f, 1.0f// 右下
//};
constinit uint EBOArray[] = {
        2, 1, 0,
        0, 1, 3
};

int main(const int argc, const char *argv[]) {
    try {
        Ace::GLWindow window(windowWidth, windowHeight, "computer graphics");
        window.generateVao();
        window.generateEBO(EBOArray);
        window.generateVBO(attributeArray, {{0, 2},
                                            {1, 3},
                           {2, 2}});

        window.loadShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl", "config/variablesLocation.json");
        window.load2DTexture("Images/brick.png", "cusTexture");
        window.load2DTexture("Images/awesomeface.png", "cusTexture1");
        window.setBackgroundColor(0.0f, 1.0f, 1.0f, 0.0f);
        window.render();
    } catch (std::exception &e) {
        return 1;
    }
    return 0;
}