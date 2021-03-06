//
// File "func.cpp"
// Test of small graphic package: Draw a graph of function y = f(x)
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gwindow.h"

//--------------------------------------------------
// Definition of our main class "MyWindow"
//
class MyWindow: public GWindow {    // Our main class
    R2Point lastClick;              // Last mouse click point
    bool clicked;                   // Mouse was clicked
    unsigned int mouseButton;       // Mouse state
public:
    MyWindow():                     // Constructor
        lastClick(),
        clicked(false),
        mouseButton(0)
    {}

    double f(double x);             // Function y = f(x)
    void drawGraphic();             // Draw graph of function

    virtual void onExpose(XEvent& event);
    virtual void onKeyPress(XEvent& event);
    virtual void onButtonPress(XEvent& event);
};

//----------------------------------------------------------
// Implementation of class "MyWindow"

//
// Function y = f(x)
//
double MyWindow::f(double x) {
    return 5. / (1. + 0.1*x*x);
}

//
// Process the Expose event: draw in the window
//
void MyWindow::onExpose(XEvent& event) {
    // Erase a window
    setForeground(getBackground());
    fillRectangle(m_RWinRect);

    // Draw the coordinate axes
    drawAxes("black", true, "gray");

    // Draw a graph of function
    setForeground("red");
    drawGraphic();

    // Draw a cross on mouse click
    if (clicked) {
        if (mouseButton == Button1)
            setForeground("blue");      // Left button
        else if (mouseButton == Button2)
            setForeground("SeaGreen");  // Middle button
        else if (mouseButton == Button3)
            setForeground("brown");     // Right mouse button
        R2Vector dx(0.2, 0.);
        R2Vector dy(0., 0.2);
        drawLine(lastClick-dx, lastClick+dx);
        drawLine(lastClick-dy, lastClick+dy);
    }
}

//
// Process the KeyPress event: 
// if "q" is pressed, then close the window
//
void MyWindow::onKeyPress(XEvent& event) {
    KeySym key;
    char keyName[256];
    int nameLen = XLookupString(&(event.xkey), keyName, 255, &key, 0);
    printf("KeyPress: keycode=0x%x, state=0x%x, KeySym=0x%x\n",
        event.xkey.keycode, event.xkey.state, (int) key);
    if (nameLen > 0) {
        keyName[nameLen] = 0;
        printf("\"%s\" button pressed.\n", keyName);
        if (keyName[0] == 'q') { // quit => close window
            destroyWindow();
        }
    }
}

// Process mouse click
void MyWindow::onButtonPress(XEvent& event) {
    int x = event.xbutton.x;
    int y = event.xbutton.y;
    mouseButton = event.xbutton.button;

    printf("Mouse click: x=%d, y=%d, button=%d\n", x, y, mouseButton);

    lastClick = invMap(I2Point(x, y));
    clicked = true;
    redraw();
}

//
// Draw graph of function in a window
//
void MyWindow::drawGraphic() {
    R2Point p;
    double dx = 0.05;
    double xmax = getXMax();

    p.x = getXMin();
    p.y = f(p.x);

    moveTo(p);
    while (p.x < xmax) {
        drawLineTo(p);
        p.x += dx;
        p.y = f(p.x);
    }
}

//
// End of class MyWindow implementation
//----------------------------------------------------------

/////////////////////////////////////////////////////////////
// Main: initialize X, create an instance of MyWindow class,
//       and start the message loop
int main() {
    // Initialize X stuff
    if (!GWindow::initX()) {
        printf("Could not connect to X-server.\n");
        exit(1);
    }

    MyWindow w;
    w.createWindow(
        I2Rectangle(                    // Window frame rectangle:
            I2Point(10, 10),            //     left-top corner
            GWindow::screenMaxX()/2,    //     width
            GWindow::screenMaxY()/2     //     height
        ),
        R2Rectangle(                    // Coordinate rectangle:
            R2Point(-12., -9.),         //     bottom-right corner
            24., 18.                    //     width, height
         ),
        "Graph of Function"             // Window title
    );
    w.setBackground("lightGray");

    GWindow::messageLoop();

    GWindow::closeX();
    return 0;
}
