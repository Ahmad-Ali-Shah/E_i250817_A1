#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;


const int modalNone = 0;
const int modalInsertId = 1;
const int modalInsertWeight = 2;
const int modalDeleteId = 3;
const int modalPruneId = 4;
const int modalForwardLetter = 5;
const int modalBackwardLetter = 6;
const int modalInfoMessage = 7;
const int modalLoadFile = 8;

////////////////////////////////////////////////////////////////////////////////
// Class MeshVisualizer
////////////////////////////////////////////////////////////////////////////////

class MeshVisualizer {
private:

    sf::RenderWindow window;

    sf::Font fontHeading;
    sf::Font fontBodyPrimary;
    sf::Font fontBodySecondary;
    sf::Font fontUi;

    bool usePrimaryBodyFont;

    sf::Texture textureIntro;
    sf::Texture textureBackground;
    sf::Sprite spriteBackground;

    int selectedLayerIndex;
    bool isMenuExpanded;
    std::string statusText;

    sf::IntRect introFrames[16];

    int activeModal;
    std::string modalInputString;
    std::string modalTitle;
    std::string modalPromptLine;

    int pendingInsertId;

    std::string loadedFilePath;

    int hoveredMenuButton;

    Neuron* propagationPath[128];
    int propagationPathLength;
    int propagationFocusIndex;
    int propagationPhase;
    sf::Clock propagationClock;

    static sf::Color themeAccent() {

        return sf::Color(70, 160, 255);

    }

    static sf::Color themeAccentHot() {

        return sf::Color(255, 120, 50);

    }

    static sf::Color themeFound() {

        return sf::Color(255, 60, 140);

    }

public:

