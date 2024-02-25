#pragma once
#include <unordered_map>
#include <vector>
#include <iostream>

#include "GraphicElement.h"

namespace graphic_sys_lib {
    class TileMap :
        public GraphicElement {
    public:
        TileMap()
            : GraphicElement() {
        }

        void render() const override;

        void assemble() noexcept;

        inline void clear() {
            mAssembledMap.clear();
            mTileSet.clear();
            mMap.clear();
            mCommonTileSizeX = 0;
            mCommonTileSizeY = 0;
        }

        inline void pushSet(int key, Texture2D&& value) noexcept {
            if (isExistSet(key))
                std::clog << "Key in TileSet like " << key << " exist, so it refuses\n";
            else
                mTileSet[key] = std::move(value);
        }
        _NODISCARD bool isExistSet(int key) const noexcept;
        _NODISCARD Texture2D& getTexture(int key);
        _NODISCARD Texture2D getTexture(int key) const;
        inline void popSet(int key) {
            if (isExistSet(key))
                mTileSet.erase(key);
            else
                std::clog << "Key value wasn't existed\n";
        }

        inline _NODISCARD std::vector<std::vector<int>> getMap() const noexcept {
            return mMap;
        }
        inline _NODISCARD std::vector<std::vector<int>>& getMap() noexcept {
            return mMap;
        }

        inline void setCommonTileSize(size_t x, size_t y) {
            mCommonTileSizeX = x;
            mCommonTileSizeY = y;
        }
        inline _NODISCARD size_t getCommonTileSizeX() const noexcept {
            return mCommonTileSizeX;
        }
        inline _NODISCARD size_t getCommonTileSizeY() const noexcept {
            return mCommonTileSizeY;
        }

        void parseFile(const std::string& pathToCMPT) override;
        _NODISCARD bool isParsableFile(const std::string& pathToCMPT) override;
        void parseFloatVar(const hardware_envi_lib::float_var floatVar) override;
        _NODISCARD bool isParsableFloatVar(const hardware_envi_lib::float_var floatVar) override;

        ~TileMap();
    private:
        std::unordered_map<int, Texture2D> mTileSet;
        std::vector<Texture*> mAssembledMap;
        std::vector<std::vector<int>> mMap;
        size_t mCommonTileSizeX = 0;
        size_t mCommonTileSizeY = 0;
    };
}
