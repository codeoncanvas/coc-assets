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

#pragma once

#include "cocCore.h"

namespace coc {

//--------------------------------------------------------------
enum AssetType {
    AssetTypeNone = 0,
    AssetTypeTexture,
    AssetTypeSound
};

//--------------------------------------------------------------
class Asset;
typedef std::shared_ptr<Asset> AssetRef;

class Asset {
public:

    Asset() {
        type = AssetTypeNone;
        assetPath = "";
        assetID = "";
        groupID = "";
        bLoaded = false;
    }
    
    AssetType type;
    std::string assetPath;
    std::string assetID;
    std::string groupID;
    bool bLoaded;
};

//--------------------------------------------------------------
class AssetAsyncLoader;
typedef std::shared_ptr<AssetAsyncLoader> AssetAsyncLoaderRef;

class AssetAsyncLoader {

public:
    
    AssetAsyncLoader() :
    bRunning(true),
    asset(NULL) {
        //
    }
    
    ~AssetAsyncLoader() {
        bRunning = false;
    }

    bool bRunning;
    Asset * asset;
};

//--------------------------------------------------------------
class Assets {

public:

    Assets();
    ~Assets();
    
    virtual AssetRef addAsset(std::string assetPath, AssetType assetType, std::string assetID="", std::string groupID="");
    virtual AssetRef addAssetAndLoad(std::string assetPath, AssetType assetType, std::string assetID="", std::string groupID="");
    virtual AssetRef addAssetAndLoadAsync(std::string assetPath, AssetType assetType, std::string assetID="", std::string groupID="");
    
    virtual AssetRef removeAsset(std::string assetID);
    virtual AssetRef removeAsset(AssetRef asset);
    virtual void removeGroup(std::string groupID);
    
    virtual AssetRef load(std::string assetID, bool bForceReload=false);
    virtual AssetRef load(AssetRef asset, bool bForceReload=false);
    
    virtual AssetRef loadAsync(std::string assetID, bool bForceReload=false);
    virtual AssetRef loadAsync(AssetRef asset, bool bForceReload=false);
    
    virtual AssetRef unload(std::string assetID);
    virtual AssetRef unload(AssetRef asset);
    virtual void unloadGroup(std::string groupID);
    virtual void unloadAll();
    virtual void clearLoadQueue();
    
    AssetRef getAssetByID(std::string assetID) const;
    AssetRef getAssetByPath(std::string assetPath) const;
    
    virtual bool fileExists( std::string assetPath ) { return true; }
    void setVerbose(bool value) { bVerbose = value; }
    
    virtual void update(float timeDelta=0);
    virtual void updateAsyncLoader(float timeDelta=0);
    
protected:

    virtual AssetRef initTexture() { return nullptr; }
    virtual AssetRef initSound() { return nullptr; }
    
    virtual void loadTexture(AssetRef asset) {}
    virtual void unloadTexture(AssetRef asset) {}
    
    virtual void loadSound(AssetRef asset) {}
    virtual void unloadSound(AssetRef asset) {}
    
    std::vector<AssetRef> assets;
    std::vector<AssetRef> assetLoadQueue;
    AssetAsyncLoaderRef asyncLoader;
    AssetRef assetLoading;
    bool bLoading;
    bool bVerbose;
};

};
