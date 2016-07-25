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

namespace coc {

//--------------------------------------------------------------
enum AssetType {
    AssetTypeNone = 0,
    AssetTypeTexture,
    AssetTypeSound
};

//--------------------------------------------------------------
class Asset {
public:
    Asset() {
        type = AssetTypeNone;
        assetPath = "";
        assetID = "";
        bLoaded = false;
    }
    
    AssetType type;
    std::string assetPath;
    std::string assetID;
    bool bLoaded;
};

//--------------------------------------------------------------
class AssetTexture : public Asset {
public:
    AssetTexture() : Asset() {
        type = AssetTypeTexture;
    }
};

//--------------------------------------------------------------
class AssetSound : public Asset {
public:
    AssetSound() : Asset() {
        type = AssetTypeSound;
    }
};

//--------------------------------------------------------------
class AssetAsyncLoader {

public:
    
    AssetAsyncLoader() : asset(NULL), bRunning(true) {}
    ~AssetAsyncLoader() {
        bRunning = false;
    }

    virtual void load(Asset * assetPtr) { asset = assetPtr; }
    virtual void cancel() { asset = NULL; }
    
    Asset * asset;
    bool bRunning;
};

//--------------------------------------------------------------
class Assets {

public:

    Assets();
    ~Assets();
    
    virtual const Asset * addAsset(std::string assetPath, AssetType assetType, std::string assetID="");
    virtual const Asset * addAssetAndLoad(std::string assetPath, AssetType assetType, std::string assetID="");
    virtual const Asset * addAssetAndLoadAsync(std::string assetPath, AssetType assetType, std::string assetID="");
    virtual void removeAsset(std::string assetID);
    
    virtual const Asset * load(std::string assetID);
    virtual const Asset * loadAsync(std::string assetID);
    
    virtual void unload(std::string assetID);
    virtual void unloadAll();
    virtual void clearLoadQueue();
    
    virtual void update(float timeDelta=0);
    
    const Asset * getAsset(std::string assetID) const;
    
protected:

    Asset * getAssetPtr(std::string assetID) const;

    virtual AssetTexture * initTexture();
    virtual void killTexture(AssetTexture * asset);
    
    virtual AssetSound * initSound();
    virtual void killSound(AssetSound * asset);
    
    virtual void loadTexture(std::string assetID) {}
    virtual void unloadTexture(std::string assetID) {}
    
    virtual void loadSound(std::string assetID) {}
    virtual void unloadSound(std::string assetID) {}
    
    std::vector<Asset *> assets;
    std::vector<Asset *> assetLoadQueue;
    Asset * assetLoading;
    AssetAsyncLoader * asyncLoader;
};

};