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

using namespace ci;
using namespace std;

//--------------------------------------------------------------
AssetsCI::AssetsCI() : coc::Assets() {
    //
}

AssetsCI::~AssetsCI() {
    //
}

//--------------------------------------------------------------
void AssetsCI::update(float timeDelta) {
    Assets::update(timeDelta);

    //
}

void AssetsCI::updateAsyncLoader(float timeDelta) {

    bool bInitAsyncLoader = true;
    bInitAsyncLoader = bInitAsyncLoader && (asyncLoader == nullptr);
    bInitAsyncLoader = bInitAsyncLoader && (assetLoadQueue.size() > 0);
    if(bInitAsyncLoader) {
        asyncLoader = AssetAsyncLoaderCIRef(new AssetAsyncLoaderCI());
    }
    
    if(asyncLoader == nullptr) {
        return;
    }

    AssetAsyncLoaderCIRef asyncLoader = getAsyncLoader();

    bool bLoaded = true;
    bLoaded = bLoaded && bLoading;
    bLoaded = bLoaded && asyncLoader->textures.isNotEmpty();
    if(bLoaded) {
        if(assetLoading) {
            AssetTextureRef assetLoaded = getTexture(assetLoading);
            asyncLoader->textures.popBack( &assetLoaded->textureRef );
            assetLoaded->bLoaded = (assetLoaded->textureRef != nullptr);
            assetLoading = nullptr;
        }
        bLoading = false;
    }
    
    bool bLoad = true;
    bLoad = bLoad && (bLoading == false);
    bLoad = bLoad && (assetLoadQueue.size() > 0);
    if(bLoad) {
        bLoading = true;
        assetLoading = assetLoadQueue[0];
        assetLoadQueue.erase(assetLoadQueue.begin());
        asyncLoader->asset = assetLoading.get();
    }
}

//--------------------------------------------------------------
AssetTextureRef AssetsCI::getTexture(string assetID) {
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
    AssetTextureRef assetTexture = static_pointer_cast<AssetTexture>(asset);
    return assetTexture;
}

//--------------------------------------------------------------
gl::TextureRef AssetsCI::getTextureRef(string assetID) {
    gl::TextureRef texture = nullptr;
    
    AssetTextureRef asset = getTexture(assetID);
    if(asset == nullptr) {
        return texture;
    }
    return asset->textureRef;
}

//--------------------------------------------------------------
AssetSoundRef AssetsCI::getSound(string assetID) {
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
    AssetSoundRef assetSound = static_pointer_cast<AssetSound>(asset);
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
    
    try {
        
        assetTexture->textureRef = gl::Texture::create(loadImage(assetTexture->assetPath), assetTexture->textureFormat);
        assetTexture->bLoaded = (assetTexture->textureRef != nullptr);
        
    } catch( ci::Exception &exc ) {
    
        std::cout << "AssetsCI::loadTexture failed, what: " << exc.what() << std::endl;
    }
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

    try {
        
        assetSound->sourceFileRef = audio::load(app::loadAsset(assetSound->assetPath));
        assetSound->voiceSamplePlayerNodeRef = audio::Voice::create(assetSound->sourceFileRef);
        assetSound->samplePlayerNodeRef = assetSound->voiceSamplePlayerNodeRef->getSamplePlayerNode();
        assetSound->bLoaded = (assetSound->sourceFileRef != nullptr);
        
    } catch( ci::Exception &exc ) {
    
        std::cout << "AssetsCI::loadSound failed, what: " << exc.what() << std::endl;
    }
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
AssetAsyncLoaderCI::AssetAsyncLoaderCI() :
AssetAsyncLoader(),
textures(1) {

    gl::ContextRef backgroundCtx = gl::Context::create( gl::context() );
	thread = shared_ptr<std::thread>( new std::thread( bind( &AssetAsyncLoaderCI::loadThreadFn, this, backgroundCtx ) ) );
}

AssetAsyncLoaderCI::~AssetAsyncLoaderCI() {
    
    textures.cancel();
    thread->join();
}

void AssetAsyncLoaderCI::loadThreadFn(gl::ContextRef context) {

	ThreadSetup threadSetup;
	context->makeCurrent();

	while(bRunning) {
		
        if(asset) {
            try {
                AssetTexture * assetTexture = (AssetTexture *)asset;
                string texturePath = assetTexture->assetPath;
                gl::Texture::Format textureFormat = assetTexture->textureFormat;
                
                ImageSourceRef textureSrc = loadImage(texturePath);
                gl::TextureRef texture = gl::Texture::create(textureSrc, textureFormat);
                
                auto fence = gl::Sync::create();
                fence->clientWaitSync();
                
                textures.pushFront(texture);

            } catch( ci::Exception &exc ) {
            
                //
            }
        }
	}
}

//--------------------------------------------------------------

bool AssetsCI::fileExists( string assetPath ) {

    bool bFileExists = (assetPath.length() > 0);
    if(bFileExists) {
        bFileExists = fs::exists( assetPath );
    }
    if(bFileExists == false) {
        if(bVerbose) {
            CI_LOG_E("Asset does not exist! " << assetPath );
        }
        return false;
    }
    return true;
}

};

#endif
