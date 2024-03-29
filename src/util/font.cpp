#include "graphics/bitmap.h"
#include "font.h"
#include "funcs.h"
#include "init.h"
#include "ftalleg.h"
#include "font_factory.h"
#include "exceptions/exception.h"
#include <string.h>

using namespace std;

Util::Parameter<Util::ReferenceCount<Filesystem::RelativePath> > Font::defaultFont;

Font::Font(){
}

/* copy/pasted from network/message.cpp */
static vector<string> wrapStrings(const string & left, const string & right, const Font & font, int max, vector< string > accum){
    if (left == ""){
        return accum;
    }

    int length = font.textLength(left.c_str());

    if (length >= max){
        return wrapStrings(left.substr(0, left.length() / 2), left.substr(left.length() / 2) + right, font, max, accum);
    } else if (right == "" || font.textLength((left + right.substr(0, 1)).c_str()) >= max){
        accum.push_back(left);
        return wrapStrings(right, "", font, max, accum);
    } else {
        return wrapStrings(left + right.substr(0, 1), right.substr(1), font, max, accum);
    }
}

void Font::printfWrapLine(int x, int & y, Graphics::Color color, const Graphics::Bitmap & work, int maxWidth, const char * line) const {
    vector< string > all;
    all = wrapStrings(string(line), "", *this, maxWidth, all );
    for ( vector< string >::iterator str = all.begin(); str != all.end(); str++ ){
        printf(x, y, color, work, *str, 0);
        y += getHeight();
    }
    y += getHeight() / 2;
}

void Font::printfWrap(int x, int y, Graphics::Color color, const Graphics::Bitmap & work, int maxWidth, const std::string & str, int marker, ... ) const {
    char buf[4096];
    va_list ap;

    va_start(ap, marker);
    Util::limitPrintf(buf, sizeof(buf), str.c_str(), ap);
    va_end(ap);

    char * start = buf;
    char * end = strchr(start, '\n');
    while (end != NULL){
        char tmp[1024];
        unsigned int ender = end - start;
        if (ender >= sizeof(tmp) - 1){
            ender = sizeof(tmp) - 1;
        }

        strncpy(tmp, start, ender);
        tmp[ender] = '\0';
        printfWrapLine(x, y, color, work, maxWidth, tmp);

        start = end + 1;
        end = strchr(start, '\n');
    }
    printfWrapLine(x, y, color, work, maxWidth, start);
}

Font::~Font(){
}

const Font & Font::getDefaultFont(){
    return getDefaultFont(16, 16);
}
        
const Path::RelativePath & Font::getDefaultFontPath(){
    if (defaultFont.current() == NULL){
        throw Exception::FontException(__FILE__, __LINE__, "No default font set");
    }
    return *defaultFont.current();
}

const Font & Font::getDefaultFont(int width, int height){
    Font * font = FontFactory::getFont(getDefaultFontPath(), width, height);
    if (font == NULL){
        throw Exception::FontException(__FILE__, __LINE__, "No default font set");
    }
    return *font;
}
	
/* name should be the path of a .ttf file in the fonts/ directory.
 * something like 'arial.ttf'
 */
const Font & Font::getFont(const Filesystem::RelativePath & name, const int x, const int y){
    Font * check = FontFactory::getFont(name, x, y);
    if (check == NULL){
        throw Exception::FontException(__FILE__, __LINE__, "Could not get font");
    }
    Font & font = *check;
    /* sanity check */
    if (font.getHeight("A") == 0){
        return getDefaultFont();
    }
    return font;
}
	
const Font & Font::getFont( const Filesystem::AbsolutePath & name, const int x, const int y){
    return *FontFactory::getFont(name, x, y);
}

FreeTypeFont::FreeTypeFont(const Filesystem::AbsolutePath & str ):
sizeX(16),
sizeY(16),
own(true){
    this->font = new ftalleg::freetype(str, getSizeX(), getSizeY() );
}

int FreeTypeFont::getHeight( const string & str ) const {
    return this->font->getHeight(str);
}

int FreeTypeFont::getHeight() const {
    return getHeight("A");
}

int FreeTypeFont::textLength( const char * text ) const {
    return this->font->getLength(string(text));
}

void FreeTypeFont::printf( int x, int y, int xSize, int ySize, Graphics::Color color, const Graphics::Bitmap & work, const string & str, int marker, ... ) const {
    char buf[512];
    va_list ap;

    va_start(ap, marker);
    vsnprintf(buf, sizeof(buf), str.c_str(), ap);
    va_end(ap);

    int old_x = 0;
    int old_y = 0;
    this->font->getSize(&old_x, &old_y);
    this->font->setSize(xSize, ySize);
    this->font->render(x, y, color, work, ftalleg::freetype::ftLeft, string(buf), 0);
    this->font->setSize(old_x, old_y);
}
	
void FreeTypeFont::printf( int x, int y, Graphics::Color color, const Graphics::Bitmap & work, const string & str, int marker, ... ) const {
    char buf[512];
    va_list ap;

    va_start(ap, marker);
    vsnprintf(buf, sizeof(buf), str.c_str(), ap);
    va_end(ap);

    this->font->render(x, y, color, work, ftalleg::freetype::ftLeft, string(buf), 0);
}

void FreeTypeFont::setSize( const int x, const int y ){
    this->sizeX = x;
    this->sizeY = y;
    this->font->setSize( this->sizeX, this->sizeY );
}

int FreeTypeFont::getSizeX() const {
    return this->sizeX;
}

int FreeTypeFont::getSizeY() const {
    return this->sizeY;
}

FreeTypeFont::~FreeTypeFont(){
	// cout << "Delete font " << this->font << endl;
    if (own){
        delete this->font;
    }
}
