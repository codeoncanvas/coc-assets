/**
 *
 *      ┌─┐╔═╗┌┬┐┌─┐
 *      │  ║ ║ ││├┤
 *      └─┘╚═╝─┴┘└─┘
 *   ┌─┐┌─┐╔╗╔┬  ┬┌─┐┌─┐
 *   │  ├─┤║║║└┐┌┘├─┤└─┐
 *   └─┘┴ ┴╝╚╝ └┘ ┴ ┴└─┘
 *
 * Copyright (c) 2016 Code on Canvas Pty Ltd, http://CodeOnCanvas.cc
 *
 **/

#include "cocAssetsCI.h"

#if defined( COC_CI )

namespace coc {

AssetsCI::AssetsCI() : coc::Assets() {
    //
}

AssetsCI::~AssetsCI() {
    //
}

//--------------------------------------------------------------
void AssetsCI::update(float timeDelta) {
    Assets::update(timeDelta);

    //
}

//--------------------------------------------------------------
ci::gl::TextureRef AssetsCI::getTexture(std::string assetID) {
    AssetTextureCI * asset = (AssetTextureCI *)getAssetPtr(assetID);
    if(asset == NULL) {
        return NULL;
    }

    if(asset->bLoaded == false) {
        return NULL;
    }
    
    return asset->textureRef;
}

//--------------------------------------------------------------
AssetTexture * AssetsCI::initTexture() {
    return new AssetTextureCI();
}

void AssetsCI::killTexture(AssetTexture * asset) {
    delete (AssetTextureCI *)asset;
}

//--------------------------------------------------------------
AssetSound * AssetsCI::initSound() {
    return new AssetSoundCI();
}

void AssetsCI::killSound(AssetSound * asset) {
    delete (AssetSoundCI *)asset;
}

//--------------------------------------------------------------
void AssetsCI::loadTexture(std::string assetID) {
    AssetTextureCI * asset = (AssetTextureCI *)getAssetPtr(assetID);
    if(asset == NULL) {
        return;
    }

    asset->textureRef = ci::gl::Texture::create(ci::loadImage(asset->assetPath));
    asset->bLoaded = (asset->textureRef.get() != NULL);
}

void AssetsCI::unloadTexture(std::string assetID) {
    AssetTextureCI * asset = (AssetTextureCI *)getAssetPtr(assetID);
    if(asset == NULL) {
        return;
    }
    
    asset->textureRef = NULL;
    asset->bLoaded = false;
}

//--------------------------------------------------------------
void AssetsCI::loadSound(std::string assetID) {
    AssetSoundCI * asset = (AssetSoundCI *)getAssetPtr(assetID);
    if(asset == NULL) {
        return;
    }

    asset->sourceFileRef = ci::audio::load(ci::app::loadAsset(asset->assetPath));
    asset->voiceSamplePlayerNodeRef = ci::audio::Voice::create(asset->sourceFileRef);
    asset->samplePlayerNodeRef = asset->voiceSamplePlayerNodeRef->getSamplePlayerNode();
    asset->bLoaded = (asset->sourceFileRef.get() != NULL);
}

void AssetsCI::unloadSound(std::string assetID) {
    AssetSoundCI * asset = (AssetSoundCI *)getAssetPtr(assetID);
    if(asset == NULL) {
        return;
    }

    asset->samplePlayerNodeRef = NULL;
    asset->voiceSamplePlayerNodeRef = NULL;
    asset->sourceFileRef = NULL;
}

};

#endif