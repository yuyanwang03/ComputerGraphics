#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixel( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
    int row_size = bytes_per_pixel * width;
    Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
    for (int y = 0; y < height * 0.5; y += 1)
    {
        Uint8* pos = (Uint8*)pixels + y * row_size;
        memcpy(temp_row, pos, row_size);
        Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
        memcpy(pos, pos2, row_size);
        memcpy(pos2, temp_row, row_size);
    }
    delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

    // Get filesize
    std::streamsize size = 0;
    if (file.seekg(0, std::ios::end).good()) size = file.tellg();
    if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

    if (!size)
        return false;

    std::vector<unsigned char> buffer;

    // Read contents of the file into the vector
    if (size > 0)
    {
        buffer.resize((size_t)size);
        file.read((char*)(&buffer[0]), size);
    }
    else
        buffer.clear();

    std::vector<unsigned char> out_image;

    if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
        return false;

    size_t bufferSize = out_image.size();
    unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
    
    // Force 3 channels
    bytes_per_pixel = 3;

    if (originalBytesPerPixel == 3) {
        pixels = new Color[bufferSize];
        memcpy(pixels, &out_image[0], bufferSize);
    }
    else if (originalBytesPerPixel == 4) {

        unsigned int newBufferSize = width * height * bytes_per_pixel;
        pixels = new Color[newBufferSize];

        unsigned int k = 0;
        for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
            pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
            k++;
        }
    }

    // Flip pixels in Y
    if (flip_y)
        FlipY();

    return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
    unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

    unsigned short header_short[3];
    header_short[0] = width;
    header_short[1] = height;
    unsigned char* header = (unsigned char*)header_short;
    header[4] = 24;
    header[5] = 0;

    fwrite(TGAheader, 1, sizeof(TGAheader), file);
    fwrite(header, 1, 6, file);

    // Convert pixels to unsigned char
    unsigned char* bytes = new unsigned char[width*height*3];
    for(unsigned int y = 0; y < height; ++y)
        for(unsigned int x = 0; x < width; ++x)
        {
            Color c = pixels[y*width+x];
            unsigned int pos = (y*width+x)*3;
            bytes[pos+2] = c.r;
            bytes[pos+1] = c.g;
            bytes[pos] = c.b;
        }

    fwrite(bytes, 1, width*height*3, file);
    fclose(file);

    return true;
}


void Image::DrawRect(int x, int y, int w, int h, const Color& c)
{
    for (int i = 0; i < w; ++i) {
        SetPixel(x + i, y, c);
        SetPixel(x + i, y + h, c);
    }

    for (int j = 0; j < h; ++j) {
        SetPixel(x, y + j, c);
        SetPixel(x + w, y + j, c);
    }
}

void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color &c){
    float dx(x1-x0), dy(y1-y0), tempX(x0), tempY(y0);
    float d = (float) std::max(abs(dx), abs(dy)); // or add a line "using namespace std"
    Vector2 dir(dx/d, dy/d);
    for(int i = 0; i < d; ++i){
        // Avoid drawing out of the range of the windows
        this->SetPixelSafe(floor(tempX), floor(tempY), c);
        tempX += dir.x;
        tempY += dir.y;
    }
    return;
}

