#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cocAssetsCI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AssetsSampleApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
    
    coc::AssetsCI assets;
    std::vector<coc::AssetTextureCI *> textures;
};

void AssetsSampleApp::setup() {

    std::vector<std::string> texturePaths;
    texturePaths.push_back(getAssetPath("1.png").string());
    texturePaths.push_back(getAssetPath("2.png").string());
    texturePaths.push_back(getAssetPath("3.png").string());
    texturePaths.push_back(getAssetPath("4.png").string());
    texturePaths.push_back(getAssetPath("5.png").string());
    texturePaths.push_back(getAssetPath("6.png").string());
    texturePaths.push_back(getAssetPath("7.png").string());
    texturePaths.push_back(getAssetPath("8.png").string());
    texturePaths.push_back(getAssetPath("9.png").string());
    
    bool bLoadAsync = true; // set to true for async loading, or false for immediate loading.

    for(int i=0; i<texturePaths.size(); i++) {
        
        std::string texturePath = texturePaths[i];
        coc::AssetTextureCI * texture = NULL;
        
        if(bLoadAsync) {
            texture = (coc::AssetTextureCI *)assets.addAssetAndLoadAsync(texturePath, coc::AssetTypeTexture);
        } else {
            texture = (coc::AssetTextureCI *)assets.addAssetAndLoad(texturePath, coc::AssetTypeTexture);
        }
        if(texture == NULL) {
            continue;
        }
        textures.push_back(texture);
    }
}

void AssetsSampleApp::update() {
    
    assets.update();
}

void AssetsSampleApp::draw() {

    gl::clear(Color(0, 0, 0));
    
    int w = getWindowWidth();
    int h = getWindowHeight();
    int rows = 3;
    
    float size = w;
    if(size > h) {
        size = h;
    }
    size /= (float)rows;

    for(int i=0; i<textures.size(); i++) {
        coc::AssetTextureCI * texture = textures[i];
        if(texture->bLoaded == false) {
            continue;
        }
        
        int ix = i % rows;
        int iy = i / rows;
        
        float x1 = ix * size;
        float y1 = iy * size;
        float x2 = x1 + size;
        float y2 = y1 + size;
        
        gl::draw(texture->textureRef, ci::Rectf(x1, y1, x2, y2));
    }
}

CINDER_APP( AssetsSampleApp, RendererGl )
