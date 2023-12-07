#include "ComplexPlane.h"
#include <iostream>
#include <complex>
#include <cmath>
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixelWidth = pixelWidth;
	m_pixelHeight = pixelHeight; 
	m_aspectRatio = (float)pixelHeight / (float)pixelWidth;
	//m_aspectRatio = (float)pixelWidth / (float)pixelHeight;
	m_plane_center = { 0,0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_ZoomCount = 0;
	m_state = State::CALCULATING;
	//cout << m_plane_size.x << " " << m_plane_size.y << endl;
	m_vArray = VertexArray(Points, pixelWidth * pixelHeight);
}

void ComplexPlane::updateRender()
{
	if (m_state == State::CALCULATING)
	{
		for (int i = 0; i < m_pixelHeight; i++)
		{
			for (int j = 0; j < m_pixelWidth; j++)
			{
				m_vArray[j + i * m_pixelWidth].position = { (float)j,(float)i };
				//cout << "updaterender" << endl;
				Vector2f coordinate = mapPixelToCoords({ j, i });
				size_t count = countIterations(coordinate);

				Uint8  r;
				Uint8 g;
				Uint8 b;

				iterationsToRGB(count, r, g, b);
				m_vArray[j + i * m_pixelWidth].color = { r,g,b };
			}
		}
		m_state = State::DISPLAYING;
	}
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
} 
void ComplexPlane::zoomOut()
{
    m_ZoomCount--;
	float localXVariable = BASE_WIDTH * (pow(BASE_ZOOM, m_ZoomCount));
	float localYVariable = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_ZoomCount));
	m_plane_size = { localXVariable, localYVariable };
	m_state = State::CALCULATING;
}
void ComplexPlane::zoomIn()  
{
	m_ZoomCount++;
	float localXVariable = BASE_WIDTH * (pow(BASE_ZOOM, m_ZoomCount));
	float localYVariable = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_ZoomCount));
	m_plane_size = { localXVariable, localYVariable };
	m_state = State::CALCULATING;
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
	//cout << "setcenter" << endl;
	Vector2f newCenter = mapPixelToCoords(mousePixel);
	m_plane_center = newCenter;
	m_state = State::CALCULATING;
}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
	//m_mouseLocation = mousePixel;
} 
void ComplexPlane::loadText(Text& text)
{
	/*text.setString("Mandelbrot set");
	text.setPosition(10.0f, 10.0f);
	//or
	//stringstream ss
	//ss << "Zoom Count: " << m_ZoomCount << "\n";
	// ss << "Center: (" << m_mouseLocation.x << ", " << mouseLocation.y << ")\n";
	//text.setString(ss.str());
	*/
	stringstream out;

	out << "Mandelbrot Set\n";  //print tittle of game
	out << "Center:(" << m_plane_center.x << "," << m_plane_center.y << ")" << endl;  //print the center
	out << "Cursor:(" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl;        //print the mouse location
	out << "Left-click to Zoom in" << endl;     //instructions for user on how to use program
	out << "Right-click to Zoom out" << endl;
	// Set the text string of the Text object to the stringstream contents
	text.setString(out.str());

	text.setCharacterSize(22);       //setting text size
	text.setFillColor(Color::White); //setting text color
	text.setPosition(10, 10);         //setting text position
}
int ComplexPlane::countIterations(Vector2f coord)
{
	complex<double> c(coord.x, coord.y);
	complex<double>x(0, 0);
	size_t count = 0;
	while (abs(x) < 2 && count < MAX_ITER)
	{
		x = pow(x, 2) + c;
		count++;
	}
	return count; 
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER)	//At MAX_ITER color the pixel black
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else if (count < 10)		//purple
	{
		r = 127;
		g = 0;
		b = 255;
	}
	else if (count < 20)		//just green
	{
		r = 0;
		g = 255;
		b = 0;
	}
	else if (count < 30)		//just blue
	{
		r = 0;
		g = 0;
		b = 255;
	}
	else if (count < 40)		//yellow
	{
		r = 255;
		g = 255;
		b = 0;
	}
	else if (count < 50)		//fully red
	{
		r = 255;
		g = 0;
		b = 0;
	}
}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    
	float xRatio = static_cast<float>(mousePixel.x) / static_cast<float>(m_pixelWidth);
	float yRatio = static_cast<float>(mousePixel.y) / static_cast<float>(m_pixelHeight);

	// Calculate the corresponding coordinates in the complex plane
	float xCoord = m_plane_center.x - 0.5f * m_plane_size.x + xRatio * m_plane_size.x;
	float yCoord = m_plane_center.y - 0.5f * m_plane_size.y + yRatio * m_plane_size.y;

	return { xCoord, yCoord };
    

    //float xCoord = ((static_cast<float>(mousePixel.x) / m_pixelWidth) * 4) - 2;
    //float yCoord = (((static_cast<float>(mousePixel.y) - m_pixelHeight) / -m_plane_size.y) * 4) - 2; 
	/*
	cout << " x coord" <<xCoord << endl;
	cout << "ycoord" << yCoord << endl;
	cout << "xpixel" << mousePixel.x << endl;
	cout << "ypixel" << mousePixel.y << endl;
	cout << "width" <<  m_pixelWidth << endl;
	cout <<"height" << m_pixelHeight << endl << endl;
	*/

    //return { xCoord, yCoord };

}
