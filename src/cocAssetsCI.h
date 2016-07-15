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
class AssetsCI : public coc::Assets {

public:

    AssetsCI();
    ~AssetsCI();
    
    void load(std::string assetID) override;
    void unload(std::string assetID) override;
    
    void update(float timeDelta=0) override;
    
    ci::gl::TextureRef getTexture(std::string assetID);
    
protected:

    Asset * initAsset() override;
    void killAsset(Asset * asset) override;
    
};

};

#endif