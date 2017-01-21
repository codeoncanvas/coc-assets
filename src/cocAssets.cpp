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
Assets::Assets():
bLoading(false),
bVerbose(false) {
    //
}

Assets::~Assets() {
    //
}

//--------------------------------------------------------------
AssetRef Assets::addAsset(std::string assetPath, AssetType assetType, std::string assetID, std::string groupID) {

    AssetRef asset = getAssetByPath(assetPath);
    if(asset != nullptr) {
        return asset;   // asset with that ID already exists, do not add again.
    }
    
    if(fileExists(assetPath) == false) { // check if asset exists.
        return nullptr;
    }
    
    if(assetType == AssetTypeTexture) {
        asset = initTexture();
    } else if(assetType == AssetTypeSound) {
        asset = initSound();
    } else {
        return nullptr;
    }
    
    asset->type = assetType;
    asset->assetPath = assetPath;
    asset->assetID = assetID;
    asset->groupID = groupID;
    if(asset->assetID.length() == 0) {
        asset->assetID = asset->assetPath;
    }
    
    assets.push_back(asset);
    
    return asset;
}

AssetRef Assets::addAssetAndLoad(std::string assetPath, AssetType assetType, std::string assetID, std::string groupID) {
    AssetRef asset = addAsset(assetPath, assetType, assetID, groupID);
    if(asset == nullptr) {
        return asset;
    }
    load(asset);
    return asset;
}

AssetRef Assets::addAssetAndLoadAsync(std::string assetPath, AssetType assetType, std::string assetID, std::string groupID) {
    AssetRef asset = addAsset(assetPath, assetType, assetID, groupID);
    if(asset == nullptr) {
        return asset;
    }
    loadAsync(asset);
    return asset;
}

//--------------------------------------------------------------
AssetRef Assets::removeAsset(std::string assetID) {
    AssetRef asset = getAssetByID(assetID);
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

void Assets::removeGroup(std::string groupID) {
    for(int i=0; i<assets.size(); i++) {
        if(assets[i]->groupID != groupID) {
            continue;
        }
        
        unload(assets[i]);
        
        assets.erase(assets.begin() + i--);
    }
}

//--------------------------------------------------------------
AssetRef Assets::load(std::string assetID, bool bForceReload) {
    AssetRef asset = getAssetByID(assetID);
    return load(asset, bForceReload);
}

AssetRef Assets::load(AssetRef asset, bool bForceReload) {
    if(asset == nullptr) {
        return asset;
    }
    
    bool bLoad = true;
    bLoad = bLoad && (asset->bLoaded == false);
    bLoad = bLoad || bForceReload;
    if(bLoad == false) {
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
AssetRef Assets::loadAsync(std::string assetID, bool bForceReload) {
    AssetRef asset = getAssetByID(assetID);
    return loadAsync(asset, bForceReload);
}

AssetRef Assets::loadAsync(AssetRef asset, bool bForceReload) {
    if(asset == nullptr) {
        return asset;
    }
    
    if(asset->type != AssetTypeTexture) {
        // async loading only applied to gl textures.
        // other asset types like audio or video already handle async loading.
        return asset;
    }
    
    bool bLoad = true;
    bLoad = bLoad && (asset->bLoaded == false);
    bLoad = bLoad || bForceReload;
    if(bLoad == false) {
        return asset;
    }
    
    assetLoadQueue.push_back(asset);
    
    return asset;
}

//--------------------------------------------------------------
AssetRef Assets::unload(std::string assetID) {
    AssetRef asset = getAssetByID(assetID);
    return unload(asset);
}

AssetRef Assets::unload(AssetRef asset) {
    if(asset == nullptr) {
        return asset;
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
    if(assetLoading == asset) {
        assetLoading = nullptr;
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
void Assets::unloadGroup(std::string groupID) {
    for(int i=0; i<assets.size(); i++) {
        if(assets[i]->groupID != groupID) {
            continue;
        }
        unload(assets[i]);
    }
}

void Assets::unloadAll() {
    for(int i=0; i<assets.size(); i++) {
        unload(assets[i]);
    }
}

void Assets::clearLoadQueue() {
    assetLoadQueue.clear();
    assetLoading = nullptr;
}

//--------------------------------------------------------------
AssetRef Assets::getAssetByID(std::string assetID) const {
    for(int i=0; i<assets.size(); i++) {
        if(assets[i]->assetID == assetID) {
            return assets[i];
        }
    }
    return nullptr;
}

AssetRef Assets::getAssetByPath(std::string assetPath) const {
    for(int i=0; i<assets.size(); i++) {
        if(assets[i]->assetPath == assetPath) {
            return assets[i];
        }
    }
    return nullptr;
}

//--------------------------------------------------------------
void Assets::update(float timeDelta) {

    updateAsyncLoader(timeDelta);

    for(int i=0; i<assets.size(); i++) {
        Asset & asset = *assets[i];
        
        if(asset.bLoaded == false) {
            continue;
        }
        
        //
    }
}

void Assets::updateAsyncLoader(float timeDelta) {
    //
}

};
