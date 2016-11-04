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
class AssetTextureCI : public AssetTexture {
public:
    AssetTextureCI() : AssetTexture() {
        //
    }
    
    ci::gl::TextureRef textureRef;
};

//--------------------------------------------------------------
class AssetSoundCI : public AssetSound {

public:

    AssetSoundCI() : AssetSound() {
            //
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
    
    const AssetTextureCI & getTexture(std::string assetID);
    ci::gl::TextureRef getTextureRef(std::string assetID);

	bool fileExists( std::string assetPath) override;
    
protected:

    AssetTexture * initTexture() override;
    void killTexture(AssetTexture * asset) override;

    AssetSound * initSound() override;
    void killSound(AssetSound * asset) override;
    
    void loadTexture(std::string assetID) override;
    void unloadTexture(std::string assetID) override;
    
    void loadSound(std::string assetID) override;
    void unloadSound(std::string assetID) override;
    
};

//--------------------------------------------------------------
class AssetAsyncLoaderCI : public AssetAsyncLoader {

public:

    AssetAsyncLoaderCI();
    ~AssetAsyncLoaderCI();

    void loadThreadFn(ci::gl::ContextRef context);

	std::shared_ptr<std::thread> thread;
};

};

#endif