    static std::string resolveAssetPath(const std::string& path) {
        if (path.empty())
         return path;
        std::ifstream f1(path.c_str());

        if (f1.good())
        return path;
        std::string c2 = "../" + path;

        std::ifstream f2(c2.c_str());
        if (f2.good())
         return c2;
        std::string c3 = "Q1/" + path;
        std::ifstream f3(c3.c_str());
        if (f3.good()) 
        return c3;
        std::string c4 = "../Q1/" + path;
        std::ifstream f4(c4.c_str());
        if (f4.good())
         return c4;
        std::string c5 = "../../" + path;
        std::ifstream f5(c5.c_str());
        if (f5.good())
         return c5;
        std::string c6 = "../../Q1/" + path;
        std::ifstream f6(c6.c_str());
        if (f6.good())
        return c6;
        return path;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Constructor
    ////////////////////////////////////////////////////////////////////////////

    MeshVisualizer(): window(sf::VideoMode(1280, 720), "NeuroMesh Explorer | Ahmad Ali i250817"), usePrimaryBodyFont(false), selectedLayerIndex(0) ,isMenuExpanded(true),statusText("System Ready. Pointer Visualization Active."),activeModal(modalNone),pendingInsertId(-1), loadedFilePath(""), hoveredMenuButton(-1), propagationPathLength(0), propagationFocusIndex(-1),propagationPhase(0)
    {

        for (int pathInit = 0; pathInit < 128; pathInit++) {

            this->propagationPath[pathInit] = nullptr;

        }

        window.setFramerateLimit(60);

        fontHeading.loadFromFile(resolveAssetPath("assest/founts/del_rose/DelRose-Regular.ttf"));

        fontBodyPrimary.loadFromFile(resolveAssetPath("assest/founts/atsuka_montreal/Atsuka Montreal.otf"));

        fontBodySecondary.loadFromFile(resolveAssetPath("assest/founts/modern_prestige/Modern Prestige DEMO.otf"));

        this->loadReadableFont();

        textureIntro.loadFromFile(resolveAssetPath("assest/button.png"));

        textureBackground.loadFromFile(resolveAssetPath("assest/images (3).jpeg"));


        spriteBackground.setTexture(textureBackground);

        float scaleX = 1280.0f / (float)textureBackground.getSize().x;

        float scaleY = 720.0f / (float)textureBackground.getSize().y;

        spriteBackground.setScale(scaleX, scaleY);

        ////////////////////////////////////////////////////////////////////////
        // Intro Frame Coordinates 
        ////////////////////////////////////////////////////////////////////////

        introFrames[0]  = sf::IntRect(90,   828,  262, 277);
        introFrames[1]  = sf::IntRect(428,  856,  253, 245);
        introFrames[2]  = sf::IntRect(762,  860,  258, 237);
        introFrames[3]  = sf::IntRect(1088, 849,  296, 252);
        introFrames[4]  = sf::IntRect(1444, 857,  266, 227);
        introFrames[5]  = sf::IntRect(1792, 864,  266, 215);
        introFrames[6]  = sf::IntRect(2129, 853,  250, 233);
        introFrames[7]  = sf::IntRect(2461, 852,  254, 243);
        introFrames[8]  = sf::IntRect(83,   1170, 290, 288);
        introFrames[9]  = sf::IntRect(402,  1170, 314, 290);
        introFrames[10] = sf::IntRect(736,  1178, 306, 275);
        introFrames[11] = sf::IntRect(1088, 1177, 297, 278);
        introFrames[12] = sf::IntRect(1431, 1177, 293, 273);
        introFrames[13] = sf::IntRect(1771, 1175, 296, 276);
        introFrames[14] = sf::IntRect(2108, 1178, 301, 275);
        introFrames[15] = sf::IntRect(2435, 1178, 297, 278);

    }

    ////////////////////////////////////////////////////////////////////////////
    // Public Entry Point
    ////////////////////////////////////////////////////////////////////////////

    void run(NeuroMesh& neuralMesh) {

        this->playIntroAnimation();

        std::string initialMesh = this->meshFileExists("mesh.txt") ? (std::ifstream("mesh.txt").good() ? "mesh.txt" : resolveAssetPath("mesh.txt")) : resolveAssetPath("mesh.txt");
        if (neuralMesh.getHeadLayer() == nullptr && this->meshFileExists(initialMesh)) {
            std::string actualPath = std::ifstream(initialMesh.c_str()).good() ? initialMesh : resolveAssetPath(initialMesh);
            neuralMesh.loadFromFile(actualPath);
            this->loadedFilePath = actualPath;
            this->statusText = "Loaded " + actualPath + "  |  Use menu [1] Load Mesh File to open another";
            this->selectedLayerIndex = 0;
        }

        while (window.isOpen()) {

            sf::Event event;

            while (window.pollEvent(event)) {

                if (event.type == sf::Event::Closed) {

                    window.close();

                }

                if (event.type == sf::Event::TextEntered && this->activeModal != modalNone) {

                    char enteredCharacter = (char)event.text.unicode;

                    if (enteredCharacter == '\b') {

                        if (!this->modalInputString.empty()) {

                            this->modalInputString.pop_back();

                        }

                    } else if (enteredCharacter == '\r' || enteredCharacter == '\n') {

                        this->submitModal(neuralMesh);

                    } else if (enteredCharacter >= 32 && enteredCharacter < 127) {

                        this->modalInputString += enteredCharacter;

                    }

                }

                if (event.type == sf::Event::KeyPressed) {

                    if (event.key.code == sf::Keyboard::Escape) {

                        if (this->activeModal != modalNone) {

                            this->closeModal();
                            this->statusText = "Dialog cancelled.";

                        } else {

                            window.close();

                        }

                    }

                }

                if (event.type == sf::Event::MouseButtonPressed
                    && event.mouseButton.button == sf::Mouse::Left) {

                    float mouseX = (float)event.mouseButton.x;
                    float mouseY = (float)event.mouseButton.y;

                    this->handleMouseClick(mouseX, mouseY, neuralMesh);

                }

                if (event.type == sf::Event::MouseMoved) {

                    this->updateMenuHover((float)event.mouseMove.x, (float)event.mouseMove.y);

                }

                if (event.type == sf::Event::KeyPressed && this->activeModal == modalNone) {

                    if (event.key.code == sf::Keyboard::L) {

                        this->openLoadFileModal();

                    }

                }

            }

            window.clear(sf::Color(12, 18, 32));

            this->updatePropagationAnimation(neuralMesh);

            this->renderBackground();
            this->renderMesh(neuralMesh);
            this->renderUI(neuralMesh);

            if (this->activeModal != modalNone) {

                this->renderModal();

            }

            window.display();

        }

    }

    void renderSingleFrame(NeuroMesh& neuralMesh) {
        window.clear(sf::Color(12, 18, 32));
        this->updatePropagationAnimation(neuralMesh);
        this->renderBackground();
        this->renderMesh(neuralMesh);
        this->renderUI(neuralMesh);
        if (this->activeModal != modalNone) {
            this->renderModal();
        }
        window.display();
    }

    bool saveScreenshot(const std::string& filename) {
        sf::Texture texture;
        if (!texture.create(window.getSize().x, window.getSize().y)) return false;
        texture.update(window);
        sf::Image screenshot = texture.copyToImage();
        return screenshot.saveToFile(filename);
    }

    void setSelectedLayerIndex(int idx) { this->selectedLayerIndex = idx; }
    int getSelectedLayerIndex() const { return this->selectedLayerIndex; }
    void setStatusText(const std::string& txt) { this->statusText = txt; }
    std::string getStatusText() const { return this->statusText; }
    sf::RenderWindow& getWindow() { return this->window; }

private:

    bool meshFileExists(const std::string& filePath) {

        ifstream checkStream;
        checkStream.open(filePath.c_str(), ios::in);

        if (checkStream.is_open()) {

            checkStream.close();
            return true;

        }

        std::string resolved = resolveAssetPath(filePath);
        if (resolved != filePath) {

            checkStream.open(resolved.c_str(), ios::in);
            if (checkStream.is_open()) {

                checkStream.close();
                return true;

            }

        }

        return false;

    }

    int countTotalLayers(NeuroMesh& neuralMesh) {

        int total = 0;
        Layer* layerCounter = neuralMesh.getHeadLayer();

        while (layerCounter != nullptr) {

            total++;
            layerCounter = layerCounter->next;

        }

        return total;

    }

    bool loadReadableFont() {

        std::string modernPrestige = resolveAssetPath("assest/founts/modern_prestige/Modern Prestige DEMO.otf");
        std::string atsukaMontreal = resolveAssetPath("assest/founts/atsuka_montreal/Atsuka Montreal.otf");
        std::string delRose = resolveAssetPath("assest/founts/del_rose/DelRose-Regular.ttf");

        const std::string fontPaths[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
            modernPrestige,
            atsukaMontreal,
            delRose
        };

        for (int pathIndex = 0; pathIndex < 7; pathIndex++) {

            if (this->fontUi.loadFromFile(fontPaths[pathIndex])) {

                return true;

            }

        }

        this->fontUi = this->fontBodySecondary;
        return false;

    }

    sf::Font& getActiveUiFont() {

        return this->usePrimaryBodyFont ? this->fontBodyPrimary : this->fontUi;

    }

    void styleReadableText(sf::Text& text, sf::Color fillColor) {

        text.setFillColor(fillColor);
        text.setOutlineThickness(1.2f);
        text.setOutlineColor(sf::Color(0, 0, 0, 200));

    }

    void drawNeuronLabelPlate(sf::Vector2f center, float radius) {

        sf::RectangleShape divider(sf::Vector2f(radius * 1.15f, 1.0f));
        divider.setOrigin(divider.getSize().x / 2.0f, 0.5f);
        divider.setPosition(center);
        divider.setFillColor(sf::Color(205, 218, 235, 210));
        window.draw(divider);

    }

    void rebuildPropagationPath(NeuroMesh& neuralMesh) {

        this->propagationPathLength = 0;

        Layer* headLayer = neuralMesh.getHeadLayer();

        if (headLayer == nullptr || headLayer->topLeft == nullptr) {

            return;

        }

        Neuron* walker = headLayer->topLeft;

        while (walker != nullptr && this->propagationPathLength < 128) {

            this->propagationPath[this->propagationPathLength] = walker;
            this->propagationPathLength++;

            Synapse* axonScan = walker->headAxon;
            Synapse* activeStep = nullptr;

            while (axonScan != nullptr) {

                if (axonScan->isActive) {

                    activeStep = axonScan;
                    break;

                }

                axonScan = axonScan->nextSynapse;

            }

            if (activeStep == nullptr || activeStep->targetNeuron == nullptr) {

                break;

            }

            walker = activeStep->targetNeuron;

        }

    }

    void syncViewToPropagationFocus(NeuroMesh& neuralMesh) {

        if (this->propagationFocusIndex < 0 || this->propagationFocusIndex >= this->propagationPathLength) {

            return;

        }

        Neuron* focusNeuron = this->propagationPath[this->propagationFocusIndex];
        Layer* focusLayer = neuralMesh.locateLayerOfNeuron(focusNeuron);

        if (focusLayer != nullptr) {

            int layerIndex = neuralMesh.getLayerIndex(focusLayer);

            if (layerIndex >= 0) {

                this->selectedLayerIndex = layerIndex;

            }

        }

        this->updatePropagationStatusMessage();

    }

    void updatePropagationStatusMessage() {

        if (this->propagationFocusIndex < 0 || this->propagationFocusIndex >= this->propagationPathLength) {

            return;

        }

        Neuron* focusNeuron = this->propagationPath[this->propagationFocusIndex];

        if (this->propagationPhase == 3) {

            this->statusText = std::string("Found neuron ID ") + std::to_string(focusNeuron->id)
                + " at layer " + std::to_string(this->selectedLayerIndex);

        } else if (this->propagationPhase == 1 || this->propagationPhase == 2) {

            this->statusText = std::string("Forward signal at ID ") + std::to_string(focusNeuron->id)
                + " (layer " + std::to_string(this->selectedLayerIndex) + ")";

        }

    }

    void drawAuthorCredit() {

        const float badgeX = 988.0f;
        const float badgeY = 684.0f;
        const float badgeW = 278.0f;
        const float badgeH = 34.0f;

        this->drawRoundedPanel(
            badgeX, badgeY, badgeW, badgeH,
            sf::Color(18, 28, 52, 245),
            sf::Color(themeAccent().r, themeAccent().g, themeAccent().b, 160)
        );

        sf::Text madeByLabel("Made by", this->fontUi, 12);
        this->styleReadableText(madeByLabel, sf::Color(170, 190, 220));
        madeByLabel.setPosition(badgeX + 12.0f, badgeY + 8.0f);
        window.draw(madeByLabel);

        sf::Text authorName("Ahmad Ali", this->fontUi, 14);
        this->styleReadableText(authorName, sf::Color(255, 214, 96));
        authorName.setPosition(badgeX + 72.0f, badgeY + 7.0f);
        window.draw(authorName);

        sf::Text studentId("i250817", this->fontUi, 13);
        this->styleReadableText(studentId, themeAccent());
        studentId.setPosition(badgeX + 188.0f, badgeY + 8.0f);
        window.draw(studentId);

        sf::RectangleShape idDivider(sf::Vector2f(1.0f, 18.0f));
        idDivider.setPosition(badgeX + 178.0f, badgeY + 8.0f);
        idDivider.setFillColor(sf::Color(90, 120, 170, 200));
        window.draw(idDivider);

    }

    void startForwardPropagationVisual(NeuroMesh& neuralMesh) {

        this->rebuildPropagationPath(neuralMesh);

        if (this->propagationPathLength <= 0) {

            this->propagationPhase = 0;
            this->propagationFocusIndex = -1;
            return;

        }

        this->propagationPhase = 1;
        this->propagationFocusIndex = 0;
        this->propagationClock.restart();
        this->syncViewToPropagationFocus(neuralMesh);

    }

    void startBackwardPropagationVisual(NeuroMesh& neuralMesh) {

        this->rebuildPropagationPath(neuralMesh);

        if (this->propagationPathLength <= 0) {

            this->propagationPhase = 0;
            this->propagationFocusIndex = -1;
            return;

        }

        this->propagationPhase = 3;
        this->propagationFocusIndex = this->propagationPathLength - 1;
        this->propagationClock.restart();
        this->syncViewToPropagationFocus(neuralMesh);

    }

    void updatePropagationAnimation(NeuroMesh& neuralMesh) {

        if (this->propagationPhase == 0 || this->propagationPathLength <= 0) {

            return;

        }

        const int stepMilliseconds = 150;
        int elapsedStep = (int)(this->propagationClock.getElapsedTime().asMilliseconds() / stepMilliseconds);

        if (this->propagationPhase == 1) {

            int newFocus = elapsedStep;

            if (newFocus >= this->propagationPathLength) {

                newFocus = this->propagationPathLength - 1;
                this->propagationPhase = 2;
                this->propagationClock.restart();
                elapsedStep = 0;

            }

            if (newFocus != this->propagationFocusIndex) {

                this->propagationFocusIndex = newFocus;
                this->syncViewToPropagationFocus(neuralMesh);

            }

        }

        if (this->propagationPhase == 2) {

            int newFocus = (this->propagationPathLength - 1) - elapsedStep;

            if (newFocus != this->propagationFocusIndex) {

                this->propagationFocusIndex = newFocus;
                this->syncViewToPropagationFocus(neuralMesh);

            }

            if (newFocus < 0) {

                this->propagationPhase = 0;
                this->propagationFocusIndex = -1;
                this->statusText = "Forward path visualization complete.";

            }

        }

        if (this->propagationPhase == 3) {

            int newFocus = (this->propagationPathLength - 1) - elapsedStep;

            if (newFocus != this->propagationFocusIndex) {

                this->propagationFocusIndex = newFocus;
                this->syncViewToPropagationFocus(neuralMesh);

            }

            if (newFocus < 0) {

                this->propagationPhase = 0;
                this->propagationFocusIndex = -1;
                this->statusText = "Backward propagation finalized. Weights updated.";

            }

        }

    }

    int indexOnPropagationPath(Neuron* targetNeuron) {

        for (int pathIndex = 0; pathIndex < this->propagationPathLength; pathIndex++) {

            if (this->propagationPath[pathIndex] == targetNeuron) {

                return pathIndex;

            }

        }

        return -1;

    }

    bool isPropagationTrailNode(int pathIndex) {

        if (this->propagationPhase == 0 || pathIndex < 0) {

            return false;

        }

        if (this->propagationPhase == 1) {

            return pathIndex <= this->propagationFocusIndex;

        }

        return pathIndex >= this->propagationFocusIndex;

    }

    void renderBackground() {

        window.draw(this->spriteBackground);

        sf::RectangleShape darkTint(sf::Vector2f(1280.0f, 720.0f));
        darkTint.setFillColor(sf::Color(10, 16, 32, 130));
        window.draw(darkTint);

        sf::RectangleShape accentBar(sf::Vector2f(1280.0f, 3.0f));
        accentBar.setPosition(0.0f, 0.0f);
        accentBar.setFillColor(themeAccent());
        window.draw(accentBar);

    }

    sf::Color weightToNodeFill(double weight, double maxWeight) {

        (void)weight;
        (void)maxWeight;
        return sf::Color(255, 255, 255, 250);

    }

    void drawCrossLayerBadge(sf::Vector2f center, float radius, bool isActive) {

        sf::CircleShape badge(radius * 0.20f);
        badge.setOrigin(badge.getRadius(), badge.getRadius());
        badge.setPosition(center.x, center.y + radius * 0.78f);
        badge.setFillColor(isActive ? sf::Color(255, 90, 150, 240) : sf::Color(80, 130, 210, 210));
        badge.setOutlineThickness(1.0f);
        badge.setOutlineColor(sf::Color(255, 255, 255, 180));
        window.draw(badge);

        sf::Text layerHint("L+", this->fontUi, 10);
        this->styleReadableText(layerHint, sf::Color(255, 255, 255));
        layerHint.setOrigin(layerHint.getLocalBounds().width / 2.0f, layerHint.getLocalBounds().height / 2.0f);
        layerHint.setPosition(center.x, center.y + radius * 0.78f);
        window.draw(layerHint);

    }

    void updateMenuHover(float mouseX, float mouseY) {

        this->hoveredMenuButton = -1;

        if (!this->isMenuExpanded || mouseX < 25.0f || mouseX > 285.0f) {

            return;

        }

        if (mouseY >= 108.0f && mouseY <= 652.0f) {

            this->hoveredMenuButton = (int)((mouseY - 108.0f) / 46.0f);

            if (this->hoveredMenuButton > 11) {

                this->hoveredMenuButton = -1;

            }

        }

    }

    void openLoadFileModal() {

        this->openModal(modalLoadFile, "Load Mesh File", "Enter file path (e.g. mesh.txt or tests/mesh.txt):");
        this->modalInputString = this->loadedFilePath.empty() ? "mesh.txt" : this->loadedFilePath;

    }

    void drawRoundedPanel(float x, float y, float w, float h, sf::Color fillColor, sf::Color borderColor) {

        sf::RectangleShape panel(sf::Vector2f(w, h));
        panel.setPosition(x, y);
        panel.setFillColor(fillColor);
        panel.setOutlineThickness(1.5f);
        panel.setOutlineColor(borderColor);
        window.draw(panel);

    }

    std::string formatWeight(double weight) {

        if (weight == (double)((int)weight)) {

            return std::to_string((int)weight);

        }

        int intPart = (int)weight;
        double frac = std::abs(weight - (double)intPart);
        int fracDigit = (int)(frac * 10.0 + 0.5);

        if (fracDigit >= 10) {

            intPart += (weight >= 0.0 ? 1 : -1);
            fracDigit = 0;

        }

        return std::to_string(intPart) + "." + std::to_string(fracDigit);

    }

    ////////////////////////////////////////////////////////////////////////////
    // Intro Animation - 16-frame sprite sheet
    ////////////////////////////////////////////////////////////////////////////

    void playIntroAnimation() {

        sf::Sprite introSprite(textureIntro);
        sf::Clock animationClock;

        for (int frameIndex = 0; frameIndex < 16; frameIndex++) {

            animationClock.restart();

            while (animationClock.getElapsedTime().asMilliseconds() < 120 && window.isOpen()) {

                sf::Event event;

                while (window.pollEvent(event)) {

                    if (event.type == sf::Event::Closed) {

                        window.close();
                        return;

                    }

                    if (event.type == sf::Event::KeyPressed) {

                        return;

                    }

                }

                window.clear(sf::Color(8, 12, 28));
                this->renderBackground();

                introSprite.setTextureRect(introFrames[frameIndex]);
                introSprite.setOrigin(
                    introFrames[frameIndex].width / 2.0f,
                    introFrames[frameIndex].height / 2.0f
                );
                introSprite.setPosition(640.0f, 360.0f);
                introSprite.setScale(1.2f, 1.2f);

                window.draw(introSprite);

                sf::Text introText("Initializing NeuroMesh Visualizer...", this->fontUi, 22);
                this->styleReadableText(introText, sf::Color(235, 240, 255));
                introText.setPosition(430.0f, 588.0f);
                window.draw(introText);

                sf::Text introCredit("Made by Ahmad Ali  |  i250817", this->fontUi, 16);
                this->styleReadableText(introCredit, sf::Color(255, 200, 90));
                introCredit.setPosition(455.0f, 622.0f);
                window.draw(introCredit);

                window.display();

            }

        }

    }

    ////////////////////////////////////////////////////////////////////////////
    // Render Live Mesh
    ////////////////////////////////////////////////////////////////////////////

    void renderMesh(NeuroMesh& neuralMesh) {

        Layer* currentLayer = neuralMesh.getHeadLayer();

        for (int layerOffset = 0; layerOffset < this->selectedLayerIndex && currentLayer != nullptr; layerOffset++) {

            currentLayer = currentLayer->next;

        }

        sf::Font& activeBodyFont = this->getActiveUiFont();

        float canvasX = this->isMenuExpanded ? 310.0f : 110.0f;
        float canvasY = 95.0f;
        float canvasW = this->isMenuExpanded ? 950.0f : 1150.0f;
        float canvasH = 575.0f;

        // Right side screen canvas: white glassmorphic styling
        this->drawRoundedPanel(
            canvasX, canvasY, canvasW, canvasH,
            sf::Color(255, 255, 255, 45),
            sf::Color(255, 255, 255, 170)
        );

        // Glassmorphic top sheen reflection highlight
        sf::RectangleShape glassSheen(sf::Vector2f(canvasW - 4.0f, 2.0f));
        glassSheen.setPosition(canvasX + 2.0f, canvasY + 2.0f);
        glassSheen.setFillColor(sf::Color(255, 255, 255, 190));
        window.draw(glassSheen);

        if (currentLayer == nullptr) {

            sf::Text emptyText("No mesh loaded — use Load File from the menu.", fontHeading, 24);
            emptyText.setFillColor(sf::Color(200, 210, 230));
            emptyText.setPosition(canvasX + 180.0f, canvasY + canvasH / 2.0f - 20.0f);
            window.draw(emptyText);

            sf::Text hintText("Default: mesh.txt  |  Or enter any path in Load File dialog", activeBodyFont, 16);
            hintText.setFillColor(sf::Color(120, 140, 170));
            hintText.setPosition(canvasX + 200.0f, canvasY + canvasH / 2.0f + 20.0f);
            window.draw(hintText);
            return;

        }

        int gridDimension = neuralMesh.getN();

        if (gridDimension <= 0) gridDimension = currentLayer->N;
        if (gridDimension <= 0) gridDimension = 3;

        float startPositionX = canvasX + 40.0f;
        float startPositionY = canvasY + 35.0f;
        float spacingX = (canvasW - 80.0f) / (float)gridDimension;
        float spacingY = (canvasH - 70.0f) / (float)gridDimension;
        float neuronRadius = (gridDimension >= 5) ? 24.0f : (gridDimension >= 4) ? 28.0f : 34.0f;

        struct NeuronPosition {
            Neuron* neuronPointer;
            sf::Vector2f screenPosition;
        } positionBuffer[128];

        int positionCount = 0;

        for (int rowIndex = 0; rowIndex < gridDimension; rowIndex++) {

            for (int columnIndex = 0; columnIndex < gridDimension; columnIndex++) {

                Neuron* slotNeuron = neuralMesh.debugGetNeuronAt(
                    this->selectedLayerIndex,
                    rowIndex,
                    columnIndex
                );

                if (slotNeuron != nullptr && positionCount < 128) {

                    positionBuffer[positionCount].neuronPointer = slotNeuron;
                    positionBuffer[positionCount].screenPosition = sf::Vector2f(
                        startPositionX + columnIndex * spacingX + neuronRadius,
                        startPositionY + rowIndex * spacingY + neuronRadius
                    );

                    positionCount++;

                }

            }

        }

        bool hasCrossLayerLink[128];
        bool crossLayerActive[128];

        for (int initIndex = 0; initIndex < 128; initIndex++) {

            hasCrossLayerLink[initIndex] = false;
            crossLayerActive[initIndex] = false;

        }

        for (int posIndex = 0; posIndex < positionCount; posIndex++) {

            Neuron* sourceNeuron = positionBuffer[posIndex].neuronPointer;
            sf::Vector2f sourcePosition = positionBuffer[posIndex].screenPosition;

            Synapse* axonRunner = sourceNeuron->headAxon;

            while (axonRunner != nullptr) {

                Neuron* targetNeuron = axonRunner->targetNeuron;

                if (targetNeuron != nullptr) {

                    sf::Vector2f targetPosition;
                    bool targetOnScreen = false;

                    for (int searchIndex = 0; searchIndex < positionCount; searchIndex++) {

                        if (positionBuffer[searchIndex].neuronPointer == targetNeuron) {

                            targetPosition = positionBuffer[searchIndex].screenPosition;
                            targetOnScreen = true;
                            break;

                        }

                    }

                    if (targetOnScreen) {

                        int sourcePathIndex = this->indexOnPropagationPath(sourceNeuron);
                        int targetPathIndex = this->indexOnPropagationPath(targetNeuron);
                        bool pathHop = (sourcePathIndex >= 0 && targetPathIndex == sourcePathIndex + 1);

                        sf::Color lineColor = sf::Color(90, 150, 220, 120);

                        if (axonRunner->isActive) {

                            lineColor = sf::Color(255, 150, 60, 240);

                        }

                        if (pathHop && this->isPropagationTrailNode(targetPathIndex)) {

                            if (this->propagationPhase == 3) {

                                lineColor = sf::Color(255, 70, 150, 255);

                            } else {

                                lineColor = sf::Color(255, 180, 40, 255);

                            }

                        }

                        sf::Vertex connectionLine[] = {
                            sf::Vertex(sourcePosition, lineColor),
                            sf::Vertex(targetPosition, lineColor)
                        };

                        window.draw(connectionLine, 2, sf::Lines);

                    } else {

                        hasCrossLayerLink[posIndex] = true;

                        if (axonRunner->isActive) {

                            crossLayerActive[posIndex] = true;

                        }

                    }

                }

                axonRunner = axonRunner->nextSynapse;

            }

        }

        // Draw intra-layer links
        for (int posIndex = 0; posIndex < positionCount; posIndex++) {

            Neuron* sourceNeuron = positionBuffer[posIndex].neuronPointer;
            sf::Vector2f sourcePosition = positionBuffer[posIndex].screenPosition;

            Neuron* rightNeighbor = sourceNeuron->right;
            Neuron* downNeighbor = sourceNeuron->down;

            if (rightNeighbor != nullptr) {

                for (int searchIndex = 0; searchIndex < positionCount; searchIndex++) {

                    if (positionBuffer[searchIndex].neuronPointer == rightNeighbor) {

                        sf::Vertex gridLine[] = {
                            sf::Vertex(sourcePosition, sf::Color(55, 75, 110, 100)),
                            sf::Vertex(positionBuffer[searchIndex].screenPosition, sf::Color(55, 75, 110, 100))
                        };

                        window.draw(gridLine, 2, sf::Lines);
                        break;

                    }

                }

            }

            if (downNeighbor != nullptr) {

                for (int searchIndex = 0; searchIndex < positionCount; searchIndex++) {

                    if (positionBuffer[searchIndex].neuronPointer == downNeighbor) {

                        sf::Vertex gridLine[] = {
                            sf::Vertex(sourcePosition, sf::Color(55, 75, 110, 100)),
                            sf::Vertex(positionBuffer[searchIndex].screenPosition, sf::Color(55, 75, 110, 100))
                        };

                        window.draw(gridLine, 2, sf::Lines);
                        break;

                    }

                }

            }

        }

        double maxWeightOnLayer = 0.0;

        for (int posIndex = 0; posIndex < positionCount; posIndex++) {

            if (positionBuffer[posIndex].neuronPointer->weight > maxWeightOnLayer) {

                maxWeightOnLayer = positionBuffer[posIndex].neuronPointer->weight;

            }

        }

        // Draw neuron nodes
        for (int posIndex = 0; posIndex < positionCount; posIndex++) {

            Neuron* displayNeuron = positionBuffer[posIndex].neuronPointer;
            sf::Vector2f displayPosition = positionBuffer[posIndex].screenPosition;

            int pathIndex = this->indexOnPropagationPath(displayNeuron);
            bool onTrail = this->isPropagationTrailNode(pathIndex);
            bool isFocusNode = (pathIndex >= 0 && pathIndex == this->propagationFocusIndex && this->propagationPhase != 0);

            sf::Color nodeOutline = themeAccent();
            sf::Color nodeFill = this->weightToNodeFill(displayNeuron->weight, maxWeightOnLayer);
            sf::Color glowTint = sf::Color(themeAccent().r, themeAccent().g, themeAccent().b, 30);

            if (onTrail) {

                nodeOutline = themeAccentHot();
                glowTint = sf::Color(255, 150, 50, 55);

            }

            if (isFocusNode) {

                if (this->propagationPhase == 3) {

                    nodeOutline = themeFound();
                    glowTint = sf::Color(255, 60, 140, 90);

                } else {

                    nodeOutline = sf::Color(255, 200, 60);
                    glowTint = sf::Color(255, 180, 40, 80);

                }

            }

            sf::CircleShape glowRing(neuronRadius + (isFocusNode ? 9.0f : 5.0f));
            glowRing.setOrigin(glowRing.getRadius(), glowRing.getRadius());
            glowRing.setPosition(displayPosition);
            glowRing.setFillColor(glowTint);
            window.draw(glowRing);

            sf::CircleShape neuronCircle(neuronRadius);
            neuronCircle.setOrigin(neuronRadius, neuronRadius);
            neuronCircle.setPosition(displayPosition);
            neuronCircle.setFillColor(nodeFill);
            neuronCircle.setOutlineThickness(isFocusNode ? 3.5f : 2.5f);
            neuronCircle.setOutlineColor(nodeOutline);
            window.draw(neuronCircle);

            sf::CircleShape innerDisc(neuronRadius * 0.55f);
            innerDisc.setOrigin(neuronRadius * 0.55f, neuronRadius * 0.55f);
            innerDisc.setPosition(displayPosition);
            innerDisc.setFillColor(isFocusNode ? sf::Color(255, 230, 240, 230) : sf::Color(242, 246, 255, 215));
            window.draw(innerDisc);

            this->drawNeuronLabelPlate(displayPosition, neuronRadius);

            unsigned int labelFontSize = (unsigned int)(neuronRadius * 0.38f);
            if (labelFontSize < 11) labelFontSize = 11;

            std::string idText = "ID:" + std::to_string(displayNeuron->id);

            sf::Text idLabel(idText, this->fontUi, labelFontSize);
            idLabel.setFillColor(sf::Color(15, 23, 42));
            idLabel.setOutlineThickness(0.5f);
            idLabel.setOutlineColor(sf::Color(255, 255, 255, 220));
            idLabel.setOrigin(
                idLabel.getLocalBounds().width / 2.0f,
                idLabel.getLocalBounds().height
            );
            idLabel.setPosition(displayPosition.x, displayPosition.y - 2.0f);
            window.draw(idLabel);

            sf::Text weightLabel(this->formatWeight(displayNeuron->weight), this->fontUi, labelFontSize);
            weightLabel.setFillColor(sf::Color(190, 50, 10));
            weightLabel.setOutlineThickness(0.5f);
            weightLabel.setOutlineColor(sf::Color(255, 255, 255, 220));
            weightLabel.setOrigin(
                weightLabel.getLocalBounds().width / 2.0f,
                0.0f
            );
            weightLabel.setPosition(displayPosition.x, displayPosition.y + 2.0f);
            window.draw(weightLabel);

            if (hasCrossLayerLink[posIndex]) {

                this->drawCrossLayerBadge(displayPosition, neuronRadius, crossLayerActive[posIndex]);

            }

        }

        std::string layerCaption = "Layer " + std::to_string(this->selectedLayerIndex)
            + "  |  " + std::to_string(currentLayer->currentCount) + " / " + std::to_string(gridDimension * gridDimension) + " neurons";

        if (!this->loadedFilePath.empty()) {

            layerCaption += "  |  " + this->loadedFilePath;

        }

        sf::Text layerLabel(layerCaption, activeBodyFont, 14);
        layerLabel.setFillColor(sf::Color(240, 245, 255));
        layerLabel.setOutlineThickness(1.0f);
        layerLabel.setOutlineColor(sf::Color(10, 15, 30, 200));
        layerLabel.setPosition(canvasX + 15.0f, canvasY + canvasH - 28.0f);
        window.draw(layerLabel);

    }

    ////////////////////////////////////////////////////////////////////////////
    // Render Side Menu
    ////////////////////////////////////////////////////////////////////////////

    void renderUI(NeuroMesh& neuralMesh) {

        sf::Font& activeBodyFont = this->usePrimaryBodyFont ? fontBodyPrimary : fontBodySecondary;

        int totalLayers = this->countTotalLayers(neuralMesh);

        sf::Text titleText("NeuroMesh Explorer", fontHeading, 32);
        titleText.setFillColor(sf::Color(240, 245, 255));
        titleText.setPosition(this->isMenuExpanded ? 320.0f : 120.0f, 18.0f);
        window.draw(titleText);

        sf::Text subtitleText("FAST NUCES  |  DS A1  |  Fall 2026", activeBodyFont, 14);
        subtitleText.setFillColor(themeAccent());
        subtitleText.setPosition(this->isMenuExpanded ? 320.0f : 120.0f, 54.0f);
        window.draw(subtitleText);

        std::string infoString = "Grid N=" + std::to_string(neuralMesh.getN())
            + "  |  Layer " + std::to_string(this->selectedLayerIndex + 1) + " of " + std::to_string(totalLayers > 0 ? totalLayers : 1)
            + "  |  Prune: " + std::to_string((int)neuralMesh.getPruningThreshold())
            + "  |  Fusion: " + std::to_string((int)neuralMesh.getFusionThreshold());

        sf::Text infoText(infoString, activeBodyFont, 14);
        infoText.setFillColor(sf::Color(150, 170, 200));
        infoText.setPosition(this->isMenuExpanded ? 320.0f : 120.0f, 74.0f);
        window.draw(infoText);

        sf::RectangleShape statusBar(sf::Vector2f(1280.0f, 38.0f));
        statusBar.setPosition(0.0f, 682.0f);
        statusBar.setFillColor(sf::Color(8, 14, 28, 240));
        statusBar.setOutlineThickness(1.0f);
        statusBar.setOutlineColor(sf::Color(themeAccent().r, themeAccent().g, themeAccent().b, 100));
        window.draw(statusBar);

        sf::CircleShape statusDot(5.0f);
        statusDot.setOrigin(5.0f, 5.0f);
        statusDot.setPosition(18.0f, 701.0f);
        statusDot.setFillColor(themeAccentHot());
        window.draw(statusDot);

        sf::Text statusDisplay(this->statusText, this->fontUi, 13);
        this->styleReadableText(statusDisplay, sf::Color(200, 210, 230));
        statusDisplay.setPosition(32.0f, 692.0f);
        window.draw(statusDisplay);

        this->drawAuthorCredit();

        sf::RectangleShape toggleButton(sf::Vector2f(42.0f, 36.0f));
        toggleButton.setPosition(18.0f, 18.0f);
        toggleButton.setFillColor(sf::Color(25, 40, 75, 230));
        toggleButton.setOutlineThickness(1.5f);
        toggleButton.setOutlineColor(themeAccent());
        window.draw(toggleButton);

        sf::Text arrowSymbol(this->isMenuExpanded ? "<" : ">", fontHeading, 22);
        arrowSymbol.setFillColor(themeAccent());
        arrowSymbol.setPosition(30.0f, 20.0f);
        window.draw(arrowSymbol);

        if (this->isMenuExpanded) {

            this->drawRoundedPanel(
                12.0f, 68.0f, 288.0f, 600.0f,
                sf::Color(14, 22, 42, 235),
                sf::Color(themeAccent().r, themeAccent().g, themeAccent().b, 90)
            );

            sf::Text menuHeader("CONTROLS", fontHeading, 18);
            menuHeader.setFillColor(themeAccent());
            menuHeader.setPosition(28.0f, 78.0f);
            window.draw(menuHeader);

            const char* menuButtonLabels[] = {
                "Load Mesh File",
                "Insert Neuron",
                "Delete Neuron",
                "Prune Neuron",
                "Merge Columns",
                "Merge Layers",
                "Forward Propagate",
                "Backward Propagate",
                "Previous Layer",
                "Next Layer",
                "Switch Font",
                "Export Mesh"
            };

            for (int buttonIndex = 0; buttonIndex < 12; buttonIndex++) {

                float buttonY = 108.0f + buttonIndex * 46.0f;
                bool isHovered = (buttonIndex == this->hoveredMenuButton);

                sf::RectangleShape menuButton(sf::Vector2f(264.0f, 38.0f));
                menuButton.setPosition(24.0f, buttonY);
                menuButton.setFillColor(isHovered ? sf::Color(32, 52, 95, 240) : sf::Color(22, 35, 65, 220));
                menuButton.setOutlineThickness(isHovered ? 1.5f : 1.0f);
                menuButton.setOutlineColor(isHovered ? sf::Color(themeAccent().r, themeAccent().g, themeAccent().b, 180) : sf::Color(50, 70, 110));
                window.draw(menuButton);

                sf::Text indexBadge(std::to_string(buttonIndex + 1), activeBodyFont, 12);
                indexBadge.setFillColor(isHovered ? sf::Color(255, 200, 80) : themeAccent());
                indexBadge.setPosition(34.0f, buttonY + 11.0f);
                window.draw(indexBadge);

                sf::Text buttonText(menuButtonLabels[buttonIndex], activeBodyFont, 13);
                buttonText.setFillColor(isHovered ? sf::Color(240, 248, 255) : sf::Color(210, 220, 240));
                buttonText.setPosition(52.0f, buttonY + 10.0f);
                window.draw(buttonText);

            }

            this->drawRoundedPanel(
                24.0f, 638.0f, 264.0f, 48.0f,
                sf::Color(16, 26, 48, 240),
                sf::Color(255, 200, 90, 120)
            );

            sf::Text sidebarCreditLabel("Made by", this->fontUi, 11);
            this->styleReadableText(sidebarCreditLabel, sf::Color(150, 170, 200));
            sidebarCreditLabel.setPosition(36.0f, 644.0f);
            window.draw(sidebarCreditLabel);

            sf::Text sidebarAuthor("Ahmad Ali", this->fontUi, 15);
            this->styleReadableText(sidebarAuthor, sf::Color(255, 214, 96));
            sidebarAuthor.setPosition(36.0f, 660.0f);
            window.draw(sidebarAuthor);

            sf::Text sidebarId("i250817", this->fontUi, 13);
            this->styleReadableText(sidebarId, themeAccent());
            sidebarId.setPosition(200.0f, 662.0f);
            window.draw(sidebarId);

        }

    }

    ////////////////////////////////////////////////////////////////////////////
    // Render Modal Dialog
    ////////////////////////////////////////////////////////////////////////////

    void renderModal() {

        sf::Font& activeBodyFont = this->usePrimaryBodyFont ? fontBodyPrimary : fontBodySecondary;

        sf::RectangleShape dimOverlay(sf::Vector2f(1280.0f, 720.0f));
        dimOverlay.setFillColor(sf::Color(0, 0, 0, 140));
        window.draw(dimOverlay);

        this->drawRoundedPanel(
            370.0f, 240.0f, 540.0f, 220.0f,
            sf::Color(16, 26, 48, 250),
            themeAccent()
        );

        sf::Text modalTitleText(this->modalTitle, fontHeading, 24);
        modalTitleText.setFillColor(sf::Color(240, 245, 255));
        modalTitleText.setPosition(395.0f, 258.0f);
        window.draw(modalTitleText);

        sf::Text promptText(this->modalPromptLine, activeBodyFont, 15);
        promptText.setFillColor(sf::Color(150, 170, 200));
        promptText.setPosition(395.0f, 300.0f);
        window.draw(promptText);

        sf::RectangleShape inputBox(sf::Vector2f(490.0f, 40.0f));
        inputBox.setPosition(385.0f, 330.0f);
        inputBox.setFillColor(sf::Color(8, 14, 28));
        inputBox.setOutlineThickness(2.0f);
        inputBox.setOutlineColor(themeAccent());
        window.draw(inputBox);

        sf::Text inputDisplayText(this->modalInputString + "|", activeBodyFont, 18);
        inputDisplayText.setFillColor(sf::Color(230, 240, 255));
        inputDisplayText.setPosition(398.0f, 338.0f);
        window.draw(inputDisplayText);

        sf::Text helpText("ENTER to confirm   |   ESC to cancel", activeBodyFont, 13);
        helpText.setFillColor(sf::Color(100, 120, 150));
        helpText.setPosition(395.0f, 410.0f);
        window.draw(helpText);

    }

    ////////////////////////////////////////////////////////////////////////////
    // Modal Helpers
    ////////////////////////////////////////////////////////////////////////////

    void openModal(int modalKind, const std::string& title, const std::string& promptLine) {

        this->activeModal = modalKind;
        this->modalTitle = title;
        this->modalPromptLine = promptLine;
        this->modalInputString = "";

    }

    void closeModal() {

        this->activeModal = modalNone;
        this->modalInputString = "";
        this->modalTitle = "";
        this->modalPromptLine = "";

    }

    void submitModal(NeuroMesh& neuralMesh) {

        if (this->modalInputString.empty()) {

            this->closeModal();
            this->statusText = "Input cancelled (empty).";
            return;

        }

        if (this->activeModal == modalInsertId) {

            this->pendingInsertId = atoi(this->modalInputString.c_str());
            this->closeModal();
            this->openModal(modalInsertWeight, "Insert Neuron", "Enter weight (decimal number):");

        } else if (this->activeModal == modalInsertWeight) {

            double neuronWeight = atof(this->modalInputString.c_str());
            neuralMesh.insertNeuron(this->pendingInsertId, neuronWeight);
            this->statusText = "Inserted Neuron ID " + std::to_string(this->pendingInsertId) + " Weight " + this->modalInputString;
            this->pendingInsertId = -1;
            this->closeModal();

        } else if (this->activeModal == modalDeleteId) {

            int neuronId = atoi(this->modalInputString.c_str());
            neuralMesh.deleteNeuron(neuronId);
            this->statusText = "Deleted Neuron ID " + this->modalInputString;
            this->closeModal();

        } else if (this->activeModal == modalPruneId) {

            int neuronId = atoi(this->modalInputString.c_str());
            Neuron* targetNeuron = neuralMesh.findNeuronById(neuronId);

            if (targetNeuron != nullptr) {

                neuralMesh.pruneNeuron(targetNeuron);
                this->statusText = "Pruned Neuron ID " + this->modalInputString + " (Weight halved, neighbors cleared)";

            } else {

                this->statusText = "Neuron ID " + this->modalInputString + " not found for pruning.";

            }

            this->closeModal();

        } else if (this->activeModal == modalForwardLetter) {

            char inputLetter = this->modalInputString[0];
            char outputLetter = neuralMesh.forwardPropagate(inputLetter);
            this->statusText = std::string("Forward: '") + inputLetter + "' -> '" + outputLetter
                + "'  |  Signal sweeping path...";
            this->startForwardPropagationVisual(neuralMesh);
            this->closeModal();

        } else if (this->activeModal == modalBackwardLetter) {

            char targetLetter = this->modalInputString[0];

            if (!neuralMesh.getHasForwardRun()) {

                this->statusText = "Run Forward Propagate first, then Backward.";
                this->closeModal();

            } else {

                neuralMesh.backwardPropagate(targetLetter);
                this->statusText = std::string("Backward toward '") + targetLetter + "' — locating path...";
                this->startBackwardPropagationVisual(neuralMesh);
                this->closeModal();

            }

        } else if (this->activeModal == modalLoadFile) {

            std::string filePath = this->modalInputString;
            std::string targetPath = std::ifstream(filePath.c_str()).good() ? filePath : resolveAssetPath(filePath);

            if (this->meshFileExists(targetPath)) {

                neuralMesh.loadFromFile(targetPath);
                this->loadedFilePath = targetPath;
                this->selectedLayerIndex = 0;
                this->statusText = "Loaded mesh from " + targetPath + "  |  Press L to reload another file";

            } else {

                this->statusText = "File not found: " + filePath;

            }

            this->closeModal();

        } else {

            this->closeModal();

        }

    }

    ////////////////////////////////////////////////////////////////////////////
    // Mouse Interaction
    ////////////////////////////////////////////////////////////////////////////

    void handleMouseClick(float mouseX, float mouseY, NeuroMesh& neuralMesh) {

        if (this->activeModal != modalNone) {

            bool clickInsideModal = (mouseX >= 370.0f && mouseX <= 910.0f &&
                                     mouseY >= 240.0f && mouseY <= 460.0f);

            if (!clickInsideModal) {

                this->closeModal();
                this->statusText = "Modal dismissed.";

            }

            return;

        }

        if (mouseX >= 20.0f && mouseX <= 65.0f && mouseY >= 20.0f && mouseY <= 58.0f) {

            this->isMenuExpanded = !this->isMenuExpanded;
            return;

        }

        if (!this->isMenuExpanded || mouseX < 25.0f || mouseX > 285.0f) {

            return;

        }

        if (mouseY >= 108.0f && mouseY <= 146.0f) {

            // 1. Load File
            this->openLoadFileModal();

        } else if (mouseY >= 154.0f && mouseY <= 192.0f) {

            // 2. Insert Neuron
            this->openModal(modalInsertId, "Insert Neuron", "Enter neuron ID (integer):");

        } else if (mouseY >= 200.0f && mouseY <= 238.0f) {

            // 3. Delete Neuron
            this->openModal(modalDeleteId, "Delete Neuron", "Enter neuron ID to delete:");

        } else if (mouseY >= 246.0f && mouseY <= 284.0f) {

            // 4. Prune Neuron
            this->openModal(modalPruneId, "Prune Neuron", "Enter neuron ID to prune:");

        } else if (mouseY >= 292.0f && mouseY <= 330.0f) {

            // 5. Merge Columns
            Layer* targetLayer = neuralMesh.getHeadLayer();

            for (int offset = 0; offset < this->selectedLayerIndex && targetLayer != nullptr; offset++) {

                targetLayer = targetLayer->next;

            }

            if (targetLayer != nullptr) {

                Neuron* topA = nullptr;
                Neuron* topB = nullptr;
                int foundCol = -1;

                for (int c = 0; c < neuralMesh.getN() - 1; c++) {

                    Neuron* a = neuralMesh.getColumnTop(targetLayer, c);
                    Neuron* b = neuralMesh.getColumnTop(targetLayer, c + 1);

                    if (a != nullptr && b != nullptr) {

                        topA = a;
                        topB = b;
                        foundCol = c;
                        break;

                    }

                }

                if (topA != nullptr && topB != nullptr) {

                    neuralMesh.mergeColumns(topA, topB);
                    this->statusText = "Merged columns " + std::to_string(foundCol) + " & " + std::to_string(foundCol + 1) + " in layer " + std::to_string(this->selectedLayerIndex);

                } else {

                    this->statusText = "Merge columns: need 2 adjacent columns with neurons in layer " + std::to_string(this->selectedLayerIndex);

                }

            } else {

                this->statusText = "Merge columns: selected layer not found.";

            }

        } else if (mouseY >= 338.0f && mouseY <= 376.0f) {

            // 6. Merge Layers
            Layer* targetLayer = neuralMesh.getHeadLayer();

            for (int offset = 0; offset < this->selectedLayerIndex && targetLayer != nullptr; offset++) {

                targetLayer = targetLayer->next;

            }

            if (targetLayer != nullptr && targetLayer->next != nullptr) {

                neuralMesh.mergeLayers(targetLayer, targetLayer->next);
                int totalLayers = countTotalLayers(neuralMesh);
                if (this->selectedLayerIndex >= totalLayers && totalLayers > 0) {
                    this->selectedLayerIndex = totalLayers - 1;
                }
                this->statusText = "Merged layer with next layer. Total layers remaining: " + std::to_string(totalLayers);

            } else {

                this->statusText = "Merge layers: need a next layer to merge with.";

            }

        } else if (mouseY >= 384.0f && mouseY <= 422.0f) {

            // 7. Forward Propagate
            this->openModal(modalForwardLetter, "Forward Propagate", "Enter input letter (A-Z):");

        } else if (mouseY >= 430.0f && mouseY <= 468.0f) {

            // 8. Backward Propagate
            this->openModal(modalBackwardLetter, "Backward Propagate", "Enter target letter (A-Z):");

        } else if (mouseY >= 476.0f && mouseY <= 514.0f) {

            // 9. Previous Layer
            if (this->selectedLayerIndex > 0) {

                this->selectedLayerIndex--;

            }

            this->statusText = "Viewing Layer " + std::to_string(this->selectedLayerIndex);

        } else if (mouseY >= 522.0f && mouseY <= 560.0f) {

            // 10. Next Layer
            int totalLayerCount = 0;
            Layer* layerCounter = neuralMesh.getHeadLayer();

            while (layerCounter != nullptr) {

                totalLayerCount++;
                layerCounter = layerCounter->next;

            }

            if (this->selectedLayerIndex < totalLayerCount - 1) {

                this->selectedLayerIndex++;

            }

            this->statusText = "Viewing Layer " + std::to_string(this->selectedLayerIndex);

        } else if (mouseY >= 568.0f && mouseY <= 606.0f) {

            // 11. Switch Font
            this->usePrimaryBodyFont = !this->usePrimaryBodyFont;
            this->statusText = this->usePrimaryBodyFont ? "Font: Atsuka Montreal" : "Font: Modern Prestige";

        } else if (mouseY >= 614.0f && mouseY <= 652.0f) {

            // 12. Export Mesh
            neuralMesh.exportMesh("mesh_exported.txt");
            this->statusText = "Exported mesh to mesh_exported.txt";

        }

    }

};