void Image::DrawLineBresenham(int x0, int y0, int x1, int y1, const Color &c){
    int dx, dy, inc_E, inc_NE, d, x, y;
    // Create a boolean indicator to check if the line is in the 2, 3, 6 or 7 octants
    bool reverse = abs(y1-y0)>abs(x1-x0);
    // If it is the case, we should swap the definitions of dx and dy
    dx = reverse ? y1-y0 : x1-x0;
    dy = reverse ? x1-x0 : y1-y0;
    // Change order of x0 and x1 for them to be increasing; that is, to have x1>x0
    if (dx < 0) {return DrawLineBresenham(x1, y1, x0, y0, c);}
    // Having increasing x, see if the line has a positive or a negative slope; this is indicated with the orientationHandler
    int orientationHandler = (dy > 0) ? 1 : -1;
    // Since we already take into account the vertical orientation, dy should take positive value
    dy *= orientationHandler;
    inc_E = 2*dy;
    inc_NE = 2*(dy-dx);
    d = 2*dy - dx;
    x = x0; y = y0;
    this->SetPixelSafe(x0, y0, c);
    // If the program is drawing in the 2, 3, 6 or 7 octants, it should make a loop with respect y; otherwise, with respect x
    if (reverse){
        // Iterate with respect to y
        while (y < y1){
            // Consider if it should decrease, increase or maintain the same x position
            if (d <= 0) {d += inc_E;}
            else {d += inc_NE; x += orientationHandler;}
            y++;
            this->SetPixelSafe(x, y, c);
        }
    } else {
        // Iterate with respect to x
        while (x < x1){
            // Consider if it should decrease, increase or maintain the same y position
            if (d <= 0) {d += inc_E;}
            else {d += inc_NE; y += orientationHandler;}
            x++;
            this->SetPixelSafe(x, y, c);
        }
    }
    return;
}

void Image::ScanLineBresenham(int x0, int y0, int x1, int y1, std::vector<cell> &table){
    // 4th and 5th octants
    if (x1 < x0) { int t = x1; x1 = x0; x0 = t; t = y1; y1 = y0; y0 = t; }
    
    int dx, dy, iE, iNE, d, x, y;
    dx = x1 - x0;
    dy = y1 - y0;
    // 8th octant
    if (y0 > y1) {dy = -dy;}
    
    iE = 2 * dy;
    iNE = 2 * (dy - dx);
    d = 2 * dy - dx;
    x = x0; y = y0;
    
    if ((y >= 0 && y <= this->height)){
        if (x < table[y].minx) {table[y].minx = x;}
        if (x > table[y].maxx) {table[y].maxx = x;}
    }
    if (dx > dy) {
        while (x < x1) {
            if (d <= 0) { d += iE; x += 1;}
            else {
                d += iNE; x += 1;
                if (y0 > y1) {y -= 1;}
                else {y += 1;}
            }
            if ((y >= 0 && y <= this->height)){
                if (x < table[y].minx) {table[y].minx = x;}
                if (x > table[y].maxx) {table[y].maxx = x;}
            }
        }
    }
    // 2nd and 6th octants
    else {
        iE = 2 * dx;
        iNE = 2 * (dx - dy);
        d = 2 * dx - dy;
        // 3rd and 7th octants
        if (y1 < y0) {int t = x1; x1 = x0; x0 = t; t = y1; y1 = y0; y0 = t; x = x0; y = y0;}
        while (y < y1) {
            if (d <= 0) { d += iE; y += 1;}
            else {
                d += iNE; y += 1;
                if (x0 > x1) {x -= 1;}
                else {x += 1;}
            }
            if ((y >= 0 && y <= this->height)){
                if (x < table[y].minx) {table[y].minx = x;}
                if (x > table[y].maxx) {table[y].maxx = x;}
            }
        }
    }
}

/*
void Image::ScanLineBresenham(int x0, int y0, int x1, int y1, std::vector<cell> &table){
    int dx, dy, inc_E, inc_NE, d, x, y;
    // Create a boolean indicator to check if the line is in the 2, 3, 6 or 7 octants
    bool reverse = abs(y1 - y0) > abs(x1 - x0);
    // If it is the case, we should swap the definitions of dx and dy
    dx = reverse ? y1 - y0 : x1 - x0;
    dy = reverse ? x1 - x0 : y1 - y0;
    // Change order of x0 and x1 for them to be increasing; that is, to have x1>x0
    if (dx < 0) { return ScanLineBresenham(x1, y1, x0, y0, table);}
    // Having increasing x, see if the line has a positive or a negative slope; this is indicated with the orientationHandler
    int orientationHandler = (dy > 0) ? 1 : -1;
    // Since we already take into account the vertical orientation, dy should take positive value
    dy *= orientationHandler;
    inc_E = 2 * dy;
    inc_NE = 2 * (dy - dx);
    d = 2 * dy - dx;
    x = x0; y = y0;
    
    // If the program is drawing in the 2, 3, 6 or 7 octants, it should make a loop with respect y; otherwise, with respect x
    if (reverse) {
        // Iterate with respect to y
        while (y < y1) {
            if ((y >= 0 && y < this->height) && (x >= 0 && x < this->width)) {
                if (x < table[y].min) {table[y].min = x;}
                if (x > table[y].max) {table[y].max = x;}
            }
            // Consider if it should decrease, increase or maintain the same x position
            if (d <= 0) { d += inc_E; }
            else { d += inc_NE; x += orientationHandler; }
            y++;
        }
    }
    else {
        // Iterate with respect to x
        while (x < x1) {
            if ((y >= 0 && y < this->height) && (x >= 0 && x < this->width)) {
                if (x < table[y].min) {table[y].min = x;}
                if (x > table[y].max) {table[y].max = x;}
            }
            // Consider if it should decrease, increase or maintain the same y position
            if (d <= 0) { d += inc_E; }
            else { d += inc_NE; y += orientationHandler; }
            x++;
        }
    }
    return;
}*/

