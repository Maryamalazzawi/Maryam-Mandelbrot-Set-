#include <iostream>
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"
using namespace std;
using namespace sf;

int main()
{
	int desktopWidth = VideoMode::getDesktopMode().width / 2;
	int desktopHeight = VideoMode::getDesktopMode().height / 2;
    
    VideoMode vm(desktopWidth, desktopHeight);
	RenderWindow window(vm, "Mandelbrot set", Style::Default);
    ComplexPlane complexPlaneObject(desktopWidth, desktopHeight);

    Font font; 
    font.loadFromFile("arial.ttf");
    if (!font.loadFromFile("arial.ttf"))
    {
        cerr << "Error loading font." << endl;
    }

	Text instructionText;
	instructionText.setFont(font);
	instructionText.setCharacterSize(20);
	//instructionText.setFillColor(Color::White);
	//instructionText.setPosition(10.f, 10.f);
	//instructionText.setString("Mandelbrot");
    enum State { CALCULATING, DISPLAYING };
    State state = CALCULATING;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                // Quit the game when the window is closed
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    complexPlaneObject.zoomIn();
                    complexPlaneObject.setCenter({ event.mouseButton.x, event.mouseButton.y });
                    //cout << "xmain" << event.mouseButton.x << endl << "ymain" << event.mouseButton.y << endl;
                    state = State::CALCULATING;
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    complexPlaneObject.zoomOut();
                    complexPlaneObject.setCenter({ event.mouseButton.x, event.mouseButton.y }); 
                    //cout << "xmain" << event.mouseButton.x << endl << "ymain" << event.mouseButton.y << endl;
                    state = State::CALCULATING;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                complexPlaneObject.setMouseLocation({ event.mouseButton.x, event.mouseButton.y });
               // cout << "xmain" << event.mouseButton.x << endl << "ymain" << event.mouseButton.y << endl;
            }

        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
       
        //Update
        complexPlaneObject.updateRender();
        complexPlaneObject.loadText(instructionText);
      

        window.clear();
        window.draw(complexPlaneObject);
        window.draw(instructionText);
        window.display();
    }
}