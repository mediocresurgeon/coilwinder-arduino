#include "CoilWinderGui.hpp"
#include "MCUFRIEND_kbv.h"
#include "TouchScreen.h"
#include <WString.h>

using namespace std;

// Assign human-readable names to some common 16-bit color values:
#define	BLACK      0x0000
#define	BLUE       0x001F
#define LIGHT_BLUE 0x073F
#define	RED        0xF800
#define	GREEN      0x07E0
#define CYAN       0x07FF
#define MAGENTA    0xF81F
#define YELLOW     0xFFE0
#define WHITE      0xFFFF

// Controls how large the cancel/okay buttons are
#define BUTTON_RADIUS 20
// The location of the center of the Okay button
#define OKAY_BTN_X 209
#define OKAY_BTN_Y 294
// The location of the center of the Cancel button
#define CANCEL_BTN_X  30
#define CANCEL_BTN_Y 294

#define COILS_UPPER_X   5
#define COILS_UPPER_Y   5
#define COILS_WIDTH   230
#define COILS_HEIGHT   45

#define ROTS_UPPER_X   5
#define ROTS_UPPER_Y  60
#define ROTS_WIDTH   230
#define ROTS_HEIGHT   45

#define UP_UPPER_X 129
#define UP_UPPER_Y 274
#define UP_WIDTH    40
#define UP_HEIGHT   40

#define DOWN_UPPER_X  69
#define DOWN_UPPER_Y 274
#define DOWN_WIDTH    40
#define DOWN_HEIGHT   40


//////////////////////
// BEGIN INITIALISE //
//////////////////////

CoilWinderGui::CoilWinderGui() {
    m_coilCount = 1;
    m_rotationCount = 300;
    m_highlightedArea = Rotations;
}

void CoilWinderGui::start() {
    // This method configures the touchscreen and loads its initial graphics
    m_tft.begin(m_tft.readID());
    m_tft.fillScreen(BLACK);
    drawCoilField();
    drawRotationField();
    drawCancelButton();
    drawDownButton();
    drawUpButton();
    drawOkayButton();
}

////////////////////
// END INITIALISE //
////////////////////

/////////////////////////
// BEGIN DRAW GRAPHICS //
/////////////////////////

void CoilWinderGui::drawCoilField() {
    // Draws the field which displays the number of coils to process
    bool isHighlighted = (Coils == m_highlightedArea);
    int16_t border = 5;
    m_tft.fillRect(COILS_UPPER_X-border,
                   COILS_UPPER_Y-border,
                   COILS_WIDTH+(2*border),
                   COILS_HEIGHT+(2*border),
                   isHighlighted ? YELLOW : BLACK);
    m_tft.fillRect(COILS_UPPER_X,
                   COILS_UPPER_Y,
                   COILS_WIDTH,
                   COILS_HEIGHT,
                   WHITE);
    m_tft.setCursor(COILS_UPPER_X+10, COILS_UPPER_Y+15);
    m_tft.setTextColor(BLACK);
    m_tft.setTextSize(2);
    String spaces = (m_coilCount > 9) ? String(F("          "))
                                      : String(F("           "));
    m_tft.print("Coils:" + spaces + String(m_coilCount));
}

void CoilWinderGui::drawRotationField() {
    // Draws the field which displays the number of rotations to process
    bool isHighlighted = (Rotations == m_highlightedArea);
    int16_t border = 5;
    m_tft.fillRect(ROTS_UPPER_X-border,
                   ROTS_UPPER_Y-border,
                   ROTS_WIDTH+(2*border),
                   ROTS_HEIGHT+(2*border),
                   isHighlighted ? YELLOW : BLACK);
    m_tft.fillRect(ROTS_UPPER_X,
                   ROTS_UPPER_Y,
                   ROTS_WIDTH,
                   ROTS_HEIGHT,
                   WHITE);
    m_tft.setCursor(ROTS_UPPER_X+10, ROTS_UPPER_Y+15);
    m_tft.setTextColor(BLACK);
    m_tft.setTextSize(2);
    String spaces = (m_rotationCount > 99) ? String(F("     "))
                                           : String(F("      "));
    m_tft.print("Rotations:" + spaces + String(m_rotationCount));
}