void Image::DrawCircle(int x0, int y0, int r, const Color &c, bool fill){
    int x(0), y(r), v(1-r);
    this->SetPixelSafe(x0, y0+y, c);
    while (y>=x){
        if (v<0){
            v = v + 2*x + 3;
            x++;
        }else{
            v = v + 2*(x-y) + 5;
            x++; y--;
        }
        this->SetPixelSafe(x0 + x, y0 + y, c);
        this->SetPixelSafe(x0 - x, y0 + y, c);
        this->SetPixelSafe(x0 + x, y0 - y, c);
        this->SetPixelSafe(x0 - x, y0 - y, c);
        this->SetPixelSafe(x0 + y, y0 + x, c);
        this->SetPixelSafe(x0 - y, y0 + x, c);
        this->SetPixelSafe(x0 + y, y0 - x, c);
        this->SetPixelSafe(x0 - y, y0 - x, c);
        if (fill) {
            for (int i = x0 - x; i <= x0 + x; i++) { SetPixelSafe(i, y0 + y, c); SetPixelSafe(i, y0 - y, c); SetPixelSafe(i, y0, c);}
            for (int i = x0 - y; i <= x0 + y; i++) { SetPixelSafe(i, y0 + x, c); SetPixelSafe(i, y0 - x, c); SetPixelSafe(i, y0, c);}
        }
    }
    this->SetPixelSafe(x0+r, y0, c);
    this->SetPixelSafe(x0-r, y0, c);
    return;
}

void Image::DrawTriangle(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Color& color){
    // Intialize the Active Edges Table (AET)
    std::vector<cell> table = std::vector<cell>(this->height);
    // Fill the AET
    ScanLineBresenham(p0.x, p0.y, p1.x, p1.y, table);
    ScanLineBresenham(p1.x, p1.y, p2.x, p2.y, table);
    ScanLineBresenham(p0.x, p0.y, p2.x, p2.y, table);
    
    for (int i=0; i<this->height; i++){
        for (int j=table[i].minx; j<=table[i].maxx; j++) {SetPixelSafe(j, i, color);}
    }
    return;
}

Color Image::BarycentricInterpolation(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2, Color c0, Color c1, Color c2){
    Vector2 v0(p1-p0), v1(p2-p0), v2(p-p0);
    float d00(v0.Dot(v0)), d01(v0.Dot(v1)), d11(v1.Dot(v1)), d20(v2.Dot(v0)), d21(v2.Dot(v1));
    float denom = d00*d11 - d01*d01;
    float v = (d11*d20 - d01*d21)/denom;
    float w = (d00*d21 - d01*d20)/denom;
    float u = 1.0 - v - w;
    u = clamp(u, 0 , 1);
    v = clamp(v, 0 , 1);
    w = clamp(w, 0 , 1);
    float sum = u + v + w;
    u /= sum; v /= sum; w /= sum;
    // if (u<0 || v<0 || w<0) {std::cout<<"exit"<<std::endl; return;}
    // if (u+v+w != 1.0) {std::cout << "error color "<< u<< " "<<v <<" "<<w <<std::endl; return Color::WHITE;}
    return Color(c0*u+c1*v+c2*w);
}

