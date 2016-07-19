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

#include "cocAssets.h"

namespace coc {

//--------------------------------------------------------------
Assets::Assets() {
    //
}

Assets::~Assets() {
    //
}

//--------------------------------------------------------------
const Asset * Assets::addAsset(std::string assetPath, AssetType assetType, std::string assetID) {
    if(assetType == AssetTypeTexture) {
        assets.push_back(initTexture());
    } else if(assetType == AssetTypeSound) {
        assets.push_back(initSound());
    } else {
        return NULL;
    }
    Asset & asset = *assets.back();
    asset.assetPath = assetPath;
    asset.assetID = assetID;
    if(asset.assetID.length() == 0) {
        asset.assetID = asset.assetPath;
    }
    return &asset;
}

void Assets::removeAsset(std::string assetID) {
    for(int i=0; i<assets.size(); i++) {
        if(assets[i]->assetID != assetID) {
            continue;
        }
        if(assets[i]->type == AssetTypeTexture) {
            killTexture((AssetTexture *)assets[i]);
        } else if(assets[i]->type == AssetTypeSound) {
            killSound((AssetSound *)assets[i]);
        }
        assets.erase(assets.begin() + i);
        break;
    }
}

//--------------------------------------------------------------
void Assets::load(std::string assetID) {
    Asset * asset = getAssetPtr(assetID);
    if(asset == NULL) {
        return;
    }
    if(asset->bLoaded == true) {
        return;
    }
    if(asset->type == AssetTypeTexture) {
        loadTexture(assetID);
    } else if(asset->type == AssetTypeSound) {
        loadSound(assetID);
    }
}

void Assets::unload(std::string assetID) {
    Asset * asset = getAssetPtr(assetID);
    if(asset == NULL) {
        return;
    }
    if(asset->bLoaded == false) {
        return;
    }
    if(asset->type == AssetTypeTexture) {
        unloadTexture(assetID);
    } else if(asset->type == AssetTypeSound) {
        unloadSound(assetID);
    }
}

//--------------------------------------------------------------
void Assets::update(float timeDelta) {

    for(int i=0; i<assets.size(); i++) {
        Asset & asset = *assets[i];
        
        if(asset.bLoaded == false) {
            continue;
        }
        
        //
    }
}

//--------------------------------------------------------------
Asset * Assets::getAssetPtr(std::string assetID) const {
    for(int i=0; i<assets.size(); i++) {
        if(assets[i]->assetID == assetID) {
            return assets[i];
        }
    }
    return NULL;
}

//--------------------------------------------------------------
const Asset * Assets::getAsset(std::string assetID) const {
    return getAssetPtr(assetID);
}

//--------------------------------------------------------------
AssetTexture * Assets::initTexture() {
    return new AssetTexture();
}

void Assets::killTexture(AssetTexture * asset) {
    delete asset;
}

//--------------------------------------------------------------
AssetSound * Assets::initSound() {
    return new AssetSound();
}

void Assets::killSound(AssetSound * asset) {
    delete asset;
}

};