void CoilWinderGui::drawCancelButton() {
    // This method draws the Cancel button
    int16_t halfThickness = 2;
    int16_t length = 10;

    m_tft.fillCircle(CANCEL_BTN_X, CANCEL_BTN_Y, BUTTON_RADIUS, RED);

    for (int16_t i = -halfThickness; i < 2*halfThickness; i++) {
        int16_t offset = abs(i);
        // upper left to lower right
        m_tft.drawLine(CANCEL_BTN_X-length+offset-halfThickness,
                       CANCEL_BTN_Y-length,
                       CANCEL_BTN_X+length+offset-halfThickness,
                       CANCEL_BTN_Y+length,
                       WHITE);
        // lower left to upper right
        m_tft.drawLine(CANCEL_BTN_X-length+offset-halfThickness,
                       CANCEL_BTN_Y+length,
                       CANCEL_BTN_X+length+offset-halfThickness,
                       CANCEL_BTN_Y-length,
                       WHITE);
    }
}

void CoilWinderGui::drawOkayButton() {
    // This method draws the Okay button
    m_tft.fillCircle(OKAY_BTN_X, OKAY_BTN_Y, BUTTON_RADIUS, GREEN);

    int16_t leftX = OKAY_BTN_X-9;
    int16_t leftY = OKAY_BTN_Y-2;

    int16_t middleX = OKAY_BTN_X-4;
    int16_t middleY = OKAY_BTN_Y+6;

    int16_t rightX = OKAY_BTN_X+10;
    int16_t rightY = OKAY_BTN_Y-8;

    for (int16_t offset = 0; offset < 5; offset++) {
        m_tft.drawLine(leftX,   leftY+offset,   middleX, middleY+offset, BLACK);
        m_tft.drawLine(middleX, middleY+offset, rightX,  rightY+offset,  BLACK);
    }
}

void CoilWinderGui::drawUpButton() {
    int16_t leftX = UP_UPPER_X;
    int16_t middleX = UP_UPPER_X + (UP_WIDTH / 2);
    int16_t rightX = UP_UPPER_X + UP_WIDTH;
    
    int16_t topY = UP_UPPER_Y;
    int16_t middleY = UP_UPPER_Y + (UP_HEIGHT / 2);

    int16_t stumpX = UP_UPPER_X + (UP_WIDTH / 4);

    m_tft.fillTriangle(leftX, middleY, middleX, topY, rightX, middleY, LIGHT_BLUE);
    m_tft.fillRect(stumpX, middleY, 1+UP_WIDTH/2, UP_HEIGHT/2, LIGHT_BLUE);
}

void CoilWinderGui::drawDownButton() {
    int16_t leftX = DOWN_UPPER_X;
    int16_t middleX = DOWN_UPPER_X + (DOWN_WIDTH / 2);
    int16_t rightX = DOWN_UPPER_X + DOWN_WIDTH;

    int16_t topY = DOWN_UPPER_Y;
    int16_t middleY = DOWN_UPPER_Y + (DOWN_HEIGHT / 2);
    int16_t bottomY = DOWN_UPPER_Y + DOWN_HEIGHT;

    int16_t stumpX = DOWN_UPPER_X + (DOWN_WIDTH / 4);

    m_tft.fillRect(stumpX, topY, 1+DOWN_WIDTH/2, DOWN_HEIGHT/2, LIGHT_BLUE);
    m_tft.fillTriangle(leftX, middleY, middleX, bottomY, rightX, middleY, LIGHT_BLUE);
    
}

///////////////////////
// END DRAW GRAPHICS //
///////////////////////

///////////////////////////////////
// BEGIN INTERPRET USER COMMANDS //
///////////////////////////////////

