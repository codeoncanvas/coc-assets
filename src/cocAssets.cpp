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
AssetRef Assets::addAsset(std::string assetPath, AssetType assetType, std::string assetID) {

    AssetRef asset = getAsset(assetID);
    if(asset != nullptr) {
        return asset;   // asset with that ID already exists, do not add again.
    }
    
    if(fileExists(assetPath) == false) { // check if asset exists.
        return asset;
    }
    
    if(assetType == AssetTypeTexture) {
        asset = initTexture();
    } else if(assetType == AssetTypeSound) {
        asset = initSound();
    } else {
        return asset;
    }
    
    asset->type = assetType;
    asset->assetPath = assetPath;
    asset->assetID = assetID;
    if(asset->assetID.length() == 0) {
        asset->assetID = asset->assetPath;
    }
    
    assets.push_back(asset);
    
    return asset;
}

AssetRef Assets::addAssetAndLoad(std::string assetPath, AssetType assetType, std::string assetID) {
    AssetRef asset = addAsset(assetPath, assetType, assetID);
    if(asset == nullptr) {
        return asset;
    }
    load(asset);
    return asset;
}

AssetRef Assets::addAssetAndLoadAsync(std::string assetPath, AssetType assetType, std::string assetID) {
    AssetRef asset = addAsset(assetPath, assetType, assetID);
    if(asset == nullptr) {
        return asset;
    }
    loadAsync(asset);
    return asset;
}

//--------------------------------------------------------------
AssetRef Assets::removeAsset(std::string assetID) {
    AssetRef asset = getAsset(assetID);
    return removeAsset(asset);
}

AssetRef Assets::removeAsset(AssetRef asset) {
    if(asset == nullptr) {
        return asset;
    }

    unload(asset);

    for(int i=0; i<assets.size(); i++) {
        if(assets[i] != asset) {
            continue;
        }
        assets.erase(assets.begin() + i);
        break;
    }
    
    return asset;
}

//--------------------------------------------------------------
AssetRef Assets::load(std::string assetID) {
    AssetRef asset = getAsset(assetID);
    return load(asset);
}

AssetRef Assets::load(AssetRef asset) {
    if(asset == nullptr) {
        return asset;
    }
    if(asset->bLoaded == true) {
        return asset;
    }
    if(asset->type == AssetTypeTexture) {
        loadTexture(asset);
    } else if(asset->type == AssetTypeSound) {
        loadSound(asset);
    }
    return asset;
}

//--------------------------------------------------------------
AssetRef Assets::loadAsync(std::string assetID) {
    AssetRef asset = getAsset(assetID);
    return loadAsync(asset);
}

AssetRef Assets::loadAsync(AssetRef asset) {
    if(asset == nullptr) {
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
AssetRef Assets::unload(std::string assetID) {
    AssetRef asset = getAsset(assetID);
    return unload(asset);
}

AssetRef Assets::unload(AssetRef asset) {
    if(asset == nullptr) {
        return asset;
    }
    
    //----------------------------------------------------------
    // if asset is loading, stop it.
    //----------------------------------------------------------
    if(assetLoading == asset) {
        asyncLoader->cancel();
        assetLoading = nullptr;
    }

    //----------------------------------------------------------
    // remove asset from load queue.
    //----------------------------------------------------------
    for(int i=0; i<assetLoadQueue.size(); i++) {
        if(assetLoadQueue[i] == asset) {
            assetLoadQueue.erase(assetLoadQueue.begin() + i);
            break;
        }
    }

    //----------------------------------------------------------
    // unload asset.
    //----------------------------------------------------------
    if(asset->bLoaded == false) {
        return asset;
    }
    if(asset->type == AssetTypeTexture) {
        unloadTexture(asset);
    } else if(asset->type == AssetTypeSound) {
        unloadSound(asset);
    }
    
    return asset;
}

//--------------------------------------------------------------
void Assets::unloadAll() {
    for(int i=0; i<assets.size(); i++) {
        unload(assets[i]);
    }
}

void Assets::clearLoadQueue() {
    assetLoadQueue.clear();
    asyncLoader->cancel();
    assetLoading = nullptr;
}

//--------------------------------------------------------------
AssetRef Assets::getAsset(std::string assetID) const {
    for(int i=0; i<assets.size(); i++) {
        if(assets[i]->assetID == assetID) {
            return assets[i];
        }
    }
    return nullptr;
}

//--------------------------------------------------------------
void Assets::update(float timeDelta) {

    bool bLoadingAsync = true;
    bLoadingAsync = bLoadingAsync && (assetLoading != nullptr);
    if(bLoadingAsync) {
        if(assetLoading->bLoaded == true) { // finished loading.
            assetLoading = nullptr;
        }
    }

    bool bLoadAsync = true;
    bLoadAsync = bLoadAsync && (asyncLoader != nullptr);
    bLoadAsync = bLoadAsync && (assetLoading == nullptr);
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

};
