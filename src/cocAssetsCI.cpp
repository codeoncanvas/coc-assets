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
#include "cinder/Log.h"

#if defined( COC_CI )

namespace coc {

//--------------------------------------------------------------
AssetsCI::AssetsCI() : coc::Assets() {
    asyncLoader = AssetAsyncLoaderCIRef(new AssetAsyncLoaderCI());
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
AssetTextureRef AssetsCI::getTexture(std::string assetID) {
    AssetRef asset = getAssetByID(assetID);
    return getTexture(asset);
}

AssetTextureRef AssetsCI::getTexture(AssetRef asset) {
    if(asset == nullptr) {
        return nullptr;
    }
    if(asset->type != AssetTypeTexture) {
        return nullptr;
    }
    AssetTextureRef assetTexture = std::static_pointer_cast<AssetTexture>(asset);
    return assetTexture;
}

//--------------------------------------------------------------
ci::gl::TextureRef AssetsCI::getTextureRef(std::string assetID) {
    ci::gl::TextureRef texture = nullptr;
    
    AssetTextureRef asset = getTexture(assetID);
    if(asset == nullptr) {
        return texture;
    }
    return asset->textureRef;
}

//--------------------------------------------------------------
AssetSoundRef AssetsCI::getSound(std::string assetID) {
    AssetRef asset = getAssetByID(assetID);
    return getSound(asset);
}

AssetSoundRef AssetsCI::getSound(AssetRef asset) {
    if(asset == nullptr) {
        return nullptr;
    }
    if(asset->type != AssetTypeSound) {
        return nullptr;
    }
    AssetSoundRef assetSound = std::static_pointer_cast<AssetSound>(asset);
    return assetSound;
}

//--------------------------------------------------------------
AssetRef AssetsCI::initTexture() {
    return AssetTextureRef(new AssetTexture());
}

AssetRef AssetsCI::initSound() {
    return AssetSoundRef(new AssetSound());
}

//--------------------------------------------------------------
void AssetsCI::loadTexture(AssetRef asset) {
    AssetTextureRef assetTexture = getTexture(asset);
    if(assetTexture == nullptr) {
        return;
    }

    assetTexture->textureRef = ci::gl::Texture::create(ci::loadImage(assetTexture->assetPath), assetTexture->textureFormat);
    assetTexture->bLoaded = (assetTexture->textureRef != nullptr);
}

void AssetsCI::unloadTexture(AssetRef asset) {
    AssetTextureRef assetTexture = getTexture(asset);
    if(assetTexture == nullptr) {
        return;
    }
    
    assetTexture->textureRef = nullptr;
    assetTexture->bLoaded = false;
}

//--------------------------------------------------------------
void AssetsCI::loadSound(AssetRef asset) {
    AssetSoundRef assetSound = getSound(asset);
    if(assetSound == nullptr) {
        return;
    }

    assetSound->sourceFileRef = ci::audio::load(ci::app::loadAsset(assetSound->assetPath));
    assetSound->voiceSamplePlayerNodeRef = ci::audio::Voice::create(assetSound->sourceFileRef);
    assetSound->samplePlayerNodeRef = assetSound->voiceSamplePlayerNodeRef->getSamplePlayerNode();
    assetSound->bLoaded = (assetSound->sourceFileRef != nullptr);
}

void AssetsCI::unloadSound(AssetRef asset) {
    AssetSoundRef assetSound = getSound(asset);
    if(assetSound == nullptr) {
        return;
    }

    assetSound->samplePlayerNodeRef = nullptr;
    assetSound->voiceSamplePlayerNodeRef = nullptr;
    assetSound->sourceFileRef = nullptr;
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
		
        if(asset) {
            AssetTexture * assetTexture = (AssetTexture *)asset;
            assetTexture->textureRef = ci::gl::Texture::create(ci::loadImage(assetTexture->assetPath), assetTexture->textureFormat);
            assetTexture->bLoaded = (assetTexture->textureRef != nullptr);
            asset = NULL;
        }
	}
}

//--------------------------------------------------------------

bool AssetsCI::fileExists( std::string assetPath )
{
    if (!assetPath.length()) {
        CI_LOG_E("Asset path is empty!");
        return false;
    }
    bool result = ci::fs::exists( assetPath );
    if (!result) {
        CI_LOG_E("Asset does not exist! " << assetPath );
        return false;
    }
    return true;
}

};

#endif
