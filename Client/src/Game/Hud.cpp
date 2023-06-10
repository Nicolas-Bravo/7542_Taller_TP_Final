#include "Hud.h"
#include "Configuration.h"
#include <iostream>

Hud::Hud(TypeOperator type, Renderer& render, Font& font) : type(type),
    renderHud(render), healthInit(0), fontHud(font) {
    this->loadTextures();
    switch (type) {
        case TypeOperator::operator_idf:
            healthInit = CF::idf_health;
            break;
        case TypeOperator::operator_p90:
            healthInit = CF::p90_health;
            break;
        case TypeOperator::operator_scout:
            healthInit = CF::scout_health;
            break;    
        default:
            break;
    }
}

void Hud::loadTextures() {
    std::string path = "assets/images/sdl/hud/";
    SDL_Color c = {0, 0, 0, 255};
    texturesHud["healthbg1"] = std::make_unique<Texture>(
                                    renderHud, path + "healthbg_1.png",c);
    texturesHud["healthbg2"] = std::make_unique<Texture>(
                                    renderHud, path + "healthbg_2.png", c);
    texturesHud["healthbg3"] = std::make_unique<Texture>(
                                    renderHud, path + "healthbg_3.png", c);
    texturesHud["healthbg4"] = std::make_unique<Texture>(
                                    renderHud, path + "healthbg_4.png", c);
    texturesHud["bar-bg"] = std::make_unique<Texture>(
                                    renderHud, path + "bar_bg.png", false);
    texturesHud["bar-fill"] = std::make_unique<Texture>(
                                    renderHud, path + "bar_fill.png", false);
    texturesHud["health-icon"] = std::make_unique<Texture>(
                                    renderHud, path + "health_icon.png", false);
    texturesHud["rifle"] = std::make_unique<Texture>(
                                    renderHud, path + "icon_rifle.png", false);
    texturesHud["hunting"] = std::make_unique<Texture>(
                                    renderHud, path + "icon_hunting.png", false);
    texturesHud["bullet-rifle"] = std::make_unique<Texture>(
                                    renderHud, path + "bullet_rifle.png", false);
    texturesHud["bullet-hunting"] = std::make_unique<Texture>(
                                    renderHud, path + "bullet_hunting.png", false);
}

void Hud::render(uint8_t healthPlayer, int numBullet) {
    this->renderBg();
    this->renderHealthBar();
    this->renderHealthIcon();
    this->renderHealthFill(healthPlayer);
    this->renderIconWeapon();
    this->renderIconBullet();
    this->renderNumBullet(numBullet);
}

void Hud::renderBg() {
    SDL_Rect rectInit = {   0,
                            0,
                            512,
                            256};
    SDL_Rect rectFinal = {  0,
                            0,
                            512 * 6 / 5,
                            256 * 8 / 10};
    this->renderHud.copy(texturesHud["healthbg1"]->getTexture(), rectInit,
                        rectFinal);
    this->renderHud.copy(texturesHud["healthbg2"]->getTexture(), rectInit,
                        rectFinal);
    this->renderHud.copy(texturesHud["healthbg3"]->getTexture(), rectInit,
                        rectFinal);
    this->renderHud.copy(texturesHud["healthbg4"]->getTexture(), rectInit,
                        rectFinal);
}

void Hud::renderHealthBar() {
    SDL_Rect rectInit = {   0,
                            0,
                            512,
                            128};
    SDL_Rect rectFinal = {  30,
                            35,
                            512 * 6 / 10,
                            128 * 7 / 10};
    this->renderHud.copy(texturesHud["bar-bg"]->getTexture(), rectInit,
                        rectFinal);
}

void Hud::renderHealthIcon() {
    SDL_Rect rectInit = {   0,
                            0,
                            64,
                            64};
    SDL_Rect rectFinal = {  30,
                            54,
                            64 * 6 / 10,
                            64 * 6 / 10};
    this->renderHud.copy(texturesHud["health-icon"]->getTexture(), rectInit,
                        rectFinal);
}

void Hud::renderHealthFill(uint8_t healthPlayer) {
    SDL_Rect rectInit = {   0,
                            0,
                            512,
                            64};
    int sizeFill = 0;
    switch (type) {
        case TypeOperator::operator_idf:
            sizeFill = 512 * 149 / 200;
            break;
        case TypeOperator::operator_p90:
            sizeFill = 512 * 87 / 125;
            break;
        case TypeOperator::operator_scout:
            sizeFill = 512 * 199 / 200;
            break;
        default:
            break;
    }
    SDL_Rect rectFinal = {  68,
                            65,
                            sizeFill * healthPlayer / healthInit,
                            64 * 8/10};
    this->renderHud.copy(texturesHud["bar-fill"]->getTexture(), rectInit,
                        rectFinal);
}

void Hud::renderIconWeapon() {
    SDL_Rect rectInit = {   0,
                            0,
                            512,
                            128};
    SDL_Rect rectFinal = {  68,
                            82,
                            512 * 1 / 4,
                            128 * 1 / 4};
    if (type == TypeOperator::operator_scout)
        this->renderHud.copy(texturesHud["hunting"]->getTexture(), rectInit,
                            rectFinal, SDL_FLIP_HORIZONTAL);
    else if (type == TypeOperator::operator_idf || 
            type == TypeOperator::operator_p90)
        this->renderHud.copy(texturesHud["rifle"]->getTexture(), rectInit,
                            rectFinal, SDL_FLIP_HORIZONTAL);
}

void Hud::renderIconBullet() {
    SDL_Rect rectInit;
    SDL_Rect rectFinal = { 200, 82, 32, 32};
    if (type == TypeOperator::operator_scout){
        rectInit = {0, 0, 64, 64};
        this->renderHud.copy(texturesHud["bullet-hunting"]->getTexture(),
                        rectInit, rectFinal);
    } else if (type == TypeOperator::operator_idf || 
            type == TypeOperator::operator_p90) {
        rectInit = {0, 0, 32, 32};
        this->renderHud.copy(texturesHud["bullet-rifle"]->getTexture(),
                            rectInit, rectFinal);
    }
}

void Hud::renderNumBullet(int numBullet) {
    SDL_Color color = {255, 255, 255, 255};
    int w, h;
    std::string text = std::to_string(numBullet);
    this->fontHud.getSizeFont(text, &w, &h);
    Texture textureFont(renderHud, fontHud.RenderText_Solid(text, color));
    SDL_Rect final = { 180, 88, w, h};
    this->renderHud.copyFont(textureFont.getTexture(), final);
}

Hud::~Hud() {
}
