#ifndef _RENDER_TEST_H
#define _RENDER_TEST_H

#include <vector>

class RenderTest
{
public:
    static RenderTest& Instance();

public:
    RenderTest();
    ~RenderTest();

    int Init(HWND hwnd);
    void Render(double lasttime, double nowtime);

private:
    void TestSwfToTexture();
    HWND m_hwnd;
};

#endif // ! _RENDER_TEST_H