TouchArea CoilWinderGui::getTouchArea(TSPoint *p) {
    // This method determines if
    // a special area of the screen
    // was touched, such as a button.

    // Hardware-specific settings used for scaling
    #define TS_MINX 100
    #define TS_MAXX 920
    #define TS_MINY 70
    #define TS_MAXY 900

    // Apply scaling correction to the TSPoint
    // so that we know which pixel was touched
    p->x = map(p->x, TS_MINX, TS_MAXX, m_tft.width(), 0);
    p->y = (m_tft.height()-map(p->y, TS_MINY, TS_MAXY, m_tft.height(), 0));

    // Test for Coil field
    if (COILS_UPPER_X <= p->x && p->x <= COILS_UPPER_X + COILS_WIDTH
        && COILS_UPPER_Y <= p->y && p->y <= COILS_UPPER_Y + COILS_HEIGHT) {
        Serial.println(F("Coils"));
        return Coils;
    }

    // Test for Rotations field
    if (ROTS_UPPER_X <= p->x && p->x <= ROTS_UPPER_X + ROTS_WIDTH
        && ROTS_UPPER_Y <= p->y && p->y <= ROTS_UPPER_Y + ROTS_HEIGHT) {
        Serial.println(F("Rotations"));
        return Rotations;
    }

    // Test for Cancel button
    if (BUTTON_RADIUS >= getDistance(CANCEL_BTN_X, CANCEL_BTN_Y, p->x, p->y)) {
        Serial.println(F("Cancel"));
        return Cancel;
    }

    // Test for Okay button
    if (BUTTON_RADIUS >= getDistance(OKAY_BTN_X, OKAY_BTN_Y, p->x, p->y)) {
        Serial.println(F("Okay"));
        return Okay;
    }

    // Test for Up button
    if (UP_UPPER_X <= p->x && p->x <= UP_UPPER_X + UP_WIDTH
        && UP_UPPER_Y <= p->y && p->y <= UP_UPPER_Y + UP_HEIGHT) {
        Serial.println(F("Up"));
        return Up;
    }

    // Test for Down button
    if (DOWN_UPPER_X <= p->x && p->x <= DOWN_UPPER_X + DOWN_WIDTH
        && DOWN_UPPER_Y <= p->y && p->y <= DOWN_UPPER_Y + DOWN_HEIGHT) {
        Serial.println(F("Down"));
        return Down;
    }

    // No buttons were touched
    Serial.println("(" + String(p->x) + ", " + String(p->y) + ")");
    return Miss;
}

void CoilWinderGui::onUpBtnPress() {
    #define MAX_COILS      10
    #define MAX_ROTATIONS 500
    // When the UP button is pressed,
    // something should be increased.
    // (It might be the coil count, or
    // it might be the number of winds.)
    switch (m_highlightedArea) {
        case Coils:
            if (m_coilCount < MAX_COILS) {
                m_coilCount++;
                drawCoilField();
            }
            break;
        case Rotations:
            if (m_rotationCount < MAX_ROTATIONS) {
                m_rotationCount += 10;
                drawRotationField();
            }
            break;
        default:
            // Do nothing, because it is unclear which is being adjusted
            break;
    }
}

void CoilWinderGui::onDownBtnPress() {
    #define MIN_COILS      1
    #define MIN_ROTATIONS 10
    // When the DOWN button is pressed,
    // something should be decreased.
    // (It might be the coil count, or
    // it might be the number of winds.)
    switch (m_highlightedArea) {
        case Coils:
            if (m_coilCount > MIN_COILS) {
                m_coilCount--;
                drawCoilField();
            }
            break;
        case Rotations:
            if (m_rotationCount > MIN_ROTATIONS) {
                m_rotationCount -= 10;
                drawRotationField();
            }
            break;
        default:
            // Do nothing, because it is unclear what is being adjusted
            // (This code should be unreachable)
            break;
    }
}

void CoilWinderGui::onOkayBtnPress() {
    // TODO: Send updated signals to attached devices
}

void CoilWinderGui::onCancelBtnPress() {
    // TODO: Send signals to turn off any attached devices
}

void CoilWinderGui::onCoilsFieldPress() {
    if (Coils != m_highlightedArea) {
        m_highlightedArea = Coils;
        drawRotationField();
        drawCoilField();
    }
}

void CoilWinderGui::onRotationsFieldPress() {
    if (Rotations != m_highlightedArea) {
        m_highlightedArea = Rotations;
        drawCoilField();
        drawRotationField();
    }
}

double CoilWinderGui::getDistance(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    // This method calculates the distance between two points
    int16_t deltaX = x0-x1;
    int16_t deltaY = y0-y1;
    return sqrt(deltaX*deltaX + deltaY*deltaY);
}

/////////////////////////////////
// END INTERPRET USER COMMANDS //
/////////////////////////////////