void Image::DrawTriangleInterpolated(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Color &c0, const Color &c1, const Color &c2){
    // Intialize the Active Edges Table (AET)
    std::vector<cell> table = std::vector<cell>(this->height);
    // Fill the AET
    ScanLineBresenham(p0.x, p0.y, p1.x, p1.y, table);
    ScanLineBresenham(p1.x, p1.y, p2.x, p2.y, table);
    ScanLineBresenham(p0.x, p0.y, p2.x, p2.y, table);
    
    Color pixelColor;
    for (int i =0; i<this->height; i++){
        // std::cout <<table[i].min<<" "<<table[i].max<< std::endl;
        for (int j=table[i].minx; j<=table[i].maxx; j++){
            pixelColor = BarycentricInterpolation(Vector2(j, i), Vector2(p0.x, p0.y), Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), c0, c1, c2);
            SetPixelSafe(j, i, pixelColor);
        }
    }
    return;
}


// Use BarycentricInterpolation to calculate the z value of a point inside the triangle
float Image::BarycentricInterpolation(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2, float p0z, float p1z, float p2z){
    Vector2 v0(p1-p0), v1(p2-p0), v2(p-p0);
    float d00(v0.Dot(v0)), d01(v0.Dot(v1)), d11(v1.Dot(v1)), d20(v2.Dot(v0)), d21(v2.Dot(v1));
    float denom = d00*d11 - d01*d01;
    float v = (d11*d20 - d01*d21)/denom;
    float w = (d00*d21 - d01*d20)/denom;
    float u = 1.0 - v - w;
    u = clamp(u, 0 , 1);
    v = clamp(v, 0 , 1);
    w = clamp(w, 0 , 1);
    float sum = u + v + w;
    u /= sum; v /= sum; w /= sum;
    if (u<0 || v<0 || w<0) {std::cout<<"exit"<<std::endl; return;}
    // if (u+v+w!=1.0) {std::cout << "error color "<< u<< " "<<v <<" "<<w <<std::endl; return Color::WHITE;}
    float z = p0z*u + p1z*v + p2z*w;
    return z;
}

void Image::DrawTriangleInterpolated(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Color &c0, const Color &c1, const Color &c2, FloatImage* zbuffer){
    // Intialize the Active Edges Table (AET)
    std::vector<cell> table = std::vector<cell>(this->height);
    // Fill the AET
    ScanLineBresenham(p0.x, p0.y, p1.x, p1.y, table);
    ScanLineBresenham(p1.x, p1.y, p2.x, p2.y, table);
    ScanLineBresenham(p0.x, p0.y, p2.x, p2.y, table);
    Color pixelColor;
    for (int i =0; i<this->height; i++){
        // std::cout <<table[i].min<<" "<<table[i].max<< std::endl;
        for (int j=table[i].minx; j<=table[i].maxx; j++){
            // if (table[i].min==INT_MAX || table[i].max==INT_MIN) continue;
            float z = BarycentricInterpolation(Vector2(j, i), Vector2(p0.x, p0.y), Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), p0.z, p1.z, p2.z);
            // Don't do anything if value z is larger than the one stored in zbuffer, meaning that the current pixel is farer to the camera
            if (z>=zbuffer->GetPixel(j, i)) {continue;}
            zbuffer->SetPixel(j, i, z);
            pixelColor = BarycentricInterpolation(Vector2(j, i), Vector2(p0.x, p0.y), Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), c0, c1, c2);
            SetPixel(j, i, pixelColor);
        }
    }
    return;
}

void Image::DrawTriangleInterpolated(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Color &c0, const Color &c1, const Color &c2, FloatImage* zbuffer, Image * texture, const Vector2 &uv0, const Vector2 &uv1, const Vector2 &uv2){
    return;
}

/*
void Image::DrawTriangleInterpolated(const sTriangleInfo &triangle, FloatImage* zbuffer){
    return;
}
*/

#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}
