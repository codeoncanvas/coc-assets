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
    assetLoading = NULL;
    asyncLoader = NULL;
}

Assets::~Assets() {
    //
}

//--------------------------------------------------------------
const Asset * Assets::addAsset(std::string assetPath, AssetType assetType, std::string assetID) {

    if ( !fileExists(assetPath) ) return NULL;

    if(getAsset(assetID) != NULL) {
        // asset with that ID already exists.
        // do not add again.
        return NULL;
    }
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

const Asset * Assets::addAssetAndLoad(std::string assetPath, AssetType assetType, std::string assetID) {
    const Asset * asset = addAsset(assetPath, assetType, assetID);
    if(asset == NULL) {
        return asset;
    }
    load(asset->assetID);
    return asset;
}

const Asset * Assets::addAssetAndLoadAsync(std::string assetPath, AssetType assetType, std::string assetID) {
    const Asset * asset = addAsset(assetPath, assetType, assetID);
    if(asset == NULL) {
        return asset;
    }
    loadAsync(asset->assetID);
    return asset;
}

void Assets::removeAsset(std::string assetID) {

    //----------------------------------------------------------
    // if asset is loading, stop it.
    //----------------------------------------------------------
    if(assetLoading != NULL) {
        if(assetLoading->assetID == assetID) {
            asyncLoader->cancel();
            assetLoading = NULL;
        }
    }

    //----------------------------------------------------------
    // remove asset from load queue.
    //----------------------------------------------------------
    for(int i=0; i<assetLoadQueue.size(); i++) {
        if(assetLoadQueue[i]->assetID == assetID) {
            assetLoadQueue.erase(assetLoadQueue.begin() + i);
            break;
        }
    }

    //----------------------------------------------------------
    // remove asset.
    //----------------------------------------------------------
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
const Asset * Assets::load(std::string assetID) {
    Asset * asset = getAssetPtr(assetID);
    if(asset == NULL) {
        return asset;
    }
    if(asset->bLoaded == true) {
        return asset;
    }
    if(asset->type == AssetTypeTexture) {
        loadTexture(assetID);
    } else if(asset->type == AssetTypeSound) {
        loadSound(assetID);
    }
    return asset;
}

const Asset * Assets::loadAsync(std::string assetID) {
    Asset * asset = getAssetPtr(assetID);
    if(asset == NULL) {
        return asset;
    }
    if(asset->type != AssetTypeTexture) {
        // async loading only applied to gl textures.
        // other asset types like audio or video already handle async loading.
        return asset;
    }
    assetLoadQueue.push_back(asset);
    return asset;
}

//--------------------------------------------------------------
void Assets::unload(std::string assetID) {
    
    //----------------------------------------------------------
    // if asset is loading, stop it.
    //----------------------------------------------------------
    if(assetLoading != NULL) {
        if(assetLoading->assetID == assetID) {
            asyncLoader->cancel();
            assetLoading = NULL;
        }
    }

    //----------------------------------------------------------
    // remove asset from load queue.
    //----------------------------------------------------------
    for(int i=0; i<assetLoadQueue.size(); i++) {
        if(assetLoadQueue[i]->assetID == assetID) {
            assetLoadQueue.erase(assetLoadQueue.begin() + i);
            break;
        }
    }

    //----------------------------------------------------------
    // unload asset.
    //----------------------------------------------------------
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

void Assets::unloadAll() {
    for(int i=0; i<assets.size(); i++) {
        unload(assets[i]->assetID);
    }
}

void Assets::clearLoadQueue() {
    assetLoadQueue.clear();
    asyncLoader->cancel();
    assetLoading = NULL;
}

//--------------------------------------------------------------
void Assets::update(float timeDelta) {

    bool bLoadingAsync = true;
    bLoadingAsync = bLoadingAsync && (assetLoading != NULL);
    if(bLoadingAsync) {
        if(assetLoading->bLoaded == true) { // finished loading.
            assetLoading = NULL;
        }
    }

    bool bLoadAsync = true;
    bLoadAsync = bLoadAsync && (asyncLoader != NULL);
    bLoadAsync = bLoadAsync && (assetLoading == NULL);
    bLoadAsync = bLoadAsync && (assetLoadQueue.size() > 0);
    if(bLoadAsync) {
    
        assetLoading = assetLoadQueue[0];
        assetLoadQueue.erase(assetLoadQueue.begin());
        asyncLoader->load(assetLoading);
    }

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