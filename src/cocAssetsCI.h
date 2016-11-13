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

#include "cocAssets.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/audio.h"

#if defined( COC_CI )

namespace coc {

//--------------------------------------------------------------
class AssetTexture;
typedef std::shared_ptr<AssetTexture> AssetTextureRef;

class AssetTexture : public Asset {
public:
    AssetTexture() :
    Asset() {
        type = AssetTypeTexture;
    }
    
    ci::gl::Texture::Format textureFormat;
    ci::gl::TextureRef textureRef;
};

//--------------------------------------------------------------
class AssetSound;
typedef std::shared_ptr<AssetSound> AssetSoundRef;

class AssetSound : public Asset {
public:
    AssetSound() :
    Asset() {
        type = AssetTypeSound;
    }
    
    ci::audio::SourceFileRef sourceFileRef;
    ci::audio::VoiceSamplePlayerNodeRef voiceSamplePlayerNodeRef;
    ci::audio::SamplePlayerNodeRef samplePlayerNodeRef;
};

//--------------------------------------------------------------
class AssetsCI : public coc::Assets {

public:

    AssetsCI();
    ~AssetsCI();
    
    void update(float timeDelta=0) override;
    
    AssetTextureRef getTexture(std::string assetID);
    AssetTextureRef getTexture(AssetRef asset);
    ci::gl::TextureRef getTextureRef(std::string assetID);
    
    AssetSoundRef getSound(std::string assetID);
    AssetSoundRef getSound(AssetRef asset);

	bool fileExists(std::string assetPath) override;
    
protected:

    AssetRef initTexture() override;
    AssetRef initSound() override;
    
    void loadTexture(AssetRef asset) override;
    void unloadTexture(AssetRef asset) override;
    
    void loadSound(AssetRef asset) override;
    void unloadSound(AssetRef asset) override;
    
};

//--------------------------------------------------------------
class AssetAsyncLoaderCI;
typedef std::shared_ptr<AssetAsyncLoaderCI> AssetAsyncLoaderCIRef;

class AssetAsyncLoaderCI : public AssetAsyncLoader {

public:

    AssetAsyncLoaderCI();
    ~AssetAsyncLoaderCI();

    void loadThreadFn(ci::gl::ContextRef context);

	std::shared_ptr<std::thread> thread;
};

};

#endif
