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
#include "cinder/app/App.h"

#if defined( COC_CI )

namespace coc {

//--------------------------------------------------------------
AssetsCI::AssetsCI() : coc::Assets() {
    asyncLoader = new AssetAsyncLoaderCI();
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
const AssetTextureCI & AssetsCI::getTexture(std::string assetID) {
    AssetTextureCI * asset = (AssetTextureCI *)getAssetPtr(assetID);
    return *asset;
}

ci::gl::TextureRef AssetsCI::getTextureRef(std::string assetID) {
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

//--------------------------------------------------------------
AssetAsyncLoaderCI::AssetAsyncLoaderCI() : AssetAsyncLoader() {
    ci::gl::ContextRef backgroundCtx = ci::gl::Context::create( ci::gl::context() );
	thread = std::shared_ptr<std::thread>( new std::thread( bind( &AssetAsyncLoaderCI::loadThreadFn, this, backgroundCtx ) ) );
}

AssetAsyncLoaderCI::~AssetAsyncLoaderCI() {
    thread->join();
}

void AssetAsyncLoaderCI::loadThreadFn(ci::gl::ContextRef context) {

	ci::ThreadSetup threadSetup;
	context->makeCurrent();

	while(bRunning) {
		
        if(asset == NULL) {
            continue;
        }
        
        AssetTextureCI * texture = (AssetTextureCI *)asset;
        texture->textureRef = ci::gl::Texture::create(ci::loadImage(texture->assetPath));
        texture->bLoaded = (texture->textureRef.get() != NULL);
        asset = NULL;
	}
}

};

#endif